/**
  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.
  Copyright (c) 2013, ARM Ltd. All rights reserved.
  Copyright (c) 2014, Linaro Ltd. All rights reserved.
  Copyright (c) 2019, Intel Corporation. All rights reserved.
  Copyright (c) 2024, Google Llc. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include "Wdt.h"

//
// Global Variables
//
STATIC EFI_MEMORY_REGION_DESCRIPTOR  mWdtRegion = {0};

STATIC
VOID
WdtReset (
  IN BOOLEAN BypassPwrKey)
{
  UINT32 Mode;
  
  // Verify WatchDog Memory Region Presence
  if (mWdtRegion.Address == 0 && mWdtRegion.Length == 0) {
    DEBUG ((EFI_D_WARN, "%a: The \"WatchDog Timer\" Memory Region isn't Mapped!\n", __FUNCTION__));
    return;
  }

  // Kick the Watchdog
  MmioWrite32 (mWdtRegion.Address + WDT_RESTART, WDT_RESTART_KEY);

  // Select the Hardware Reset Mode
  Mode  = MmioRead32 (mWdtRegion.Address + WDT_MODE);
  Mode &= ~(WDT_MODE_ENABLE | WDT_MODE_IRQ | WDT_MODE_DUAL | WDT_MODE_AUTO_RESTART);
  Mode |= WDT_MODE_KEY | WDT_MODE_EXTEN;

  // Tell the Preloader to boot again
  if (BypassPwrKey) {
    Mode |= WDT_MODE_AUTO_RESTART;
  }

  MmioWrite32 (mWdtRegion.Address + WDT_MODE, Mode);

  MicroSecondDelay (100);

  // Trigger SoC Software Reset
  MmioWrite32 (mWdtRegion.Address + WDT_SWRST, WDT_SWRST_KEY);

  // Wait for the Reset to happen
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetCold ()
{
  // Reboot
  WdtReset (TRUE);
}

VOID
EFIAPI
ResetWarm ()
{
  // Do Cold Reboot instead
  ResetCold ();
}

VOID
EFIAPI
ResetShutdown ()
{
  // Power Off
  WdtReset (FALSE);
}

VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN  DataSize,
  IN VOID  *ResetData)
{
  // Do Cold Reboot instead
  ResetCold ();
}

VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData)
{
  // Check Reset Type
  switch (ResetType) {
    case EfiResetWarm:
      ResetWarm ();
      break;

    case EfiResetCold:
      ResetCold ();
      break;

    case EfiResetShutdown:
      ResetShutdown ();
      return;

    case EfiResetPlatformSpecific:
      ResetPlatformSpecific (DataSize, ResetData);
      return;

    default:
      return;
  }
}

EFI_STATUS
EFIAPI
ResetSystemLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate WatchDog Memory Region
  Status = LocateMemoryRegionByName ("WatchDog Timer", &mWdtRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"WatchDog Timer\" Memory Region! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}
