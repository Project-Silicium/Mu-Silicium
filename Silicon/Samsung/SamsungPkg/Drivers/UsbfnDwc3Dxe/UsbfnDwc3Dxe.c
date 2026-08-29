#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <IndustryStandard/Usb.h>

#include <Protocol/EFIChipInfo.h>

#include "UsbfnDwc3Dxe.h"

STATIC CHAR16 mManufacturerName[] = L"Mu-Silicium";
STATIC CHAR16 mProductName[]      = L"Samsung Exynos Device";
STATIC CHAR16 mSerialNumber[]     = L"0123456789ABCDEF";

STATIC
VOID
Dwc3BuildSerialNumber (VOID)
{
  EFI_STATUS              Status;
  EFI_CHIP_INFO_PROTOCOL *ChipInfo;
  UINT64                  ChipId;
  UINT8                   Index;

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID *)&ChipInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "Failed to locate ChipInfo protocol, serial number will not be accurate! Status = %r\n", Status));
    return;
  }

  Status = ChipInfo->GetId (&ChipId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "Failed to read the Chip ID! Status = %r\n", Status));
    return;
  }

  for (Index = 0; Index < 16; Index++) {
    UINT8 Nibble = (UINT8)((ChipId >> ((15 - Index) * 4)) & 0xF);

    mSerialNumber[Index] = (CHAR16)((Nibble < 10) ? (L'0' + Nibble) : (L'A' + Nibble - 10));
  }
}

STATIC
EFI_STATUS
Dwc3ValidateEndpoint (
  IN  DWC3_DEV                     *Dev,
  IN  UINT8                         EndpointIndex,
  IN  EFI_USBFN_ENDPOINT_DIRECTION  Direction,
  OUT UINT8                        *PhysEpNum)
{
  UINT8 Phys;

  if (Direction != EfiUsbEndpointDirectionDeviceRx && Direction != EfiUsbEndpointDirectionDeviceTx) {
    return EFI_INVALID_PARAMETER;
  }

  if (EndpointIndex >= DWC3_MAX_LOGICAL_EP) {
    return EFI_INVALID_PARAMETER;
  }

  Phys = DWC3_PHYS_EP (EndpointIndex, Direction);

  if (Phys >= Dev->NumPhysicalEps) {
    return EFI_INVALID_PARAMETER;
  }

  *PhysEpNum = Phys;

  return EFI_SUCCESS;
}

STATIC
UINT16
Dwc3MaxPacketSizeForSpeed (
  IN EFI_USB_ENDPOINT_TYPE EndpointType,
  IN EFI_USB_BUS_SPEED     BusSpeed)
{
  if (EndpointType == UsbEndpointControl) {
    switch (BusSpeed) {
      case UsbBusSpeedLow:
        return 8;
      case UsbBusSpeedSuper:
        return 512;
      default:
        return 64;
    }
  }

  switch (BusSpeed) {
    case UsbBusSpeedSuper:
      return 1024;
    case UsbBusSpeedHigh:
      return 512;
    default:
      return 64;
  }
}

STATIC
BOOLEAN
Dwc3IsLinkDisconnected (IN DWC3_DEV *Dev)
{
  if (!Dev->Started) {
    return TRUE;
  }

  // Hacky, ideally we talk to whatever PD controller is here to check VBUS and cable state.
  return (BOOLEAN)(DWC3_DSTS_USBLNKST (MmioRead32 (Dev->Base + DWC3_DSTS)) == DWC3_LINK_STATE_SS_DIS);
}

STATIC
EFI_STATUS
EFIAPI
Dwc3DetectPort (
  IN  EFI_USBFN_IO_PROTOCOL *This,
  OUT EFI_USBFN_PORT_TYPE   *PortType)
{
  DWC3_DEV *Dev;

  if (This == NULL || PortType == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  if (!UsbfnPlatformIsVbusPresent () || Dwc3IsLinkDisconnected (Dev)) {
    *PortType = EfiUsbUnknownPort;
    return EFI_SUCCESS;
  }

  if (Dev->Started && Dev->Speed != UsbBusSpeedUnknown) {
    *PortType = EfiUsbStandardDownstreamPort;
  } else {
    *PortType = EfiUsbUnknownPort;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3ConfigureEnableEndpoints (
  IN  EFI_USBFN_IO_PROTOCOL *This,
  OUT EFI_USB_DEVICE_INFO   *DeviceInfo)
{
  DWC3_DEV                    *Dev;
  EFI_STATUS                   Status;
  EFI_USB_CONFIG_INFO         *ConfigInfo;
  EFI_USB_INTERFACE_INFO      *InterfaceInfo;
  EFI_USB_ENDPOINT_DESCRIPTOR *EpDesc;
  UINT8                        Index;
  UINT8                        PhysEpNum;
  UINT8                        EpType;
  UINT16                       MaxPacketSize;

  if (This == NULL || DeviceInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  if (!Dev->Started) {
    return EFI_NOT_READY;
  }

  if (DeviceInfo->ConfigInfoTable == NULL || DeviceInfo->ConfigInfoTable[0] == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ConfigInfo = DeviceInfo->ConfigInfoTable[0];

  if (ConfigInfo->InterfaceInfoTable == NULL || ConfigInfo->InterfaceInfoTable[0] == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  InterfaceInfo = ConfigInfo->InterfaceInfoTable[0];

  if (InterfaceInfo->InterfaceDescriptor == NULL || InterfaceInfo->EndpointDescriptorTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MaxPacketSize = Dwc3MaxPacketSizeForSpeed (UsbEndpointControl, Dev->Platform.MaximumSpeed);

  Status = Dwc3EnableEndpoint (Dev, DWC3_EP0_OUT, DWC3_DEPCMD_TYPE_CONTROL, MaxPacketSize, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DWC3: Failed to configure EP0 OUT! Status = %r\n", Status));
    return Status;
  }

  Status = Dwc3EnableEndpoint (Dev, DWC3_EP0_IN, DWC3_DEPCMD_TYPE_CONTROL, MaxPacketSize, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DWC3: Failed to configure EP0 IN! Status = %r\n", Status));
    return Status;
  }

  for (Index = 0; Index < InterfaceInfo->InterfaceDescriptor->NumEndpoints; Index++) {
    EpDesc = InterfaceInfo->EndpointDescriptorTable[Index];

    if (EpDesc == NULL) {
      continue;
    }

    if ((EpDesc->Attributes & USB_ENDPOINT_TYPE_MASK) != USB_ENDPOINT_BULK) {
      DEBUG ((EFI_D_ERROR, "DWC3: Endpoint 0x%02x is not Bulk!\n", EpDesc->EndpointAddress));
      return EFI_UNSUPPORTED;
    }

    EpType    = DWC3_DEPCMD_TYPE_BULK;
    PhysEpNum = (UINT8)(((EpDesc->EndpointAddress & 0x0F) << 1) |
                        ((EpDesc->EndpointAddress & USB_ENDPOINT_DIR_IN) ? 1 : 0));

    if (PhysEpNum >= Dev->NumPhysicalEps) {
      return EFI_INVALID_PARAMETER;
    }

    MaxPacketSize = Dwc3MaxPacketSizeForSpeed (UsbEndpointBulk, Dev->Platform.MaximumSpeed);

    Status = Dwc3EnableEndpoint (Dev, PhysEpNum, EpType, MaxPacketSize, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "DWC3: Failed to configure Physical EP %u! Status = %r\n", PhysEpNum, Status));
      return Status;
    }
  }

  Dwc3Ep0OutStart (Dev);

  Status = Dwc3RunStop (Dev, TRUE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3GetEndpointMaxPacketSize (
  IN  EFI_USBFN_IO_PROTOCOL *This,
  IN  EFI_USB_ENDPOINT_TYPE  EndpointType,
  IN  EFI_USB_BUS_SPEED      BusSpeed,
  OUT UINT16                *MaxPacketSize)
{
  DWC3_DEV *Dev;

  if (This == NULL || MaxPacketSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (EndpointType != UsbEndpointControl && EndpointType != UsbEndpointBulk) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  if (BusSpeed == UsbBusSpeedUnknown) {
    BusSpeed = Dev->Platform.MaximumSpeed;
  } else if (BusSpeed > UsbBusSpeedMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  *MaxPacketSize = Dwc3MaxPacketSizeForSpeed (EndpointType, BusSpeed);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3GetDeviceInfo (
  IN     EFI_USBFN_IO_PROTOCOL    *This,
  IN     EFI_USBFN_DEVICE_INFO_ID  Id,
  IN OUT UINTN                    *BufferSize,
  OUT    VOID                     *Buffer OPTIONAL)
{
  CHAR16 *String;
  UINTN   Size;

  if (This == NULL || BufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*BufferSize != 0 && Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Id) {
    case EfiUsbDeviceInfoSerialNumber:
      String = mSerialNumber;
      break;
    case EfiUsbDeviceInfoManufacturerName:
      String = mManufacturerName;
      break;
    case EfiUsbDeviceInfoProductName:
      String = mProductName;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  Size = StrSize (String);

  if (*BufferSize < Size) {
    *BufferSize = Size;
    return EFI_BUFFER_TOO_SMALL;
  }

  CopyMem (Buffer, String, Size);
  *BufferSize = Size;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3GetVendorIdProductId (
  IN  EFI_USBFN_IO_PROTOCOL *This,
  OUT UINT16                *Vid,
  OUT UINT16                *Pid)
{
  DWC3_DEV *Dev;

  if (This == NULL || Vid == NULL || Pid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  *Vid = Dev->Platform.VendorId;
  *Pid = Dev->Platform.ProductId;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3AbortTransfer (
  IN EFI_USBFN_IO_PROTOCOL        *This,
  IN UINT8                         EndpointIndex,
  IN EFI_USBFN_ENDPOINT_DIRECTION  Direction)
{
  DWC3_DEV  *Dev;
  EFI_STATUS Status;
  UINT8      PhysEpNum;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  Status = Dwc3ValidateEndpoint (Dev, EndpointIndex, Direction, &PhysEpNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!Dev->Endpoints[PhysEpNum].Enabled) {
    return EFI_INVALID_PARAMETER;
  }

  Dwc3EndTransfer (Dev, PhysEpNum);

  Dev->Endpoints[PhysEpNum].TransferBuffer = NULL;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3GetEndpointStallState (
  IN     EFI_USBFN_IO_PROTOCOL        *This,
  IN     UINT8                         EndpointIndex,
  IN     EFI_USBFN_ENDPOINT_DIRECTION  Direction,
  IN OUT BOOLEAN                      *State)
{
  DWC3_DEV  *Dev;
  EFI_STATUS Status;
  UINT8      PhysEpNum;

  if (This == NULL || State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  Status = Dwc3ValidateEndpoint (Dev, EndpointIndex, Direction, &PhysEpNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *State = Dev->Endpoints[PhysEpNum].Stalled;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3SetEndpointStallState (
  IN     EFI_USBFN_IO_PROTOCOL        *This,
  IN     UINT8                         EndpointIndex,
  IN     EFI_USBFN_ENDPOINT_DIRECTION  Direction,
  IN OUT BOOLEAN                      *State)
{
  DWC3_DEV  *Dev;
  EFI_STATUS Status;
  UINT8      PhysEpNum;

  if (This == NULL || State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  Status = Dwc3ValidateEndpoint (Dev, EndpointIndex, Direction, &PhysEpNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!Dev->Endpoints[PhysEpNum].Enabled) {
    return EFI_INVALID_PARAMETER;
  }

  if (PhysEpNum <= DWC3_EP0_IN) {
    if (*State) {
      Dwc3Ep0StallAndRestart (Dev);
      return EFI_SUCCESS;
    }

    return EFI_SUCCESS;
  }

  return Dwc3SetHalt (Dev, PhysEpNum, *State);
}

STATIC
EFI_STATUS
EFIAPI
Dwc3EventHandler (
  IN     EFI_USBFN_IO_PROTOCOL     *This,
  OUT    EFI_USBFN_MESSAGE         *Message,
  IN OUT UINTN                     *PayloadSize,
  OUT    EFI_USBFN_MESSAGE_PAYLOAD *Payload)
{
  DWC3_DEV     *Dev;
  DWC3_MESSAGE  Pending;

  if (This == NULL || Message == NULL || PayloadSize == NULL || Payload == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*PayloadSize < sizeof (EFI_USBFN_MESSAGE_PAYLOAD)) {
    *PayloadSize = sizeof (EFI_USBFN_MESSAGE_PAYLOAD);
    return EFI_BUFFER_TOO_SMALL;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  if (!Dev->Started) {
    return EFI_NOT_READY;
  }

  if (Dev->MessageHead == Dev->MessageTail) {
    Dwc3ProcessEvents (Dev);
  }

  if (!Dwc3PopMessage (Dev, &Pending)) {
    *Message     = EfiUsbMsgNone;
    *PayloadSize = 0;
    return EFI_SUCCESS;
  }

  *Message     = Pending.Message;
  *PayloadSize = sizeof (EFI_USBFN_MESSAGE_PAYLOAD);

  CopyMem (Payload, &Pending.Payload, sizeof (EFI_USBFN_MESSAGE_PAYLOAD));

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3Transfer (
  IN     EFI_USBFN_IO_PROTOCOL        *This,
  IN     UINT8                         EndpointIndex,
  IN     EFI_USBFN_ENDPOINT_DIRECTION  Direction,
  IN OUT UINTN                        *BufferSize,
  IN OUT VOID                         *Buffer)
{
  DWC3_DEV      *Dev;
  DWC3_ENDPOINT *Endpoint;
  EFI_STATUS     Status;
  UINT8          PhysEpNum;
  UINT32         TrbLength;
  UINT32         TrbType;
  VOID          *DmaBuffer;

  if (This == NULL || BufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*BufferSize != 0 && Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  Status = Dwc3ValidateEndpoint (Dev, EndpointIndex, Direction, &PhysEpNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Endpoint = &Dev->Endpoints[PhysEpNum];

  if (!Endpoint->Enabled) {
    DEBUG ((EFI_D_ERROR, "DWC3: Physical EP %u is not configured\n", PhysEpNum));
    return EFI_NOT_READY;
  }

  if (*BufferSize > DWC3_MAX_TRANSFER_SIZE) {
    return EFI_INVALID_PARAMETER;
  }

  if (Endpoint->TransferStarted && PhysEpNum > DWC3_EP0_IN) {
    DEBUG ((EFI_D_ERROR, "DWC3: Physical EP %u already has a transfer in progress\n", PhysEpNum));
    return EFI_NOT_READY;
  }

  TrbLength = (UINT32)*BufferSize;
  DmaBuffer = Buffer;

  Dev->Ep0Bounced = FALSE;

  if (PhysEpNum <= DWC3_EP0_IN) {
    if (Dev->Ep0State != Dwc3Ep0StateData) {
      return EFI_NOT_READY;
    }

    if ((PhysEpNum == DWC3_EP0_IN) != Dev->Ep0ExpectIn) {
      return EFI_INVALID_PARAMETER;
    }

    Dwc3EndTransfer (Dev, PhysEpNum);

    if (TrbLength > Dev->SetupPacket.Length) {
      TrbLength   = Dev->SetupPacket.Length;
      *BufferSize = TrbLength;
    }

    TrbType = DWC3_TRBCTL_CONTROL_DATA;

    if (Direction == EfiUsbEndpointDirectionDeviceRx) {
      if (TrbLength > DWC3_EP0_BOUNCE_SIZE) {
        return EFI_INVALID_PARAMETER;
      }

      Dev->Ep0Bounced      = TRUE;
      Dev->Ep0BounceTarget = Buffer;
      Dev->Ep0BounceLength = TrbLength;

      DmaBuffer = Dev->Ep0Buffer;
      TrbLength = (UINT32)ALIGN_VALUE (TrbLength, Endpoint->MaxPacketSize);

      if (TrbLength > DWC3_EP0_BOUNCE_SIZE) {
        TrbLength = DWC3_EP0_BOUNCE_SIZE;
      }
    }
  } else {
    TrbType = DWC3_TRBCTL_NORMAL;

    if (Direction == EfiUsbEndpointDirectionDeviceRx && Endpoint->MaxPacketSize != 0) {
      TrbLength = (UINT32)ALIGN_VALUE (TrbLength, Endpoint->MaxPacketSize);
    }
  }

  if (TrbLength != 0 && DmaBuffer == Buffer) {
    if (Direction == EfiUsbEndpointDirectionDeviceTx) {
      WriteBackDataCacheRange (Buffer, TrbLength);
    } else {
      WriteBackInvalidateDataCacheRange (Buffer, TrbLength);
    }
  }

  Endpoint->TransferBuffer    = Buffer;
  Endpoint->TransferTrbLength = TrbLength;

  Status = Dwc3StartTransfer (Dev, PhysEpNum,
                              (UINT64)(UINTN)DmaBuffer,
                              TrbLength, TrbType);
  if (EFI_ERROR (Status)) {
    Endpoint->TransferBuffer = NULL;
    Dev->Ep0Bounced          = FALSE;
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3GetMaxTransferSize (
  IN  EFI_USBFN_IO_PROTOCOL *This,
  OUT UINTN                 *MaxTransferSize)
{
  if (This == NULL || MaxTransferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *MaxTransferSize = DWC3_MAX_TRANSFER_SIZE;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3AllocateTransferBuffer (
  IN  EFI_USBFN_IO_PROTOCOL *This,
  IN  UINTN                  Size,
  OUT VOID                 **Buffer)
{
  DWC3_DEV *Dev;
  UINTN     Pages;
  UINT8     Index;

  if (This == NULL || Buffer == NULL || Size == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  for (Index = 0; Index < DWC3_MAX_TRANSFER_BUFFERS; Index++) {
    if (Dev->TransferBuffers[Index].Buffer == NULL) {
      break;
    }
  }

  if (Index == DWC3_MAX_TRANSFER_BUFFERS) {
    return EFI_OUT_OF_RESOURCES;
  }

  Pages   = EFI_SIZE_TO_PAGES (Size);
  *Buffer = AllocateAlignedPages (Pages, SIZE_4KB);

  if (*Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Dev->TransferBuffers[Index].Buffer = *Buffer;
  Dev->TransferBuffers[Index].Pages  = Pages;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3FreeTransferBuffer (
  IN EFI_USBFN_IO_PROTOCOL *This,
  IN VOID                  *Buffer)
{
  DWC3_DEV *Dev;
  UINT8     Index;

  if (This == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  for (Index = 0; Index < DWC3_MAX_TRANSFER_BUFFERS; Index++) {
    if (Dev->TransferBuffers[Index].Buffer == Buffer) {
      FreeAlignedPages (Buffer, Dev->TransferBuffers[Index].Pages);

      Dev->TransferBuffers[Index].Buffer = NULL;
      Dev->TransferBuffers[Index].Pages  = 0;

      return EFI_SUCCESS;
    }
  }

  return EFI_INVALID_PARAMETER;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3StartController (IN EFI_USBFN_IO_PROTOCOL *This)
{
  DWC3_DEV  *Dev;
  EFI_STATUS Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  if (Dev->Started) {
    return EFI_SUCCESS;
  }

  Status = UsbfnPlatformPhyInit ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DWC3: Failed to initialise the USB PHY! Status = %r\n", Status));
    return Status;
  }

  Status = Dwc3CoreInit (Dev);
  if (EFI_ERROR (Status)) {
    UsbfnPlatformPhyExit ();
    return Status;
  }

  Dev->Started          = TRUE;
  Dev->MessageHead      = 0;
  Dev->MessageTail      = 0;
  Dev->EventTraceBudget = DWC3_EVENT_TRACE_BUDGET;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3StopController (IN EFI_USBFN_IO_PROTOCOL *This)
{
  DWC3_DEV *Dev;
  UINT8     Index;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  if (!Dev->Started) {
    return EFI_SUCCESS;
  }

  Dwc3RunStop (Dev, FALSE);

  for (Index = 0; Index < Dev->NumPhysicalEps; Index++) {
    Dwc3DisableEndpoint (Dev, Index);
  }

  Dwc3CoreExit (Dev);
  UsbfnPlatformPhyExit ();

  Dev->Started     = FALSE;
  Dev->Speed       = UsbBusSpeedUnknown;
  Dev->MessageHead = 0;
  Dev->MessageTail = 0;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Dwc3SetEndpointPolicy (
  IN EFI_USBFN_IO_PROTOCOL        *This,
  IN UINT8                         EndpointIndex,
  IN EFI_USBFN_ENDPOINT_DIRECTION  Direction,
  IN EFI_USBFN_POLICY_TYPE         PolicyType,
  IN UINTN                         BufferSize,
  IN VOID                         *Buffer)
{
  DWC3_DEV  *Dev;
  EFI_STATUS Status;
  UINT8      PhysEpNum;

  if (This == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  Status = Dwc3ValidateEndpoint (Dev, EndpointIndex, Direction, &PhysEpNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  switch (PolicyType) {
    case EfiUsbPolicyZeroLengthTermination:
      if (BufferSize < sizeof (BOOLEAN)) {
        return EFI_INVALID_PARAMETER;
      }

      Dev->Endpoints[PhysEpNum].ZeroLengthTermination = *(BOOLEAN *)Buffer;
      return EFI_SUCCESS;

    case EfiUsbPolicyMaxTransactionSize:
    case EfiUsbPolicyZeroLengthTerminationSupport:
      return EFI_UNSUPPORTED;

    default:
      return EFI_INVALID_PARAMETER;
  }
}

STATIC
EFI_STATUS
EFIAPI
Dwc3GetEndpointPolicy (
  IN     EFI_USBFN_IO_PROTOCOL        *This,
  IN     UINT8                         EndpointIndex,
  IN     EFI_USBFN_ENDPOINT_DIRECTION  Direction,
  IN     EFI_USBFN_POLICY_TYPE         PolicyType,
  IN OUT UINTN                        *BufferSize,
  IN OUT VOID                         *Buffer)
{
  DWC3_DEV  *Dev;
  EFI_STATUS Status;
  UINT8      PhysEpNum;

  if (This == NULL || BufferSize == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = DWC3_DEV_FROM_PROTOCOL (This);

  Status = Dwc3ValidateEndpoint (Dev, EndpointIndex, Direction, &PhysEpNum);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  switch (PolicyType) {
    case EfiUsbPolicyMaxTransactionSize:
      if (*BufferSize < sizeof (UINTN)) {
        *BufferSize = sizeof (UINTN);
        return EFI_BUFFER_TOO_SMALL;
      }

      *(UINTN *)Buffer = DWC3_MAX_TRANSFER_SIZE;
      *BufferSize      = sizeof (UINTN);
      return EFI_SUCCESS;

    case EfiUsbPolicyZeroLengthTerminationSupport:
      if (*BufferSize < sizeof (BOOLEAN)) {
        *BufferSize = sizeof (BOOLEAN);
        return EFI_BUFFER_TOO_SMALL;
      }

      *(BOOLEAN *)Buffer = TRUE;
      *BufferSize        = sizeof (BOOLEAN);
      return EFI_SUCCESS;

    case EfiUsbPolicyZeroLengthTermination:
      if (*BufferSize < sizeof (BOOLEAN)) {
        *BufferSize = sizeof (BOOLEAN);
        return EFI_BUFFER_TOO_SMALL;
      }

      *(BOOLEAN *)Buffer = Dev->Endpoints[PhysEpNum].ZeroLengthTermination;
      *BufferSize        = sizeof (BOOLEAN);
      return EFI_SUCCESS;

    default:
      return EFI_INVALID_PARAMETER;
  }
}

STATIC
VOID
Dwc3FreeResources (IN DWC3_DEV *Dev)
{
  UINT8 Index;

  if (Dev == NULL) {
    return;
  }

  for (Index = 0; Index < DWC3_MAX_PHYSICAL_EP; Index++) {
    if (Index == DWC3_EP0_IN) {
      continue;
    }

    if (Dev->Endpoints[Index].TrbPool != NULL) {
      Dwc3FreeDmaBuffer (Dev->Endpoints[Index].TrbPool, sizeof (DWC3_TRB) * DWC3_TRB_PER_EP);
    }
  }

  if (Dev->Ep0Buffer != NULL) {
    Dwc3FreeDmaBuffer (Dev->Ep0Buffer, DWC3_EP0_BOUNCE_SIZE);
  }

  if (Dev->EventBuffer != NULL) {
    Dwc3FreeDmaBuffer (Dev->EventBuffer, DWC3_EVENT_BUFFER_SIZE);
  }

  FreePool (Dev);
}

EFI_STATUS
EFIAPI
InitUsbfnDwc3Driver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  DWC3_DEV  *Dev;
  UINT8      Index;

  Dev = AllocateZeroPool (sizeof (DWC3_DEV));
  if (Dev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = UsbfnPlatformGetData (&Dev->Platform);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DWC3: Unsupported platform! Status = %r\n", Status));
    FreePool (Dev);
    return Status;
  }

  Dev->Signature      = DWC3_DEV_SIGNATURE;
  Dev->Base           = Dev->Platform.ControllerBase;
  Dev->NumPhysicalEps = DWC3_MAX_PHYSICAL_EP;
  Dev->Speed          = UsbBusSpeedUnknown;

  Dev->EventBuffer = Dwc3AllocateDmaBuffer (DWC3_EVENT_BUFFER_SIZE, &Dev->EventBufferPhys);
  if (Dev->EventBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Failure;
  }

  Dev->Ep0Buffer = Dwc3AllocateDmaBuffer (DWC3_EP0_BOUNCE_SIZE, &Dev->Ep0BufferPhys);
  if (Dev->Ep0Buffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Failure;
  }

  for (Index = 0; Index < DWC3_MAX_PHYSICAL_EP; Index++) {
    Dev->Endpoints[Index].PhysEpNum = Index;

    if (Index == DWC3_EP0_IN) {
      Dev->Endpoints[Index].TrbPool     = Dev->Endpoints[DWC3_EP0_OUT].TrbPool;
      Dev->Endpoints[Index].TrbPoolPhys = Dev->Endpoints[DWC3_EP0_OUT].TrbPoolPhys;
      continue;
    }

    Dev->Endpoints[Index].TrbPool = Dwc3AllocateDmaBuffer (sizeof (DWC3_TRB) * DWC3_TRB_PER_EP,
                                                           &Dev->Endpoints[Index].TrbPoolPhys);
    if (Dev->Endpoints[Index].TrbPool == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Failure;
    }
  }

  Dev->UsbfnIo.Revision                 = EFI_USBFN_IO_PROTOCOL_REVISION;
  Dev->UsbfnIo.DetectPort               = Dwc3DetectPort;
  Dev->UsbfnIo.ConfigureEnableEndpoints = Dwc3ConfigureEnableEndpoints;
  Dev->UsbfnIo.GetEndpointMaxPacketSize = Dwc3GetEndpointMaxPacketSize;
  Dev->UsbfnIo.GetDeviceInfo            = Dwc3GetDeviceInfo;
  Dev->UsbfnIo.GetVendorIdProductId     = Dwc3GetVendorIdProductId;
  Dev->UsbfnIo.AbortTransfer            = Dwc3AbortTransfer;
  Dev->UsbfnIo.GetEndpointStallState    = Dwc3GetEndpointStallState;
  Dev->UsbfnIo.SetEndpointStallState    = Dwc3SetEndpointStallState;
  Dev->UsbfnIo.EventHandler             = Dwc3EventHandler;
  Dev->UsbfnIo.Transfer                 = Dwc3Transfer;
  Dev->UsbfnIo.GetMaxTransferSize       = Dwc3GetMaxTransferSize;
  Dev->UsbfnIo.AllocateTransferBuffer   = Dwc3AllocateTransferBuffer;
  Dev->UsbfnIo.FreeTransferBuffer       = Dwc3FreeTransferBuffer;
  Dev->UsbfnIo.StartController          = Dwc3StartController;
  Dev->UsbfnIo.StopController           = Dwc3StopController;
  Dev->UsbfnIo.SetEndpointPolicy        = Dwc3SetEndpointPolicy;
  Dev->UsbfnIo.GetEndpointPolicy        = Dwc3GetEndpointPolicy;

  Status = gBS->InstallMultipleProtocolInterfaces (&Dev->Handle,
                                                   &gEfiUsbFunctionIoProtocolGuid, &Dev->UsbfnIo,
                                                   NULL);
  Dwc3BuildSerialNumber();

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to install USB Function IO Protocol! Status = %r\n", Status));
    goto Failure;
  }

  return EFI_SUCCESS;

Failure:
  Dwc3FreeResources (Dev);

  return Status;
}
