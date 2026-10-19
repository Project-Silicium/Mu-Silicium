/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PlatformRegisters.h"

VOID
EnableFrameBufferWrites ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR DeconRegion;

  // Locate "Decon" Memory Region
  Status = LocateMemoryRegionByName ("Decon", &DeconRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Configure Decon
  MmioWrite32 (DeconRegion.Address + HW_SW_TRIG_CONTROL, 0x1281);
}

VOID
PlatformInitialize ()
{
  // Enable Frame Buffer Writes
  EnableFrameBufferWrites ();
}
