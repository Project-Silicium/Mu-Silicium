/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PlatformRegisters.h"

VOID
DisableWatchdogTimer ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR WatchdogTimerRegion;
  UINT32                       Value;

  // Locate Watchdog Timer Memory Region
  Status = LocateMemoryRegionByName ("Watchdog Timer", &WatchdogTimerRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable Watchdog Timer
  MmioAnd32 (WatchdogTimerRegion.Address, ~WATCHDOG_ENABLE);
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
  for (UINT16 Length = 0; Length < DrmDeconRegion.Length; Length += 0x1000) {
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
