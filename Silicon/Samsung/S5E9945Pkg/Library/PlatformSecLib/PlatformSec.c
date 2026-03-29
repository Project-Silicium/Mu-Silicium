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
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x200, 0, 0, 0, 0xFFFFFFFF },
  { 0x300, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 1
  { 0x400, 0, 0, 0, 0xFFFFFFFF },
  { 0x500, 0, 0, 0, 0xFFFFFFFF },
  { 0x600, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 2
  { 0x700, 0, 0, 0, 0xFFFFFFFF },
  { 0x800, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 3
  { 0x900, 0, 0, 0, 0xFFFFFFFF }
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
DisableWatchdogTimer ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR WatchdogTimerRegion;
  UINT32                       Value;

  // Locate Watchdog Timner Memory Region
  Status = LocateMemoryRegionByName ("Watchdog Timer", &WatchdogTimerRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get current Config
  Value = MmioRead32 (WatchdogTimerRegion.Address);

  // Update Config
  Value &= ~WATCHDOG_ENABLE;

  // Write new Config
  MmioWrite32 (WatchdogTimerRegion.Address, Value);
}

VOID
EnableFrameBufferWrites ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR DrmDeconRegion;

  // Locate "DRM Decon" Memory Region
  Status = LocateMemoryRegionByName ("DRM Decon", &DrmDeconRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Configure DRM Decon
  for (UINT16 Length = 0; i < DrmDeconRegion.Length; Length += 0x1000) {
    MmioWrite32 (DrmDeconRegion.Address + Length + HW_SW_TRIG_CONTROL, 0x1281);
  }
}

VOID
PlatformInitialize ()
{
  // Disable Watchdog Timer
  DisableWatchdogTimer ();

  // Enable Frame Buffer Writes
  EnableFrameBufferWrites ();
}
