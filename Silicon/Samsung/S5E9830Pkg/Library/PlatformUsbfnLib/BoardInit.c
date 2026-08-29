#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PlatformUsbfnLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Device/Dwc3.h>

#include <Protocol/EFIChipInfo.h>

#include "PlatformUsbRegisters.h"

#define EXYNOS9830_FLADJ_30MHZ          0x20

#define EXYNOS9830_PMU_BASE             0x15860000
#define EXYNOS9830_PMU_USBDRD_PHY_CTRL  (EXYNOS9830_PMU_BASE + 0x072C)
#define EXYNOS9830_PMU_USBDP_PHY_CTRL   (EXYNOS9830_PMU_BASE + 0x0704)
#define EXYNOS9830_PMU_PHY_ENABLE       BIT0

#define EXYNOS9830_USBDRD_LINK_BASE     0x10E00000
#define EXYNOS9830_USBDRD_CTRL_BASE     0x10C00000
#define EXYNOS9830_USBDP_PMA_BASE       0x10AE0000
#define EXYNOS9830_USBDP_PCS_BASE       0x10AF0000

STATIC
VOID
UsbPhyIsolation (IN BOOLEAN Enable)
{
  UINT32 Register;

  Register = MmioRead32 (EXYNOS9830_PMU_USBDRD_PHY_CTRL);

  if (Enable) {
    Register |= EXYNOS9830_PMU_PHY_ENABLE;
  } else {
    Register &= ~EXYNOS9830_PMU_PHY_ENABLE;
  }

  MmioWrite32 (EXYNOS9830_PMU_USBDRD_PHY_CTRL, Register);
}

STATIC
VOID
UsbPhyForceQChannel (VOID)
{
  UINT32 Register;

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL);
  Register |= LINKCTRL_DIS_QACT_ID0;
  Register |= LINKCTRL_DIS_QACT_VBUS_VALID;
  Register |= LINKCTRL_DIS_QACT_BVALID;
  Register |= LINKCTRL_DIS_QACT_LINKGATE;
  Register &= ~LINKCTRL_FORCE_QACT;

  gBS->Stall (500);
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, Register);
  gBS->Stall (500);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL);
  Register |= LINKCTRL_FORCE_QACT;

  gBS->Stall (500);
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, Register);
}

STATIC
VOID
UsbPhyPowerEnable (IN BOOLEAN Enable)
{
  UINT32 Register;

  Register = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TEST);

  if (Enable) {
    Register &= ~HSP_TEST_SIDDQ;
  } else {
    Register |= HSP_TEST_SIDDQ;
  }

  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TEST, Register);
}

STATIC
VOID
UsbPhySoftResetAssert (VOID)
{
  UINT32 Register;

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST);
  Register |= CLKRST_PHY20_SW_RST | CLKRST_PHY20_RST_SEL;
  Register |= CLKRST_PHY30_SW_RST | CLKRST_PHY30_RST_SEL;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST, Register);
}

STATIC
VOID
UsbPhySoftResetDeassert (VOID)
{
  UINT32 Register;

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST);
  Register |= CLKRST_PHY20_RST_SEL;
  Register &= ~(CLKRST_PHY20_SW_RST | CLKRST_PHY30_SW_RST | CLKRST_PORT_RST);
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST, Register);
}

STATIC
VOID
UsbPhyTune (VOID)
{
  UINT32 Register;

  Register = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TUNE);

  Register &= ~HSP_TUNE_COMPDIS_MASK;
  Register |= HSP_TUNE_COMPDIS (EXYNOS9830_TUNE_COMPDIS);

  Register &= ~HSP_TUNE_SQRX_MASK;
  Register |= HSP_TUNE_SQRX (EXYNOS9830_TUNE_RX_SQRX);

  Register &= ~HSP_TUNE_TXPREEMPA_MASK;
  Register |= HSP_TUNE_TXPREEMPA (EXYNOS9830_TUNE_TX_PRE_EMP);

  Register &= ~HSP_TUNE_TXRES_MASK;
  Register |= HSP_TUNE_TXRES (EXYNOS9830_TUNE_TX_RES);

  Register &= ~HSP_TUNE_TXVREF_MASK;
  Register |= HSP_TUNE_TXVREF (EXYNOS9830_TUNE_TX_VREF);

  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TUNE, Register);
}

STATIC
VOID
UsbPhyPipe3Override (VOID)
{
  UINT32 Register;

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL);
  Register |= LINKCTRL_PIPE3_FORCE_EN;
  Register &= ~LINKCTRL_PIPE3_FORCE_PHY_STATUS;
  Register |= LINKCTRL_PIPE3_FORCE_RX_ELEC_IDLE;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, Register);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_COMBO_PMA_CTRL);
  Register |= PMA_LOW_PWR;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_COMBO_PMA_CTRL, Register);
}

STATIC
VOID
UsbfnGetFrameLengthAdjust (OUT USBFN_PLATFORM_DATA *PlatformData)
{
  EFI_STATUS              Status;
  EFI_CHIP_INFO_PROTOCOL *ChipInfo;
  UINT8                   MajorRevision = 1;
  UINT8                   MinorRevision = 1;

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID *)&ChipInfo);
  ASSERT_EFI_ERROR (Status);

  ChipInfo->GetRevision (&MajorRevision, &MinorRevision);

  PlatformData->FrameLengthAdjustMask = DWC3_GFLADJ_30MHZ_MASK |
                                        DWC3_GFLADJ_REFCLK_240MHZ_DECR_MASK |
                                        DWC3_GFLADJ_REFCLK_FLADJ_MASK;

  if (MajorRevision == 0) {
    PlatformData->FrameLengthAdjustValue = EXYNOS9830_FLADJ_30MHZ |
                                           DWC3_GFLADJ_REFCLK_240MHZ_DECR (0xA) |
                                           DWC3_GFLADJ_REFCLK_FLADJ (0x7F0) |
                                           DWC3_GFLADJ_REFCLK_LPM_SEL;
  } else {
    PlatformData->FrameLengthAdjustValue = EXYNOS9830_FLADJ_30MHZ |
                                           DWC3_GFLADJ_REFCLK_240MHZ_DECR (0xC) |
                                           DWC3_GFLADJ_REFCLK_LPM_SEL |
                                           DWC3_GFLADJ_30MHZ_SDBND_SEL;
  }
}

EFI_STATUS
UsbfnPlatformGetData (OUT USBFN_PLATFORM_DATA *PlatformData)
{
  if (PlatformData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PlatformData->ControllerBase = EXYNOS9830_USBDRD_LINK_BASE;

  PlatformData->MaximumSpeed          = UsbBusSpeedHigh;
  PlatformData->SuperSpeedPhyDisabled = TRUE;

  PlatformData->UtmiWidth = UsbfnUtmiWidth16Bit;

  PlatformData->Usb2FreeClockExists = FALSE;

  UsbfnGetFrameLengthAdjust (PlatformData);

  PlatformData->NumEndpoints = 0;

  PlatformData->VendorId  = 0x04E8;
  PlatformData->ProductId = 0x2143;

  return EFI_SUCCESS;
}

EFI_STATUS
UsbfnPlatformPhyInit (VOID)
{
  UINT32 Register;

  UsbPhyIsolation (TRUE);

  gBS->Stall (10);

  Register = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_CTRL_VER);
  if (Register == 0xFFFFFFFF || Register == 0) {
    return EFI_DEVICE_ERROR;
  }

  UsbPhyForceQChannel ();
  UsbPhySoftResetAssert ();

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI);
  Register &= ~(UTMI_FORCE_SUSPEND | UTMI_FORCE_SLEEP);
  Register &= ~(UTMI_DP_PULLDOWN | UTMI_DM_PULLDOWN);
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI, Register);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP);
  Register |= HSP_EN_UTMISUSPEND;
  Register |= HSP_COMMONONN;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP, Register);

  gBS->Stall (100);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL);
  Register |= LINKCTRL_BUS_FILTER_BYPASS (0xF);
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, Register);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI);
  Register |= UTMI_FORCE_BVALID | UTMI_FORCE_VBUSVALID;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI, Register);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP);
  Register |= HSP_VBUSVLDEXTSEL | HSP_VBUSVLDEXT;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP, Register);

  UsbPhyTune ();
  UsbPhyPowerEnable (TRUE);
  gBS->Stall (10);
  UsbPhySoftResetDeassert ();
  gBS->Stall (75);
  UsbPhyPipe3Override ();


  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_PORT);
  Register |= LINKPORT_HUB_PORT_SEL_OCD_U3 | LINKPORT_HUB_PORT_SEL_OCD_U2;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_PORT, Register);

  return EFI_SUCCESS;
}

EFI_STATUS
UsbfnPlatformPhyExit (VOID)
{
  UINT32 Register;

  // Stop forcing VBUS
  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI);
  Register &= ~(UTMI_FORCE_BVALID | UTMI_FORCE_VBUSVALID);
  Register |= UTMI_FORCE_SUSPEND | UTMI_FORCE_SLEEP;
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI, Register);

  Register  = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP);
  Register &= ~(HSP_VBUSVLDEXTSEL | HSP_VBUSVLDEXT);
  MmioWrite32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP, Register);

  // Power the PHY down
  UsbPhyPowerEnable (FALSE);

  UsbPhySoftResetAssert ();

  UsbPhyIsolation (FALSE);
  return EFI_SUCCESS;
}

BOOLEAN
UsbfnPlatformIsVbusPresent (VOID)
{
  UINT32 Register;

  // No vbus pad, next best thing.
  Register = MmioRead32 (EXYNOS9830_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TEST);

  return (BOOLEAN)(HSP_TEST_LINESTATE (Register) != 0);
}
