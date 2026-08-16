/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PlatformRegisters.h"

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
