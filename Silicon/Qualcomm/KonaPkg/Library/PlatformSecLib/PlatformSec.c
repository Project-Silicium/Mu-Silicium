/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ArmSmmuDetachLib.h>

#include "PlatformRegisters.h"

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
  CONST UINT16 MdpStreams[] = { 0x820, 0x821, 0x822, 0xC20, 0xC21, 0xC22 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
