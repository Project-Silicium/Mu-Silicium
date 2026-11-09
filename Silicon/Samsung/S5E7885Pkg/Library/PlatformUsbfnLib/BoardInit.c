#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PlatformUsbfnLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "PlatformUsbRegisters.h"

STATIC
VOID
UsbPhyIsolation (IN BOOLEAN Isolate)
{
  UINT32 Register;

  Register = MmioRead32 (EXYNOS7885_PMU_USBDRD_PHY_CTRL);

  if (Isolate) {
    Register &= ~EXYNOS7885_PMU_PHY_ENABLE;
  } else {
    Register |= EXYNOS7885_PMU_PHY_ENABLE;
  }

  MmioWrite32 (EXYNOS7885_PMU_USBDRD_PHY_CTRL, Register);
}

STATIC
EFI_STATUS
UsbPhyRefClockEnable (VOID)
{
  UINT32 Register;
  UINT32 Timeout;

  Register = MmioRead32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_PLL_CON0_PLL_USB);

  if ((Register & PLL_USB_PMS_MASK) != PLL_USB_PMS_50MHZ ||
      !(Register & PLL_USB_ENABLE)                       ||
      !(Register & PLL_USB_MUX_FOUT)) {

    Register &= ~PLL_USB_MUX_FOUT;
    MmioWrite32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_PLL_CON0_PLL_USB, Register);

    MmioWrite32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_PLL_LOCKTIME_PLL_USB, PLL_USB_PDIV * PLL_USB_LOCK_FACTOR);

    Register &= ~PLL_USB_PMS_MASK;
    Register |= PLL_USB_PMS_50MHZ | PLL_USB_ENABLE;
    MmioWrite32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_PLL_CON0_PLL_USB, Register);

    for (Timeout = 2000; Timeout > 0; Timeout--) {
      if (MmioRead32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_PLL_CON0_PLL_USB) & PLL_USB_LOCKED) {
        break;
      }

      gBS->Stall (10);
    }

    if (Timeout == 0) {
      DEBUG ((EFI_D_ERROR, "PLL_USB failed to lock\n"));

      return EFI_DEVICE_ERROR;
    }

    Register |= PLL_USB_MUX_FOUT;
    MmioWrite32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_PLL_CON0_PLL_USB, Register);
  }

  //
  // Ungate Clocks.
  //
  MmioOr32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_GAT_USB20PHY_CLKCORE,         CMU_GATE_ENABLE);
  MmioOr32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_GAT_USB30DRD_ACLK_20PHYCTRL,  CMU_GATE_ENABLE);
  MmioOr32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_GAT_USB30DRD_BUS_CLK_EARLY,   CMU_GATE_ENABLE);
  MmioOr32 (EXYNOS7885_CMU_FSYS_BASE + CMU_FSYS_GAT_USB30DRD_REF_CLK,         CMU_GATE_ENABLE);

  return EFI_SUCCESS;
}

STATIC
VOID
UsbPhyPipe3Override (VOID)
{
  UINT32 Register;

  Register  = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL);
  Register &= ~LINKCTRL_FORCE_PHYSTATUS;
  Register |= LINKCTRL_FORCE_PIPE_EN | LINKCTRL_FORCE_RXELECIDLE;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, Register);

  Register  = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_COMBO_PMA_CTRL);
  Register |= SECPMACTL_PMA_LOW_PWR;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_COMBO_PMA_CTRL, Register);
}

EFI_STATUS
UsbfnPlatformGetData (OUT USBFN_PLATFORM_DATA *PlatformData)
{
  if (PlatformData == NULL)
    return EFI_INVALID_PARAMETER;

  PlatformData->ControllerBase = EXYNOS7885_USBDRD_LINK_BASE;

  PlatformData->MaximumSpeed          = UsbBusSpeedHigh;
  PlatformData->SuperSpeedPhyDisabled = TRUE;

  PlatformData->UtmiWidth = UsbfnUtmiWidth16Bit;

  PlatformData->Usb2FreeClockExists = TRUE;

  PlatformData->FrameLengthAdjustMask  = 0;
  PlatformData->FrameLengthAdjustValue = 0;

  PlatformData->NumEndpoints = 0;

  PlatformData->VendorId  = 0x04E8;
  PlatformData->ProductId = 0x2143;

  return EFI_SUCCESS;
}

EFI_STATUS
UsbfnPlatformPhyInit (VOID)
{
  EFI_STATUS Status;
  UINT32     Register;
  UINT32     SsPorts;

  Status = UsbPhyRefClockEnable ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Register = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CTRL_VER);
  if (Register == 0xFFFFFFFF || Register == 0) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Disable HWACG
  //
  MmioOr32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, LINKCTRL_FORCE_QACT);

  Register = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_PORT);
  SsPorts  = (Register & LINKPORT_HOST_NUM_U3_MASK) >> LINKPORT_HOST_NUM_U3_SHIFT;

  //
  // Start PHY reset.
  //
  Register = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST);
  if (SsPorts != 0) {
    Register |= CLKRST_PHY20_SW_POR | CLKRST_PHY20_SW_POR_SEL | CLKRST_PHY_RESET_SEL;
  }
  Register |= CLKRST_PHY_SW_RST;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST, Register);

  //
  // Enable UTMI+.
  //
  Register  = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI);
  Register &= ~(UTMI_FORCE_SUSPEND | UTMI_FORCE_SLEEP | UTMI_DP_PULLDOWN | UTMI_DM_PULLDOWN);
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI, Register);

  MmioOr32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP, HSP_EN_UTMISUSPEND | HSP_COMMONONN);

  //
  // Bypass filtering.
  //
  MmioOr32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_LINK_CTRL, LINKCTRL_BUS_FILTER_BYPASS (0xF));
  MmioOr32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI,      UTMI_FORCE_BVALID | UTMI_FORCE_VBUSVALID);
  MmioOr32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP,       HSP_VBUSVLDEXT | HSP_VBUSVLDEXTSEL);

  //
  // REFCLK to 50MHz.
  //
  Register  = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_SSPPLLCTL);
  Register &= ~SSPPLLCTL_FSEL_MASK;
  Register |= SSPPLLCTL_FSEL_50MHZ;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_SSPPLLCTL, Register);

  //
  // Power up the PHY analog blocks.
  //
  MmioAnd32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TEST, ~HSP_TEST_SIDDQ);

  //
  // Wait for PHY reset to complete.
  //
  gBS->Stall (10);

  Register = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST);
  if (SsPorts != 0) {
    Register |= CLKRST_PHY20_SW_POR_SEL;
    Register &= ~CLKRST_PHY20_SW_POR;
  }
  Register &= ~(CLKRST_PHY_SW_RST | CLKRST_PORT_RST);
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST, Register);

  gBS->Stall (75);

  //
  // Disable single ended signal out.
  //
  MmioAnd32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP, ~HSP_FSV_OUT_EN);

  if (SsPorts != 0) {
    UsbPhyPipe3Override ();
  }

  //
  // Disable PHY isolation.
  //
  UsbPhyIsolation (FALSE);

  return EFI_SUCCESS;
}

EFI_STATUS
UsbfnPlatformPhyExit (VOID)
{
  UINT32 Register;

  Register  = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI);
  Register &= ~(UTMI_DP_PULLDOWN | UTMI_DM_PULLDOWN);
  Register |= UTMI_FORCE_SUSPEND | UTMI_FORCE_SLEEP;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_UTMI, Register);

  MmioOr32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TEST, HSP_TEST_SIDDQ);

  Register  = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST);
  Register |= CLKRST_LINK_SW_RST;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST, Register);

  gBS->Stall (10);

  Register &= ~CLKRST_LINK_SW_RST;
  MmioWrite32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_CLKRST, Register);

  UsbPhyIsolation (TRUE);

  return EFI_SUCCESS;
}

BOOLEAN
UsbfnPlatformIsVbusPresent (VOID)
{
  UINT32 Register;

  //
  // No VBUS, next best thing.
  //
  Register = MmioRead32 (EXYNOS7885_USBDRD_CTRL_BASE + EXYNOS_USBCON_HSP_TEST);

  return (BOOLEAN)(HSP_TEST_LINESTATE (Register) != 0);
}
