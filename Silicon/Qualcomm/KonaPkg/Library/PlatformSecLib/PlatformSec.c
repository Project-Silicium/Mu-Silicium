/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ArmSmmuDetachLib.h>

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

  // Cluster 1
  { 0x400, 0, 0, 0, 0xFFFFFFFF },
  { 0x500, 0, 0, 0, 0xFFFFFFFF },
  { 0x600, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 2
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
  EFI_MEMORY_REGION_DESCRIPTOR ApssRegion;

  // Locate "APSS_HM" Memory Region
  Status = LocateMemoryRegionByName ("APSS_HM", &ApssRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable WatchDog Timer
  MmioWrite32 (ApssRegion.Address + APSS_WDT_OFFSET + APSS_WDT_ENABLE_OFFSET, 0x0);
}

VOID
PlatformInitialize ()
{
  // Disable WatchDog Timer
  DisableWatchDogTimer ();

  // Set MDP SIDs
  CONST UINT16 MdpStreams[] = { 0x820, 0x821, 0xC21 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
