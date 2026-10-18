#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/UfsHostBridge.h>

#include <Protocol/EFIGpio.h>

#define UFS_SCLK                       166000000UL
#define CNT_VAL_1US_MASK               0x3FFU
#define UFSHCI_VS_1US_TO_CNT_VAL       0x110CU
#define UFSHCI_VS_UFSHCI_V2P1_CTRL     0x118CU
#define IA_TICK_SEL                    (1U << 16)

#define MUX_CLKCMU_UFS_EMBD_CON        0x15A81064UL
#define DIV_CLKCMU_UFS_EMBD_MUX        0x15A81860UL
#define UFS_CLKCMU_TIMEOUT             100

#define EXYNOS8895_UFS_BASE            0x11120000
#define EXYNOS8895_UFS_VS_BASE         (EXYNOS8895_UFS_BASE + 0x1100)
#define EXYNOS8895_UNIPRO_BASE         (EXYNOS8895_UFS_BASE + 0x10000)
#define EXYNOS8895_PHY_PMA_BASE        (EXYNOS8895_UFS_BASE + 0x4000)

#define EXYNOS8895_PMU_BASE            0x16480000
#define EXYNOS8895_PMU_RST_STAT        (EXYNOS8895_PMU_BASE + 0x404)
#define EXYNOS8895_PMU_SEQUENCER       (EXYNOS8895_PMU_BASE + 0x500)
#define EXYNOS8895_PMU_UFS_PHY_CONTROL (EXYNOS8895_PMU_BASE + 0x724)

#define EXYNOS8895_PERIC1_BASE         0x10980000
#define EXYNOS8895_GPG0_BASE           (EXYNOS8895_PERIC1_BASE + 0x01A0)
#define EXYNOS8895_GPG0_DAT            (EXYNOS8895_GPG0_BASE + 0x0004)

#define EXYNOS8895_SYSREG_FSYS0_BASE    0x11020000
#define EXYNOS8895_SYSREG_FSYS0_IOCOHERENCY (EXYNOS8895_SYSREG_FSYS0_BASE + 0x700)

STATIC EFI_GPIO_PROTOCOL *mGpioProtocol;

STATIC
VOID
UfsVsSet1usToCnt (struct UfsHost *Ufs)
{
  UINT32 nVal = MmioRead32((UINTN)(Ufs->IoAddr + UFSHCI_VS_UFSHCI_V2P1_CTRL));
  nVal |= IA_TICK_SEL;
  MmioWrite32((UINTN)(Ufs->IoAddr + UFSHCI_VS_UFSHCI_V2P1_CTRL), nVal);
  MmioWrite32((UINTN)(Ufs->IoAddr + UFSHCI_VS_1US_TO_CNT_VAL), (UFS_SCLK / 1000000) & CNT_VAL_1US_MASK);
}

STATIC
VOID
UfsSetUniProClk (struct UfsHost *Ufs)
{
  int timeout = 0;
  MmioWrite32(DIV_CLKCMU_UFS_EMBD_MUX, 3);
  do { timeout++; } while ((MmioRead32(DIV_CLKCMU_UFS_EMBD_MUX) & 0x10000) && timeout < UFS_CLKCMU_TIMEOUT);
  timeout = 0;
  MmioWrite32(MUX_CLKCMU_UFS_EMBD_CON, 1);
  do { timeout++; } while ((MmioRead32(MUX_CLKCMU_UFS_EMBD_CON) & 0x10000) && timeout < UFS_CLKCMU_TIMEOUT);
  UfsVsSet1usToCnt (Ufs);
}

EFI_STATUS
UfsBoardInit (struct UfsHost *Ufs)
{
  UINT32 Register;
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID *)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate GPIO Protocol! Status = %r\n", Status));
    return Status;
  }

  DEBUG ((EFI_D_INFO, "UFS: Board init\n"));

  /* UFS Addrs */
  Ufs->IoAddr = (VOID *)(UINTN)EXYNOS8895_UFS_BASE;
  Ufs->VsAddr = (VOID *)(UINTN)EXYNOS8895_UFS_VS_BASE;
  Ufs->UniProAddr = (VOID *)(UINTN)EXYNOS8895_UNIPRO_BASE;
  Ufs->PhyPma = (VOID *)(UINTN)EXYNOS8895_PHY_PMA_BASE;

  /* Power / PHY isolation addresses */
  Ufs->DevPwrAddr = (VOID *)(UINTN)EXYNOS8895_GPG0_DAT;
  Ufs->DevPwrShift = 0;
  Ufs->PhyIsoAddr = (VOID *)(UINTN)EXYNOS8895_PMU_UFS_PHY_CONTROL;

  Ufs->MclkRate = 166 * 1000 * 1000;
  Ufs->GearMode = 3;

  UfsSetUniProClk (Ufs);

  /* GPIO: RST_N and REFCLK */
  Status = mGpioProtocol->SetPull(BANK_ID_I, 0, 0, PULL_NONE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set GPIO pull for RST_N! Status = %r\n", Status));
    return Status;
  }

  Status = mGpioProtocol->SetPull(BANK_ID_I, 0, 1, PULL_NONE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set GPIO pull for REFCLK! Status = %r\n", Status));
    return Status;
  }

  Status = mGpioProtocol->SetFunction(BANK_ID_I, 0, 0, FUNCTION_3);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to configure GPIO pin for RST_N (GPI0-0)! Status = %r\n", Status));
    return Status;
  }

  Status = mGpioProtocol->SetFunction(BANK_ID_I, 0, 1, FUNCTION_3);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to configure GPIO pin for REFCLK (GPI0-1)! Status = %r\n", Status));
    return Status;
  }

  /* XBOOTLDO GPG0[0] */
  Status = mGpioProtocol->SetFunction(BANK_ID_G, 0, 0, FUNCTION_OUTPUT);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to configure GPG0-0! Status = %r\n", Status));
    return Status;
  }

  Register = MmioRead32(EXYNOS8895_SYSREG_FSYS0_IOCOHERENCY);
  Register |= (BIT8 | BIT9);
  MmioWrite32(EXYNOS8895_SYSREG_FSYS0_IOCOHERENCY, Register);

  return EFI_SUCCESS;
}
