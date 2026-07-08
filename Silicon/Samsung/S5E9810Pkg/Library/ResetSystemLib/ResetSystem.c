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
#include <Library/IoLib.h>

#include "Pmu.h"

//
// Global Variables
//
STATIC EFI_MEMORY_REGION_DESCRIPTOR mPmuRegion = {0};

VOID
EFIAPI
ResetCold ()
{
  // Verify PMU Memory Region Presense
  if (mPmuRegion.Address == 0 && mPmuRegion.Length == 0) {
    DEBUG ((EFI_D_WARN, "%a: The \"PMU\" Memory Region isn't Mapped!\n", __FUNCTION__));
    return;
  }

  // Set SWRESET
  MmioWrite32 (mPmuRegion.Address + SWRESET, TRUE);
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
  // Verify PMU Memory Region Presense
  if (mPmuRegion.Address == 0 && mPmuRegion.Length == 0) {
    DEBUG ((EFI_D_WARN, "%a: The \"PMU\" Memory Region isn't Mapped!\n", __FUNCTION__));
    return;
  }

  // Set PS_HOLD Low
  MmioAnd32 (mPmuRegion.Address + PS_HOLD_CONTROL, ~PS_HOLD_LOW);
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

  // Locate PMU Memory Region
  Status = LocateMemoryRegionByName ("PMU", &mPmuRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"PMU\" Memory Region! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}
