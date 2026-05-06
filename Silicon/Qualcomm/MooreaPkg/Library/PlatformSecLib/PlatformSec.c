/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>
//#include <Library/ArmSmmuDetachLib.h>

#include "PlatformRegisters.h"

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  // Mpidr, MailboxSetAddress, MailboxGetAddress, MailboxClearAddress, MailboxClearValue

  // Cluster 0
  { 0x000, 0, 0, 0, 0xFFFFFFFF },
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x200, 0, 0, 0, 0xFFFFFFFF },
  { 0x300, 0, 0, 0, 0xFFFFFFFF },
  { 0x400, 0, 0, 0, 0xFFFFFFFF },
  { 0x500, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 1
  { 0x600, 0, 0, 0, 0xFFFFFFFF },
  { 0x700, 0, 0, 0, 0xFFFFFFFF }
};

VOID
GetPlatformCoreTable (
  OUT ARM_CORE_INFO **ArmCoreTable,
  OUT UINTN          *CoreCount)
{
  // Pass Data
  *ArmCoreTable = mArmPlatformMpCoreInfoTable;
  *CoreCount    = ARRAY_SIZE (mArmPlatformMpCoreInfoTable);
}

VOID
DisableWatchDogTimer ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR WDogRegion;

  // Locate "APSS_WDT_TMR1" Memory Region
  Status = LocateMemoryRegionByName ("APSS_WDT_TMR1", &WDogRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable WatchDog Timer
  MmioWrite32 (WDogRegion.Address + APSS_WDT_ENABLE_OFFSET, 0x0);
}

VOID
EnableUfsCacheCoherency ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR SmmuRegion;
  UINT32                       ContextBankAddr;

  // Locate "SMMU" Memory Region
  Status = LocateMemoryRegionByName ("SMMU", &SmmuRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Set UFS Context Bank Address
  ContextBankAddr = SmmuRegion.Address + SMMU_CTX_BANK_0_OFFSET + UFS_CTX_BANK * SMMU_CTX_BANK_SIZE;

  // Configure UFS Context Bank
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_SCTLR_OFFSET,   SMMU_CCA_SCTLR);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR0_0_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR0_1_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR1_0_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBR1_1_OFFSET, 0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_MAIR0_OFFSET,   0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_MAIR1_OFFSET,   0);
  MmioWrite32 (ContextBankAddr + SMMU_CTX_BANK_TTBCR_OFFSET,   0);
}

VOID
PlatformInitialize ()
{
  // Disable WatchDog Timer
  DisableWatchDogTimer ();

  // Enable UFS Cache Coherency (DEPRECATED!)
  EnableUfsCacheCoherency ();

  // Set MDP SIDs
  //CONST UINT16 MdpStreams[] = { 0x800, 0x801 };

  // Detach IOMMU Domains
  //ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
