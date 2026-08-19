/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>
//#include <Library/ArmSmmuDetachLib.h>

#include "PlatformRegisters.h"

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
PlatformInitialize ()
{
  // Disable WatchDog Timer
  DisableWatchDogTimer ();

  // Set MDP SIDs
  //CONST UINT16 MdpStreams[] = { 0x880, 0x881, 0x888, 0x889, 0xC80, 0xC81, 0xC88, 0xC89 };

  // Detach IOMMU Domains
  //ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
