#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/UfsHostBridge.h>

#include <Protocol/EFIGpio.h>
#include <Protocol/EFIChipInfo.h>

#define UFS_SCLK                       166000000UL
#define CNT_VAL_1US_MASK               0x3FFU
#define UFSHCI_VS_1US_TO_CNT_VAL       0x110CU
#define UFSHCI_VS_UFSHCI_V2P1_CTRL     0x118CU
#define IA_TICK_SEL                    (1U << 16)

#define CMU_FSYS0_BASE                  0x1A240000UL

#define MUX_CLKCMU_UFS_EMBD_CON_EVT0   (CMU_FSYS0_BASE + 0x1058)
#define MUX_CLKCMU_UFS_EMBD_CON_EVT1   (CMU_FSYS0_BASE + 0x105C)
#define DIV_CLKCMU_UFS_EMBD_MUX_EVT0   (CMU_FSYS0_BASE + 0x1858)
#define DIV_CLKCMU_UFS_EMBD_MUX_EVT1   (CMU_FSYS0_BASE + 0x1854)

#define UFS_CLKCMU_TIMEOUT             100

#define EXYNOS9810_UFS_BASE            0x11120000
#define EXYNOS9810_UFS_VS_BASE         (EXYNOS9810_UFS_BASE + 0x1100)
#define EXYNOS9810_UNIPRO_BASE         (EXYNOS9810_UFS_BASE + 0x10000)
#define EXYNOS9810_PHY_PMA_BASE        (EXYNOS9810_UFS_BASE + 0x4000)

#define EXYNOS9810_PMU_BASE            0x14060000
#define EXYNOS9810_PMU_RST_STAT        (EXYNOS9810_PMU_BASE + 0x404)
#define EXYNOS9810_PMU_SEQUENCER       (EXYNOS9810_PMU_BASE + 0x500)
#define EXYNOS9810_PMU_UFS_PHY_CONTROL (EXYNOS9810_PMU_BASE + 0x724)

#define EXYNOS9810_PERIC1_BASE         0x10830000
#define EXYNOS9810_GPD0_BASE           (EXYNOS9810_PERIC1_BASE + 0x00A0)
#define EXYNOS9810_GPD0_DAT            (EXYNOS9810_GPD0_BASE + 0x0004)

#define EXYNOS9810_SYSREG_FSYS0_BASE    0x11010000
#define EXYNOS9810_SYSREG_FSYS0_IOCOHERENCY (EXYNOS9810_SYSREG_FSYS0_BASE + 0x700)

STATIC EFI_GPIO_PROTOCOL *mGpioProtocol;
STATIC EFI_CHIP_INFO_PROTOCOL *mChipInfoProtocol;

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

  UINT32 MuxRegister, DivRegister;
  UINT8  MajorChipRev, MinorChipRev;

  mChipInfoProtocol->GetRevision (&MajorChipRev, &MinorChipRev);

  if (MajorChipRev == 1) // EVT1
  {
    MuxRegister = MUX_CLKCMU_UFS_EMBD_CON_EVT1;
    DivRegister = DIV_CLKCMU_UFS_EMBD_MUX_EVT1;
  }
  else
  {
    MuxRegister = MUX_CLKCMU_UFS_EMBD_CON_EVT0;
    DivRegister = DIV_CLKCMU_UFS_EMBD_MUX_EVT0;
  }


  MmioWrite32(MuxRegister, 3);
  do { timeout++; } while ((MmioRead32(MuxRegister) & 0x10000) && timeout < UFS_CLKCMU_TIMEOUT);

  timeout = 0;

  MmioWrite32(DivRegister, 1);
  do { timeout++; } while ((MmioRead32(DivRegister) & 0x10000) && timeout < UFS_CLKCMU_TIMEOUT);

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

  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID *)&mChipInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Chip Info Protocol! Status = %r\n", Status));
    return Status;
  }


  DEBUG ((EFI_D_INFO, "UFS: Board init\n"));

  /* UFS Addrs */
  Ufs->IoAddr = (VOID *)(UINTN)EXYNOS9810_UFS_BASE;
  Ufs->VsAddr = (VOID *)(UINTN)EXYNOS9810_UFS_VS_BASE;
  Ufs->UniProAddr = (VOID *)(UINTN)EXYNOS9810_UNIPRO_BASE;
  Ufs->PhyPma = (VOID *)(UINTN)EXYNOS9810_PHY_PMA_BASE;

  /* Power / PHY isolation addresses */
  Ufs->DevPwrAddr = (VOID *)(UINTN)EXYNOS9810_GPD0_DAT;
  Ufs->DevPwrShift = 2;
  Ufs->PhyIsoAddr = (VOID *)(UINTN)EXYNOS9810_PMU_UFS_PHY_CONTROL;

  Ufs->MclkRate = 166 * 1000 * 1000;
  Ufs->GearMode = 3;

  UfsSetUniProClk (Ufs);

  /* GPIO: RST_N and REFCLK */
  Status = mGpioProtocol->SetPull(BANK_ID_F, 0, 1, PULL_NONE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set GPIO pull for RST_N! Status = %r\n", Status));
    return Status;
  }

  Status = mGpioProtocol->SetPull(BANK_ID_F, 0, 0, PULL_NONE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set GPIO pull for REFCLK! Status = %r\n", Status));
    return Status;
  }

  Status = mGpioProtocol->SetFunction(BANK_ID_F, 0, 1, FUNCTION_2);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to configure GPIO pin for RST_N! Status = %r\n", Status));
    return Status;
  }

  Status = mGpioProtocol->SetFunction(BANK_ID_F, 0, 0, FUNCTION_2);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to configure GPIO pin for REFCLK! Status = %r\n", Status));
    return Status;
  }

  /* XBOOTLDO GPD0[2] */
  Status = mGpioProtocol->SetFunction(BANK_ID_D, 0, 2, FUNCTION_OUTPUT);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to configure GPD0-2! Status = %r\n", Status));
    return Status;
  }

  Register = MmioRead32(EXYNOS9810_SYSREG_FSYS0_IOCOHERENCY);
  Register |= (BIT8 | BIT9);
  MmioWrite32(EXYNOS9810_SYSREG_FSYS0_IOCOHERENCY, Register);

  return EFI_SUCCESS;
}
