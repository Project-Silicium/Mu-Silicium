/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/PlatformSecLib.h>

#include "PlatformRegisters.h"

VOID
DisableWatchDogTimer ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR WDogRegion;

  // Locate "WatchDog Timer" Memory Region
  Status = LocateMemoryRegionByName ("WatchDog Timer", &WDogRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable WatchDog Timer
  MmioWrite32 (WDogRegion.Address, WDT_MODE_KEY);
}

VOID
StartSystemTimer ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR CpuxgptRegion;
  UINT32                       Value;

  // Locate "CPUXGPT" Memory Region
  Status = LocateMemoryRegionByName ("CPUXGPT", &CpuxgptRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Read Global Control Register
  MmioWrite32 (CpuxgptRegion.Address + CPUXGPT_IDX_OFFSET, CPUXGPT_IDX_GLOBAL_CTRL);
  Value = MmioRead32 (CpuxgptRegion.Address + CPUXGPT_CON_OFFSET);

  // Leave an already running System Timer alone
  if (Value & CPUXGPT_ENABLE) {
    return;
  }

  // Set Clock Divider
  Value = (Value & ~CPUXGPT_CLK_DIV_MASK) | CPUXGPT_CLK_DIV2;
  MmioWrite32 (CpuxgptRegion.Address + CPUXGPT_IDX_OFFSET, CPUXGPT_IDX_GLOBAL_CTRL);
  MmioWrite32 (CpuxgptRegion.Address + CPUXGPT_CON_OFFSET, Value);

  // Start the System Timer
  MmioWrite32 (CpuxgptRegion.Address + CPUXGPT_IDX_OFFSET, CPUXGPT_IDX_GLOBAL_CTRL);
  MmioWrite32 (CpuxgptRegion.Address + CPUXGPT_CON_OFFSET, Value | CPUXGPT_ENABLE);
}

VOID
EnableConstantBlending ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR OvlRegion;
  UINT32                       Value;

  // Locate "Display OVL" Memory Region
  Status = LocateMemoryRegionByName ("Display OVL", &OvlRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Read Layer 0 Config
  Value = MmioRead32 (OvlRegion.Address + OVL_CON_OFFSET (0));

  // Disable Pixel Alpha Blending and use the Constant Alpha
  Value &= ~OVL_ALPHA_ENABLE;
  Value &= ~OVL_CONST_ALPHA_MASK;
  Value |= OVL_CONST_ALPHA_OPAQUE;

  // Write Layer 0 Config
  MmioWrite32 (OvlRegion.Address + OVL_CON_OFFSET (0), Value);
}

VOID
PlatformInitialize ()
{
  // Disable WatchDog Timer
  DisableWatchDogTimer ();

  // Start System Timer
  StartSystemTimer ();

  // Enable Constant Blending
  EnableConstantBlending ();
}
