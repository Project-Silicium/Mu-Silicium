/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PlatformRegisters.h"

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  // Mpidr, MailboxSetAddress, MailboxGetAddress, MailboxClearAddress, MailboxClearValue

  // Cluster 0
  { 0x000, 0, 0, 0, 0xFFFFFFFF },
  { 0x001, 0, 0, 0, 0xFFFFFFFF },
  { 0x002, 0, 0, 0, 0xFFFFFFFF },
  { 0x003, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 1
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x101, 0, 0, 0, 0xFFFFFFFF },
  { 0x102, 0, 0, 0, 0xFFFFFFFF },
  { 0x103, 0, 0, 0, 0xFFFFFFFF }
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
  EFI_MEMORY_REGION_DESCRIPTOR ApcsRegion;

  // Locate "APCS_CC" Memory Region
  Status = LocateMemoryRegionByName ("APCS_CC", &ApcsRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable WatchDog Timer
  MmioWrite32 (ApcsRegion.Address + APSS_WDT_OFFSET + APSS_WDT_ENABLE_OFFSET, 0x0);
}

VOID
PlatformInitialize ()
{
  // Disable WatchDog Timer
  DisableWatchDogTimer ();
}
