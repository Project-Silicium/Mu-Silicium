#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>

#include "PlatformRegisters.h"

VOID
ConfigureIOMMUContextBankCacheSetting (UINT32 ContextBankId)
{
  UINT32 ContextBankAddr = SMMU_BASE + SMMU_CTX_BANK_0_OFFSET + ContextBankId * SMMU_CTX_BANK_SIZE;

  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_SCTLR_OFFSET, SMMU_CCA_SCTLR);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR0_0_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR0_1_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR1_0_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR1_1_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_MAIR0_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_MAIR1_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBCR_OFFSET, 0);
}

VOID
PlatformInitialize ()
{
  //
  // Windows Requires Cache Coherency for the UFS to Work at its Best.
  // The UFS Device is Currently Attached to the Main IOMMU on Context Bank 1.
  // Configure Cache Coherency for Best UFS Performance.
  //
  ConfigureIOMMUContextBankCacheSetting (UFS_CTX_BANK);

  // Disable WatchDog Timer
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_ENABLE_OFFSET, 0x0);
}
