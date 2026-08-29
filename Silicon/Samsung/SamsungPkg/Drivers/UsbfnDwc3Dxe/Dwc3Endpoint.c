#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <IndustryStandard/Usb.h>

#include "UsbfnDwc3Dxe.h"

STATIC
VOID
Dwc3PushMessage (
  IN DWC3_DEV                  *Dev,
  IN EFI_USBFN_MESSAGE          Message,
  IN EFI_USBFN_MESSAGE_PAYLOAD *Payload OPTIONAL)
{
  UINT32 Next = (Dev->MessageTail + 1) % DWC3_MESSAGE_QUEUE_DEPTH;

  if (Next == Dev->MessageHead) {
    DEBUG ((EFI_D_ERROR, "DWC3: Message Queue Overflow, dropping Message %u!\n", Message));
    return;
  }

  Dev->Messages[Dev->MessageTail].Message = Message;

  if (Payload != NULL) {
    CopyMem (&Dev->Messages[Dev->MessageTail].Payload, Payload, sizeof (EFI_USBFN_MESSAGE_PAYLOAD));
  } else {
    ZeroMem (&Dev->Messages[Dev->MessageTail].Payload, sizeof (EFI_USBFN_MESSAGE_PAYLOAD));
  }

  Dev->MessageTail = Next;
}

BOOLEAN
Dwc3PopMessage (
  IN  DWC3_DEV     *Dev,
  OUT DWC3_MESSAGE *Message)
{
  if (Dev->MessageHead == Dev->MessageTail) {
    return FALSE;
  }

  CopyMem (Message, &Dev->Messages[Dev->MessageHead], sizeof (DWC3_MESSAGE));

  Dev->MessageHead = (Dev->MessageHead + 1) % DWC3_MESSAGE_QUEUE_DEPTH;

  return TRUE;
}

STATIC
VOID
Dwc3PushTransferResult (
  IN DWC3_DEV                *Dev,
  IN UINT8                    PhysEpNum,
  IN UINTN                    BytesTransferred,
  IN EFI_USBFN_TRANSFER_STATUS Status)
{
  EFI_USBFN_MESSAGE_PAYLOAD Payload;
  DWC3_ENDPOINT            *Endpoint = &Dev->Endpoints[PhysEpNum];
  BOOLEAN                   IsIn     = (PhysEpNum & 1) != 0;

  ZeroMem (&Payload, sizeof (Payload));

  Payload.utr.BytesTransferred = BytesTransferred;
  Payload.utr.TransferStatus   = Status;
  Payload.utr.EndpointIndex    = (UINT8)(PhysEpNum >> 1);
  Payload.utr.Direction        = IsIn ? EfiUsbEndpointDirectionDeviceTx : EfiUsbEndpointDirectionDeviceRx;
  Payload.utr.Buffer           = Endpoint->TransferBuffer;

  Dwc3PushMessage (Dev, IsIn ? EfiUsbMsgEndpointStatusChangedTx : EfiUsbMsgEndpointStatusChangedRx, &Payload);
}

EFI_STATUS
Dwc3SendEpCommand (
  IN  DWC3_DEV *Dev,
  IN  UINT8     PhysEpNum,
  IN  UINT32    Command,
  IN  UINT32    Param0,
  IN  UINT32    Param1,
  IN  UINT32    Param2,
  OUT UINT32   *CommandRegister OPTIONAL)
{
  UINTN  DepBase = DWC3_DEP_BASE (PhysEpNum);
  UINT32 Register;
  UINT32 Timeout;
  UINT32 CommandStatus;

  MmioWrite32 (Dev->Base + DepBase + DWC3_DEPCMDPAR0, Param0);
  MmioWrite32 (Dev->Base + DepBase + DWC3_DEPCMDPAR1, Param1);
  MmioWrite32 (Dev->Base + DepBase + DWC3_DEPCMDPAR2, Param2);

  MmioWrite32 (Dev->Base + DepBase + DWC3_DEPCMD, Command | DWC3_DEPCMD_CMDACT);

  for (Timeout = 0; Timeout < DWC3_CMD_TIMEOUT_US; Timeout++) {
    Register = MmioRead32 (Dev->Base + DepBase + DWC3_DEPCMD);

    if ((Register & DWC3_DEPCMD_CMDACT) == 0) {
      CommandStatus = DWC3_DEPCMD_STATUS (Register);

      if (CommandRegister != NULL) {
        *CommandRegister = Register;
      }

      if (CommandStatus != 0) {
        DEBUG ((EFI_D_ERROR, "DWC3: Endpoint Command 0x%x on Physical EP %u failed with Status %u!\n", DWC3_DEPCMD_CMD (Command), PhysEpNum, CommandStatus));
        return EFI_DEVICE_ERROR;
      }

      return EFI_SUCCESS;
    }

    gBS->Stall (1);
  }

  DEBUG ((EFI_D_ERROR, "DWC3: Endpoint Command 0x%x on Physical EP %u timed out!\n", DWC3_DEPCMD_CMD (Command), PhysEpNum));

  return EFI_TIMEOUT;
}

STATIC
EFI_STATUS
Dwc3StartConfig (IN DWC3_DEV *Dev)
{
  EFI_STATUS Status;
  UINT8      Index;

  Status = Dwc3SendEpCommand (Dev, DWC3_EP0_OUT, DWC3_DEPCMD_DEPSTARTCFG, 0, 0, 0, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < Dev->NumPhysicalEps; Index++) {
    Status = Dwc3SendEpCommand (Dev, Index, DWC3_DEPCMD_SETTRANSFRESOURCE, DWC3_DEPXFERCFG_NUM_XFER_RES (1), 0, 0, NULL);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

STATIC
VOID
Dwc3InitTrbRing (IN DWC3_ENDPOINT *Endpoint)
{
  DWC3_TRB *Link;

  ZeroMem (Endpoint->TrbPool, sizeof (DWC3_TRB) * DWC3_TRB_PER_EP);

  Link = &Endpoint->TrbPool[DWC3_TRB_PER_EP - 1];

  Link->BufferPtrLow  = (UINT32)(Endpoint->TrbPoolPhys & 0xFFFFFFFF);
  Link->BufferPtrHigh = (UINT32)(Endpoint->TrbPoolPhys >> 32);
  Link->Size          = 0;
  Link->Control       = DWC3_TRBCTL_LINK_TRB | DWC3_TRB_CTRL_HWO;
}

EFI_STATUS
Dwc3EnableEndpoint (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN UINT8     EpType,
  IN UINT16    MaxPacketSize,
  IN BOOLEAN   Modify)
{
  DWC3_ENDPOINT *Endpoint = &Dev->Endpoints[PhysEpNum];
  EFI_STATUS     Status;
  UINT32         Param0;
  UINT32         Param1;
  UINT32         Register;

  if (PhysEpNum >= Dev->NumPhysicalEps) {
    return EFI_INVALID_PARAMETER;
  }

  if (PhysEpNum == DWC3_EP0_OUT && !Modify) {
    Status = Dwc3StartConfig (Dev);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Param0 = DWC3_DEPCFG_EP_TYPE (EpType) | DWC3_DEPCFG_MAX_PACKET_SIZE (MaxPacketSize);
  Param0 |= Modify ? DWC3_DEPCFG_ACTION_MODIFY : DWC3_DEPCFG_ACTION_INIT;

  if ((PhysEpNum & 1) != 0) {
    Param0 |= DWC3_DEPCFG_FIFO_NUMBER (PhysEpNum >> 1);
  }

  Param1 = DWC3_DEPCFG_EP_NUMBER (PhysEpNum);

  if (EpType == DWC3_DEPCMD_TYPE_CONTROL) {
    Param1 |= DWC3_DEPCFG_XFER_COMPLETE_EN | DWC3_DEPCFG_XFER_NOT_READY_EN;
  } else {
    Param1 |= DWC3_DEPCFG_XFER_COMPLETE_EN | DWC3_DEPCFG_XFER_IN_PROGRESS_EN;
  }

  Status = Dwc3SendEpCommand (Dev, PhysEpNum, DWC3_DEPCMD_SETEPCONFIG, Param0, Param1, 0, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Endpoint->PhysEpNum     = PhysEpNum;
  Endpoint->EpType        = EpType;
  Endpoint->MaxPacketSize = MaxPacketSize;

  if (Modify) {
    return EFI_SUCCESS;
  }

  Register  = MmioRead32 (Dev->Base + DWC3_DALEPENA);
  Register |= DWC3_DALEPENA_EP (PhysEpNum);
  MmioWrite32 (Dev->Base + DWC3_DALEPENA, Register);

  Endpoint->Enabled         = TRUE;
  Endpoint->Stalled         = FALSE;
  Endpoint->TransferStarted = FALSE;
  Endpoint->ResourceIndex   = 0;

  if (EpType != DWC3_DEPCMD_TYPE_CONTROL) {
    Dwc3InitTrbRing (Endpoint);
  }

  return EFI_SUCCESS;
}

VOID
Dwc3DisableEndpoint (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum)
{
  DWC3_ENDPOINT *Endpoint = &Dev->Endpoints[PhysEpNum];
  UINT32         Register;

  if (!Endpoint->Enabled) {
    return;
  }

  Dwc3EndTransfer (Dev, PhysEpNum);

  Register  = MmioRead32 (Dev->Base + DWC3_DALEPENA);
  Register &= ~DWC3_DALEPENA_EP (PhysEpNum);
  MmioWrite32 (Dev->Base + DWC3_DALEPENA, Register);

  Endpoint->Enabled         = FALSE;
  Endpoint->TransferStarted = FALSE;
  Endpoint->TransferBuffer  = NULL;
}

EFI_STATUS
Dwc3StartTransfer (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN UINT64    BufferPhys,
  IN UINT32    Length,
  IN UINT32    TrbType)
{
  DWC3_ENDPOINT *Endpoint = &Dev->Endpoints[PhysEpNum];
  DWC3_TRB      *Trb      = &Endpoint->TrbPool[0];
  DWC3_TRB      *Zlp      = &Endpoint->TrbPool[1];
  EFI_STATUS     Status   = EFI_DEVICE_ERROR;
  UINT32         CommandRegister;
  BOOLEAN        NeedZlp;
  UINT32         Attempt;

  NeedZlp = (BOOLEAN)(Endpoint->ZeroLengthTermination &&
                      (PhysEpNum & 1) != 0 &&
                      TrbType == DWC3_TRBCTL_NORMAL &&
                      Length != 0 &&
                      Endpoint->MaxPacketSize != 0 &&
                      (Length % Endpoint->MaxPacketSize) == 0);

  Trb->BufferPtrLow  = (UINT32)(BufferPhys & 0xFFFFFFFF);
  Trb->BufferPtrHigh = (UINT32)(BufferPhys >> 32);
  Trb->Size          = DWC3_TRB_SIZE_LENGTH (Length);
  Trb->Control       = TrbType | DWC3_TRB_CTRL_HWO | DWC3_TRB_CTRL_ISP_IMI;

  if (NeedZlp) {
    Trb->Control |= DWC3_TRB_CTRL_CHN;

    Zlp->BufferPtrLow  = (UINT32)(BufferPhys & 0xFFFFFFFF);
    Zlp->BufferPtrHigh = (UINT32)(BufferPhys >> 32);
    Zlp->Size          = 0;
    Zlp->Control       = TrbType | DWC3_TRB_CTRL_HWO | DWC3_TRB_CTRL_ISP_IMI |
                         DWC3_TRB_CTRL_IOC | DWC3_TRB_CTRL_LST;
  } else {
    Trb->Control |= DWC3_TRB_CTRL_IOC | DWC3_TRB_CTRL_LST;
  }

  for (Attempt = 0; Attempt < DWC3_START_XFER_ATTEMPTS; Attempt++) {
    if (Attempt != 0) {
      gBS->Stall (200);

      Dwc3EndTransfer (Dev, PhysEpNum);

      Trb->BufferPtrLow  = (UINT32)(BufferPhys & 0xFFFFFFFF);
      Trb->BufferPtrHigh = (UINT32)(BufferPhys >> 32);
      Trb->Size          = DWC3_TRB_SIZE_LENGTH (Length);
      Trb->Control       = TrbType | DWC3_TRB_CTRL_HWO | DWC3_TRB_CTRL_ISP_IMI;

      if (NeedZlp) {
        Trb->Control |= DWC3_TRB_CTRL_CHN;

        Zlp->BufferPtrLow  = (UINT32)(BufferPhys & 0xFFFFFFFF);
        Zlp->BufferPtrHigh = (UINT32)(BufferPhys >> 32);
        Zlp->Size          = 0;
        Zlp->Control       = TrbType | DWC3_TRB_CTRL_HWO | DWC3_TRB_CTRL_ISP_IMI |
                             DWC3_TRB_CTRL_IOC | DWC3_TRB_CTRL_LST;
      } else {
        Trb->Control |= DWC3_TRB_CTRL_IOC | DWC3_TRB_CTRL_LST;
      }
    }

    Status = Dwc3SendEpCommand (Dev, PhysEpNum, DWC3_DEPCMD_STARTTRANSFER,
                                (UINT32)(Endpoint->TrbPoolPhys >> 32),
                                (UINT32)(Endpoint->TrbPoolPhys & 0xFFFFFFFF),
                                0, &CommandRegister);
    if (!EFI_ERROR (Status)) {
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DWC3: STARTTRANSFER failed on Physical EP %u, TrbType 0x%x Len %u RscIdx %u Ep0State %u\n",
            PhysEpNum, TrbType, Length, Endpoint->ResourceIndex, Dev->Ep0State));
    Trb->Control = 0;
    return Status;
  }

  Endpoint->ResourceIndex   = (UINT8)DWC3_DEPCMD_GET_RSC_IDX (CommandRegister);
  Endpoint->TransferStarted = TRUE;

  return EFI_SUCCESS;
}

VOID
Dwc3EndTransfer (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum)
{
  DWC3_ENDPOINT *Endpoint = &Dev->Endpoints[PhysEpNum];
  UINT32         Command;

  if (Endpoint->ResourceIndex == 0) {
    Endpoint->TransferStarted = FALSE;
    return;
  }

  Command = DWC3_DEPCMD_ENDTRANSFER | DWC3_DEPCMD_HIPRI_FORCERM | DWC3_DEPCMD_CMDIOC;
  Command |= DWC3_DEPCMD_PARAM (Endpoint->ResourceIndex);

  Dwc3SendEpCommand (Dev, PhysEpNum, Command, 0, 0, 0, NULL);

  gBS->Stall (100);

  Endpoint->ResourceIndex      = 0;
  Endpoint->TransferStarted    = FALSE;
  Endpoint->TrbPool[0].Control = 0;
  Endpoint->TrbPool[1].Control = 0;
}

EFI_STATUS
Dwc3SetHalt (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN BOOLEAN   Stall)
{
  DWC3_ENDPOINT *Endpoint = &Dev->Endpoints[PhysEpNum];
  EFI_STATUS     Status;

  if (Stall) {
    Status = Dwc3SendEpCommand (Dev, PhysEpNum, DWC3_DEPCMD_SETSTALL, 0, 0, 0, NULL);
  } else {
    Status = Dwc3SendEpCommand (Dev, PhysEpNum, DWC3_DEPCMD_CLEARSTALL, 0, 0, 0, NULL);
  }

  if (!EFI_ERROR (Status)) {
    Endpoint->Stalled = Stall;
  }

  return Status;
}

VOID
Dwc3Ep0OutStart (IN DWC3_DEV *Dev)
{
  Dev->Ep0State         = Dwc3Ep0StateSetup;
  Dev->Ep0StatusStarted = FALSE;
  Dev->Ep0Bounced       = FALSE;

  Dwc3EndTransfer (Dev, DWC3_EP0_IN);
  Dwc3EndTransfer (Dev, DWC3_EP0_OUT);
  Dwc3StartTransfer (Dev, DWC3_EP0_OUT, Dev->Ep0BufferPhys, 8, DWC3_TRBCTL_CONTROL_SETUP);
}

VOID
Dwc3Ep0ResetState (IN DWC3_DEV *Dev)
{
  Dev->Ep0State         = Dwc3Ep0StateSetup;
  Dev->Ep0StatusStarted = FALSE;
  Dev->Ep0Bounced       = FALSE;

  Dwc3EndTransfer (Dev, DWC3_EP0_IN);
}

VOID
Dwc3Ep0StallAndRestart (IN DWC3_DEV *Dev)
{
  Dwc3SetHalt (Dev, DWC3_EP0_OUT, TRUE);

  Dev->Endpoints[DWC3_EP0_OUT].Stalled = FALSE;
  Dev->Endpoints[DWC3_EP0_IN].Stalled  = FALSE;

  Dwc3Ep0OutStart (Dev);
}

STATIC
VOID
Dwc3ResetNonControlEndpoints (IN DWC3_DEV *Dev)
{
  DWC3_ENDPOINT *Endpoint;
  UINT8          Index;

  for (Index = DWC3_EP0_IN + 1; Index < Dev->NumPhysicalEps; Index++) {
    Endpoint = &Dev->Endpoints[Index];

    if (!Endpoint->Enabled) {
      continue;
    }

    if (Endpoint->TransferStarted) {
      Dwc3EndTransfer (Dev, Index);
      Dwc3PushTransferResult (Dev, Index, 0, UsbTransferStatusAborted);
      Endpoint->TransferBuffer = NULL;
    }

    Dwc3EnableEndpoint (Dev, Index, Endpoint->EpType, Endpoint->MaxPacketSize, FALSE);
  }
}

STATIC
BOOLEAN
Dwc3Ep0HandleStandardRequest (IN DWC3_DEV *Dev)
{
  EFI_USB_DEVICE_REQUEST *Request = &Dev->SetupPacket;
  UINT32                  Register;
  UINT8                   Address;

  if ((Request->RequestType & (0x03 << 5)) != USB_REQ_TYPE_STANDARD) {
    return FALSE;
  }

  switch (Request->Request) {
    case USB_REQ_SET_ADDRESS:
      Address = (UINT8)(Request->Value & 0x7F);

      Register  = MmioRead32 (Dev->Base + DWC3_DCFG);
      Register &= ~DWC3_DCFG_DEVADDR_MASK;
      Register |= DWC3_DCFG_DEVADDR (Address);
      MmioWrite32 (Dev->Base + DWC3_DCFG, Register);

      return TRUE;

    case USB_REQ_SET_CONFIG:
    case USB_REQ_SET_INTERFACE:
      Dwc3ResetNonControlEndpoints (Dev);

      return FALSE;

    default:
      return FALSE;
  }
}

STATIC
VOID
Dwc3Ep0HandleSetup (IN DWC3_DEV *Dev)
{
  EFI_USBFN_MESSAGE_PAYLOAD Payload;

  CopyMem (&Dev->SetupPacket, Dev->Ep0Buffer, sizeof (EFI_USB_DEVICE_REQUEST));

  Dev->ThreeStageSetup = (BOOLEAN)(Dev->SetupPacket.Length != 0);
  Dev->Ep0ExpectIn     = (BOOLEAN)((Dev->SetupPacket.RequestType & USB_ENDPOINT_DIR_IN) != 0);

  if (Dwc3Ep0HandleStandardRequest (Dev)) {
    Dev->Ep0State = Dwc3Ep0StateStatus;
    return;
  }

  Dev->Ep0State = Dev->ThreeStageSetup ? Dwc3Ep0StateData : Dwc3Ep0StateStatus;

  ZeroMem (&Payload, sizeof (Payload));
  CopyMem (&Payload.udr, &Dev->SetupPacket, sizeof (EFI_USB_DEVICE_REQUEST));

  Dwc3PushMessage (Dev, EfiUsbMsgSetupPacket, &Payload);
}

STATIC
VOID
Dwc3Ep0StartStatus (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum)
{
  UINT32 TrbType = Dev->ThreeStageSetup ? DWC3_TRBCTL_CONTROL_STATUS3 : DWC3_TRBCTL_CONTROL_STATUS2;

  if (Dev->Ep0StatusStarted) {
    return;
  }

  if (Dev->Ep0State == Dwc3Ep0StateSetup) {
    return;
  }

  Dev->Ep0State         = Dwc3Ep0StateStatus;
  Dev->Ep0StatusStarted = TRUE;

  Dwc3EndTransfer (Dev, PhysEpNum);
  Dwc3StartTransfer (Dev, PhysEpNum, Dev->Ep0BufferPhys, 0, TrbType);
}

STATIC
UINTN
Dwc3TrbBytesTransferred (IN DWC3_ENDPOINT *Endpoint)
{
  UINT32 Remaining = DWC3_TRB_SIZE_LENGTH (Endpoint->TrbPool[0].Size);

  if (Remaining > Endpoint->TransferTrbLength) {
    return 0;
  }

  return Endpoint->TransferTrbLength - Remaining;
}

STATIC
VOID
Dwc3HandleEp0Event (
  IN DWC3_DEV *Dev,
  IN UINT8     PhysEpNum,
  IN UINT8     EventType,
  IN UINT8     EventStatus)
{
  DWC3_ENDPOINT *Endpoint = &Dev->Endpoints[PhysEpNum];
  UINTN          Transferred;

  switch (EventType) {
    case DWC3_DEPEVT_XFERCOMPLETE:
      Endpoint->TransferStarted = FALSE;

      switch (Dev->Ep0State) {
        case Dwc3Ep0StateSetup:
          Dwc3Ep0HandleSetup (Dev);
          break;

        case Dwc3Ep0StateData:
          Transferred = Dwc3TrbBytesTransferred (Endpoint);

          if (Dev->Ep0Bounced) {
            if (Transferred > Dev->Ep0BounceLength) {
              Transferred = Dev->Ep0BounceLength;
            }

            if (Dev->Ep0BounceTarget != NULL && Transferred != 0) {
              CopyMem (Dev->Ep0BounceTarget, Dev->Ep0Buffer, Transferred);
            }

            Dev->Ep0Bounced = FALSE;
          }

          Dwc3PushTransferResult (Dev, PhysEpNum, Transferred, UsbTransferStatusComplete);
          Endpoint->TransferBuffer = NULL;
          break;

        case Dwc3Ep0StateStatus:
          Dwc3Ep0OutStart (Dev);
          break;

        default:
          break;
      }

      break;

    case DWC3_DEPEVT_XFERNOTREADY:
      switch (DEPEVT_STATUS_CONTROL_PHASE (EventStatus)) {
        case DEPEVT_STATUS_CONTROL_DATA:
          if ((PhysEpNum == DWC3_EP0_IN) != Dev->Ep0ExpectIn) {
            DEBUG ((EFI_D_ERROR, "DWC3: Unexpected Direction for the EP0 Data Stage!\n"));
            Dwc3EndTransfer (Dev, Dev->Ep0ExpectIn ? DWC3_EP0_IN : DWC3_EP0_OUT);
            Dwc3Ep0StallAndRestart (Dev);
          }

          break;

        case DEPEVT_STATUS_CONTROL_STATUS:
          Dwc3Ep0StartStatus (Dev, PhysEpNum);
          break;

        default:
          break;
      }

      break;

    default:
      break;
  }
}

STATIC
VOID
Dwc3HandleEpEvent (
  IN DWC3_DEV *Dev,
  IN UINT32    Event)
{
  UINT8          PhysEpNum   = (UINT8)DWC3_DEPEVT_EPNUMBER (Event);
  UINT8          EventType   = (UINT8)DWC3_DEPEVT_TYPE (Event);
  UINT8          EventStatus = (UINT8)DWC3_DEPEVT_STATUS (Event);
  DWC3_ENDPOINT *Endpoint;
  UINTN          Transferred;

  if (PhysEpNum >= DWC3_MAX_PHYSICAL_EP) {
    return;
  }

  Endpoint = &Dev->Endpoints[PhysEpNum];

  if (PhysEpNum <= DWC3_EP0_IN) {
    Dwc3HandleEp0Event (Dev, PhysEpNum, EventType, EventStatus);
    return;
  }

  switch (EventType) {
    case DWC3_DEPEVT_XFERCOMPLETE:
    case DWC3_DEPEVT_XFERINPROGRESS:
      if (!Endpoint->TransferStarted) {
        break;
      }

      if ((Endpoint->TrbPool[0].Control & DWC3_TRB_CTRL_HWO) != 0) {
        break;
      }

      Endpoint->TransferStarted = FALSE;
      Endpoint->ResourceIndex   = 0;

      Transferred = Dwc3TrbBytesTransferred (Endpoint);

      if ((PhysEpNum & 1) == 0 && Endpoint->TransferBuffer != NULL && Endpoint->TransferTrbLength != 0) {
        InvalidateDataCacheRange (Endpoint->TransferBuffer, Endpoint->TransferTrbLength);
      }

      Dwc3PushTransferResult (Dev, PhysEpNum, Transferred,
                              (EventStatus & DEPEVT_STATUS_BUSERR) ? UsbTransferStatusAborted : UsbTransferStatusComplete);

      Endpoint->TransferBuffer = NULL;
      break;

    case DWC3_DEPEVT_XFERNOTREADY:
      break;

    default:
      break;
  }
}

STATIC
VOID
Dwc3HandleDeviceEvent (
  IN DWC3_DEV *Dev,
  IN UINT32    Event)
{
  UINT8                     EventType = (UINT8)DWC3_DEVT_TYPE (Event);
  UINT16                    EventInfo = (UINT16)DWC3_DEVT_INFO (Event);
  EFI_USBFN_MESSAGE_PAYLOAD Payload;
  UINT32                    Register;
  UINT16                    Ep0MaxPacket;
  UINT16                    BulkMaxPacket;
  UINT8                     Index;
  BOOLEAN                   Suspended;
  BOOLEAN                   Ep0Rearm;

  ZeroMem (&Payload, sizeof (Payload));

  switch (EventType) {
    case DWC3_DEVICE_EVENT_DISCONNECT:
      Dev->Speed = UsbBusSpeedUnknown;

      Dwc3PushMessage (Dev, EfiUsbMsgBusEventDetach, NULL);
      break;

    case DWC3_DEVICE_EVENT_RESET:
      Register  = MmioRead32 (Dev->Base + DWC3_DCTL);
      Register &= ~DWC3_DCTL_TSTCTRL_MASK;
      MmioWrite32 (Dev->Base + DWC3_DCTL, Register);

      for (Index = 0; Index < Dev->NumPhysicalEps; Index++) {
        if (Dev->Endpoints[Index].Enabled && Dev->Endpoints[Index].Stalled) {
          Dwc3SetHalt (Dev, Index, FALSE);
        }
      }

      Register  = MmioRead32 (Dev->Base + DWC3_DCFG);
      Register &= ~DWC3_DCFG_DEVADDR_MASK;
      MmioWrite32 (Dev->Base + DWC3_DCFG, Register);

      for (Index = DWC3_EP0_IN + 1; Index < Dev->NumPhysicalEps; Index++) {
        if (Dev->Endpoints[Index].Enabled) {
          Dwc3EndTransfer (Dev, Index);
          Dev->Endpoints[Index].TransferBuffer = NULL;
        }
      }

      Dev->LinkState = DWC3_LINK_STATE_HS_ON;
      Dev->Suspended = FALSE;

      Dwc3Ep0ResetState (Dev);

      Dwc3PushMessage (Dev, EfiUsbMsgBusEventReset, NULL);
      break;

    case DWC3_DEVICE_EVENT_CONNECT_DONE:
      Register = MmioRead32 (Dev->Base + DWC3_DSTS) & DWC3_DSTS_CONNECTSPD;

      switch (Register) {
        case DWC3_DSTS_SUPERSPEED_PLUS:
        case DWC3_DSTS_SUPERSPEED:
          Dev->Speed   = UsbBusSpeedSuper;
          Ep0MaxPacket = 512;
          break;
        case DWC3_DSTS_HIGHSPEED:
          Dev->Speed   = UsbBusSpeedHigh;
          Ep0MaxPacket = 64;
          break;
        case DWC3_DSTS_FULLSPEED:
          Dev->Speed   = UsbBusSpeedFull;
          Ep0MaxPacket = 64;
          break;
        default:
          Dev->Speed   = UsbBusSpeedLow;
          Ep0MaxPacket = 8;
          break;
      }

      Ep0Rearm = (BOOLEAN)(!Dev->Endpoints[DWC3_EP0_OUT].TransferStarted);

      Dwc3EnableEndpoint (Dev, DWC3_EP0_OUT, DWC3_DEPCMD_TYPE_CONTROL, Ep0MaxPacket, (BOOLEAN)!Ep0Rearm);
      Dwc3EnableEndpoint (Dev, DWC3_EP0_IN, DWC3_DEPCMD_TYPE_CONTROL, Ep0MaxPacket, (BOOLEAN)!Ep0Rearm);

      BulkMaxPacket = (UINT16)((Dev->Speed >= UsbBusSpeedHigh) ? 512 : 64);

      for (Index = DWC3_EP0_IN + 1; Index < Dev->NumPhysicalEps; Index++) {
        if (!Dev->Endpoints[Index].Enabled) {
          continue;
        }

        Dwc3EnableEndpoint (Dev, Index, Dev->Endpoints[Index].EpType, BulkMaxPacket, FALSE);
      }

      if (Ep0Rearm) {
        Dwc3Ep0OutStart (Dev);
      }

      Payload.ubs = Dev->Speed;
      Dwc3PushMessage (Dev, EfiUsbMsgBusEventSpeed, &Payload);
      break;

    case DWC3_DEVICE_EVENT_LINK_STATUS_CHANGE:
      EventInfo &= DWC3_LINK_STATE_MASK;

      if (EventInfo == Dev->LinkState) {
        break;
      }

      Dev->LinkState = (UINT8)EventInfo;

      if (Dev->Speed == UsbBusSpeedSuper) {
        Suspended = (BOOLEAN)(EventInfo == DWC3_LINK_STATE_U3);
      } else {
        Suspended = (BOOLEAN)(EventInfo == DWC3_LINK_STATE_HS_SUSPEND ||
                              EventInfo == DWC3_LINK_STATE_HS_EARLY_SUSPEND);
      }

      if (Suspended != Dev->Suspended) {
        Dev->Suspended = Suspended;
        Dwc3PushMessage (Dev, Suspended ? EfiUsbMsgBusEventSuspend : EfiUsbMsgBusEventResume, NULL);
      }

      break;

    case DWC3_DEVICE_EVENT_WAKEUP:
      Dwc3PushMessage (Dev, EfiUsbMsgBusEventResume, NULL);
      break;

    case DWC3_DEVICE_EVENT_OVERFLOW:
      DEBUG ((EFI_D_ERROR, "DWC3: Event Buffer Overflow!\n"));
      break;

    default:
      break;
  }
}

EFI_STATUS
Dwc3ProcessEvents (IN DWC3_DEV *Dev)
{
  UINT32 Count;
  UINT32 Event;

  Count = MmioRead32 (Dev->Base + DWC3_GEVNTCOUNT (0)) & DWC3_GEVNTCOUNT_MASK;
  if (Count == 0) {
    return EFI_NOT_READY;
  }

  while (Count >= sizeof (UINT32)) {
    CopyMem (&Event, Dev->EventBuffer + Dev->EventPos, sizeof (UINT32));

    Dev->EventPos = (Dev->EventPos + sizeof (UINT32)) % DWC3_EVENT_BUFFER_SIZE;
    Count        -= sizeof (UINT32);

    MmioWrite32 (Dev->Base + DWC3_GEVNTCOUNT (0), sizeof (UINT32));

    if (Dev->EventTraceBudget != 0) {
      Dev->EventTraceBudget--;
    }

    if (DWC3_EVENT_IS_ENDPOINT (Event)) {
      Dwc3HandleEpEvent (Dev, Event);
    } else if (DWC3_DEVT_CATEGORY (Event) == DWC3_EVENT_TYPE_DEV) {
      Dwc3HandleDeviceEvent (Dev, Event);
    } else {
      DEBUG ((EFI_D_ERROR, "DWC3: Unknown Event 0x%08x\n", Event));
    }

    if (Dev->MessageHead != Dev->MessageTail) {
      break;
    }
  }

  return EFI_SUCCESS;
}
