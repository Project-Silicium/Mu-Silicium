#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "UsbfnDwc3Dxe.h"

VOID *
Dwc3AllocateDmaBuffer (
  IN  UINTN                 Size,
  OUT EFI_PHYSICAL_ADDRESS *PhysicalAddress)
{
  EFI_STATUS Status;
  VOID      *Buffer;
  UINTN      Pages = EFI_SIZE_TO_PAGES (Size);

  Buffer = AllocateAlignedPages (Pages, SIZE_4KB);
  if (Buffer == NULL) {
    return NULL;
  }

  ZeroMem (Buffer, EFI_PAGES_TO_SIZE (Pages));

  Status = gDS->SetMemorySpaceAttributes ((EFI_PHYSICAL_ADDRESS)(UINTN)Buffer, EFI_PAGES_TO_SIZE (Pages), EFI_MEMORY_UC);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DWC3: Failed to set memory attribs! Status = %r\n", Status));
    FreeAlignedPages (Buffer, Pages);
    return NULL;
  }

  if (PhysicalAddress != NULL) {
    *PhysicalAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)Buffer;
  }

  return Buffer;
}

VOID
Dwc3FreeDmaBuffer (
  IN VOID  *Buffer,
  IN UINTN  Size)
{
  UINTN Pages = EFI_SIZE_TO_PAGES (Size);

  if (Buffer == NULL) {
    return;
  }

  gDS->SetMemorySpaceAttributes ((EFI_PHYSICAL_ADDRESS)(UINTN)Buffer, EFI_PAGES_TO_SIZE (Pages), EFI_MEMORY_WB);

  FreeAlignedPages (Buffer, Pages);
}

STATIC
EFI_STATUS
Dwc3SoftReset (IN DWC3_DEV *Dev)
{
  UINT32 Register;
  UINT32 Timeout;

  if (!Dev->Platform.SuperSpeedPhyDisabled) {
    Register  = MmioRead32 (Dev->Base + DWC3_GUSB3PIPECTL (0));
    Register |= DWC3_GUSB3PIPECTL_PHYSOFTRST;
    MmioWrite32 (Dev->Base + DWC3_GUSB3PIPECTL (0), Register);
  }

  Register  = MmioRead32 (Dev->Base + DWC3_GUSB2PHYCFG (0));
  Register |= DWC3_GUSB2PHYCFG_PHYSOFTRST;
  MmioWrite32 (Dev->Base + DWC3_GUSB2PHYCFG (0), Register);

  gBS->Stall (100);

  if (!Dev->Platform.SuperSpeedPhyDisabled) {
    Register  = MmioRead32 (Dev->Base + DWC3_GUSB3PIPECTL (0));
    Register &= ~DWC3_GUSB3PIPECTL_PHYSOFTRST;
    MmioWrite32 (Dev->Base + DWC3_GUSB3PIPECTL (0), Register);
  }

  Register  = MmioRead32 (Dev->Base + DWC3_GUSB2PHYCFG (0));
  Register &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;
  MmioWrite32 (Dev->Base + DWC3_GUSB2PHYCFG (0), Register);

  gBS->Stall (100);

  Register  = MmioRead32 (Dev->Base + DWC3_DCTL);
  Register |= DWC3_DCTL_CSFTRST;
  MmioWrite32 (Dev->Base + DWC3_DCTL, Register);

  for (Timeout = 0; Timeout < DWC3_RESET_TIMEOUT_US; Timeout++) {
    if ((MmioRead32 (Dev->Base + DWC3_DCTL) & DWC3_DCTL_CSFTRST) == 0) {
      if (Dev->IsUsb31) {
        gBS->Stall (50000);
      }

      return EFI_SUCCESS;
    }

    gBS->Stall (1);
  }

  DEBUG ((EFI_D_ERROR, "DWC3: Core Soft Reset timed out!\n"));

  return EFI_DEVICE_ERROR;
}

STATIC
VOID
Dwc3PhyInterfaceSetup (IN DWC3_DEV *Dev)
{
  UINT32 Register;

  Register = MmioRead32 (Dev->Base + DWC3_GUSB3PIPECTL (0));

  if (Dev->Platform.SuperSpeedPhyDisabled) {
    Register |= DWC3_GUSB3PIPECTL_SUSPHY;
  } else {
    Register &= ~DWC3_GUSB3PIPECTL_SUSPHY;
    Register |= DWC3_GUSB3PIPECTL_DEP1P2P3_EN;
  }

  MmioWrite32 (Dev->Base + DWC3_GUSB3PIPECTL (0), Register);

  Register  = MmioRead32 (Dev->Base + DWC3_GUSB2PHYCFG (0));
  Register &= ~(DWC3_GUSB2PHYCFG_PHYIF_MASK | DWC3_GUSB2PHYCFG_USBTRDTIM_MASK);
  Register &= ~DWC3_GUSB2PHYCFG_ULPI_UTMI;

  if (Dev->Platform.UtmiWidth == UsbfnUtmiWidth16Bit) {
    Register |= DWC3_GUSB2PHYCFG_PHYIF (UTMI_PHYIF_16_BIT);
    Register |= DWC3_GUSB2PHYCFG_USBTRDTIM (USBTRDTIM_UTMI_16_BIT);
  } else {
    Register |= DWC3_GUSB2PHYCFG_PHYIF (UTMI_PHYIF_8_BIT);
    Register |= DWC3_GUSB2PHYCFG_USBTRDTIM (USBTRDTIM_UTMI_8_BIT);
  }

  Register &= ~(DWC3_GUSB2PHYCFG_SUSPHY | DWC3_GUSB2PHYCFG_ENBLSLPM);

  if (!Dev->Platform.Usb2FreeClockExists) {
    Register &= ~DWC3_GUSB2PHYCFG_U2_FREECLK_EXISTS;
  }

  MmioWrite32 (Dev->Base + DWC3_GUSB2PHYCFG (0), Register);
}

STATIC
VOID
Dwc3FrameLengthAdjust (IN DWC3_DEV *Dev)
{
  UINT32 Register;

  if (Dev->Platform.FrameLengthAdjustMask == 0) {
    return;
  }

  Register  = MmioRead32 (Dev->Base + DWC3_GFLADJ);
  Register &= ~Dev->Platform.FrameLengthAdjustMask;
  Register |= Dev->Platform.FrameLengthAdjustValue;
  MmioWrite32 (Dev->Base + DWC3_GFLADJ, Register);
}

VOID
Dwc3EventBufferSetup (IN DWC3_DEV *Dev)
{
  Dev->EventPos = 0;

  MmioWrite32 (Dev->Base + DWC3_GEVNTADRLO (0), (UINT32)(Dev->EventBufferPhys & 0xFFFFFFFF));
  MmioWrite32 (Dev->Base + DWC3_GEVNTADRHI (0), (UINT32)(Dev->EventBufferPhys >> 32));
  MmioWrite32 (Dev->Base + DWC3_GEVNTSIZ (0), DWC3_GEVNTSIZ_INTMASK | DWC3_GEVNTSIZ_SIZE (DWC3_EVENT_BUFFER_SIZE));
  MmioWrite32 (Dev->Base + DWC3_GEVNTCOUNT (0), MmioRead32 (Dev->Base + DWC3_GEVNTCOUNT (0)) & DWC3_GEVNTCOUNT_MASK);
}

VOID
Dwc3EventBufferCleanup (IN DWC3_DEV *Dev)
{
  MmioWrite32 (Dev->Base + DWC3_GEVNTADRLO (0), 0);
  MmioWrite32 (Dev->Base + DWC3_GEVNTADRHI (0), 0);
  MmioWrite32 (Dev->Base + DWC3_GEVNTSIZ (0), DWC3_GEVNTSIZ_INTMASK);
  MmioWrite32 (Dev->Base + DWC3_GEVNTCOUNT (0), 0);
}

EFI_STATUS
Dwc3RunStop (
  IN DWC3_DEV *Dev,
  IN BOOLEAN   Run)
{
  UINT32 Register;
  UINT32 Timeout;

  Register = MmioRead32 (Dev->Base + DWC3_DCTL);

  if (Run) {
    Register |= DWC3_DCTL_RUN_STOP;
  } else {
    Register &= ~DWC3_DCTL_RUN_STOP;
  }

  Register &= ~DWC3_DCTL_TSTCTRL_MASK;

  MmioWrite32 (Dev->Base + DWC3_DCTL, Register);

  for (Timeout = 0; Timeout < DWC3_RUNSTOP_TIMEOUT_US; Timeout++) {
    Register = MmioRead32 (Dev->Base + DWC3_DSTS);

    if (Run) {
      if ((Register & DWC3_DSTS_DEVCTRLHLT) == 0) {
        return EFI_SUCCESS;
      }
    } else {
      if ((Register & DWC3_DSTS_DEVCTRLHLT) != 0) {
        return EFI_SUCCESS;
      }
    }

    gBS->Stall (1);
  }

  DEBUG ((EFI_D_ERROR, "DWC3: %a timed out! DSTS = 0x%08x\n", Run ? "Run" : "Stop", Register));

  return EFI_DEVICE_ERROR;
}

EFI_STATUS
Dwc3CoreInit (IN DWC3_DEV *Dev)
{
  EFI_STATUS Status;
  UINT32     Register;
  UINT32     HwParams3;
  UINT8      Index;

  Register = MmioRead32 (Dev->Base + DWC3_GSNPSID);
  if ((Register & DWC3_GSNPSID_MASK) != DWC3_GSNPSID_USB3 &&
      (Register & DWC3_GSNPSID_MASK) != DWC3_GSNPSID_USB31)
  {
    DEBUG ((EFI_D_ERROR, "DWC3: Bad Synopsys ID 0x%08x!\n", Register));
    ASSERT (FALSE);
  }

  Dev->IsUsb31 = (BOOLEAN)((Register & DWC3_GSNPSID_MASK) == DWC3_GSNPSID_USB31);

  Register  = MmioRead32 (Dev->Base + DWC3_GCTL);
  Register &= ~DWC3_GCTL_PRTCAPDIR (DWC3_GCTL_PRTCAP_OTG);
  Register |= DWC3_GCTL_PRTCAPDIR (DWC3_GCTL_PRTCAP_DEVICE);
  MmioWrite32 (Dev->Base + DWC3_GCTL, Register);

  gBS->Stall (100);

  Status = Dwc3SoftReset (Dev);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Dwc3PhyInterfaceSetup (Dev);

  Register  = MmioRead32 (Dev->Base + DWC3_GCTL);
  Register &= ~DWC3_GCTL_SCALEDOWN_MASK;
  Register &= ~DWC3_GCTL_DISSCRAMBLE;
  Register &= ~DWC3_GCTL_GBLHIBERNATIONEN;
  Register &= ~DWC3_GCTL_DSBLCLKGTNG;
  MmioWrite32 (Dev->Base + DWC3_GCTL, Register);

  Dwc3FrameLengthAdjust (Dev);

  if (Dev->Platform.NumEndpoints != 0) {
    Dev->NumPhysicalEps = Dev->Platform.NumEndpoints;
  } else {
    HwParams3           = MmioRead32 (Dev->Base + DWC3_GHWPARAMS3);
    Dev->NumPhysicalEps = (UINT8)DWC3_GHWPARAMS3_NUM_EPS (HwParams3);
  }

  if (Dev->NumPhysicalEps == 0 || Dev->NumPhysicalEps > DWC3_MAX_PHYSICAL_EP) {
    Dev->NumPhysicalEps = DWC3_MAX_PHYSICAL_EP;
  }

  Dwc3EventBufferSetup (Dev);

  Register  = MmioRead32 (Dev->Base + DWC3_GCTL);
  Register &= ~DWC3_GCTL_PRTCAPDIR (DWC3_GCTL_PRTCAP_OTG);
  Register |= DWC3_GCTL_PRTCAPDIR (DWC3_GCTL_PRTCAP_DEVICE);
  MmioWrite32 (Dev->Base + DWC3_GCTL, Register);

  gBS->Stall (100);

  Register  = MmioRead32 (Dev->Base + DWC3_DCFG);
  Register &= ~DWC3_DCFG_SPEED_MASK;

  switch (Dev->Platform.MaximumSpeed) {
    case UsbBusSpeedSuper:
      Register |= DWC3_DCFG_SUPERSPEED;
      break;
    case UsbBusSpeedFull:
      Register |= DWC3_DCFG_FULLSPEED;
      break;
    default:
      Register |= DWC3_DCFG_HIGHSPEED;
      break;
  }

  // Start out unaddressed
  Register &= ~DWC3_DCFG_DEVADDR_MASK;

  MmioWrite32 (Dev->Base + DWC3_DCFG, Register);

  MmioWrite32 (Dev->Base + DWC3_DEVTEN,
               DWC3_DEVTEN_DISCONNEVTEN  |
               DWC3_DEVTEN_USBRSTEN      |
               DWC3_DEVTEN_CONNECTDONEEN |
               DWC3_DEVTEN_ULSTCNGEN     |
               DWC3_DEVTEN_WKUPEVTEN);

  MmioWrite32 (Dev->Base + DWC3_DALEPENA, 0);

  for (Index = 0; Index < DWC3_MAX_PHYSICAL_EP; Index++) {
    Dev->Endpoints[Index].Enabled         = FALSE;
    Dev->Endpoints[Index].Stalled         = FALSE;
    Dev->Endpoints[Index].TransferStarted = FALSE;
    Dev->Endpoints[Index].PhysEpNum       = Index;
  }

  Dev->Speed     = UsbBusSpeedUnknown;
  Dev->Ep0State  = Dwc3Ep0StateSetup;
  Dev->LinkState = DWC3_LINK_STATE_HS_ON;
  Dev->Suspended = FALSE;

  return EFI_SUCCESS;
}

VOID
Dwc3CoreExit (IN DWC3_DEV *Dev)
{
  UINT32 Register;

  // Mask every Device Event
  MmioWrite32 (Dev->Base + DWC3_DEVTEN, 0);
  MmioWrite32 (Dev->Base + DWC3_DALEPENA, 0);

  Dwc3EventBufferCleanup (Dev);

  // Park PHYs
  Register  = MmioRead32 (Dev->Base + DWC3_GUSB2PHYCFG (0));
  Register |= DWC3_GUSB2PHYCFG_SUSPHY;
  MmioWrite32 (Dev->Base + DWC3_GUSB2PHYCFG (0), Register);

  Register  = MmioRead32 (Dev->Base + DWC3_GUSB3PIPECTL (0));
  Register |= DWC3_GUSB3PIPECTL_SUSPHY;
  MmioWrite32 (Dev->Base + DWC3_GUSB3PIPECTL (0), Register);
}
