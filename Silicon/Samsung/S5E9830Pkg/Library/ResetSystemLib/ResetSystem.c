/**
  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.
  Copyright (c) 2013, ARM Ltd. All rights reserved.
  Copyright (c) 2014, Linaro Ltd. All rights reserved.
  Copyright (c) 2019, Intel Corporation. All rights reserved.
  Copyright (c) 2024, Google Llc. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIPmicExtra.h>

#include "Pmu.h"

//
// Global Variables
//
STATIC EFI_PMIC_EXTRA_PROTOCOL      *mPmicExtraProtocol = NULL;
STATIC EFI_MEMORY_REGION_DESCRIPTOR  mPmuRegion         = {0};

VOID
SecSetRebootMagic (
  IN INT32 Magic,
  IN INT32 Offset,
  IN INT32 Mask)
{
  UINT32 Value;

  // Read SEC_DEBUG_MAGIC_INFORM State
  Value = MmioRead32 (mPmuRegion.Address + SEC_DEBUG_MAGIC_INFORM);

  // Combine Values
  Mask  <<= Offset;
  Value &=  (~Mask);
  Value |=  Magic << Offset;

  // Write new SEC_DEBUG_MAGIC_INFORM State
  MmioWrite32 (mPmuRegion.Address + SEC_DEBUG_MAGIC_INFORM, Value);
}

VOID
EFIAPI
ResetCold ()
{
  // Verify PMU Memory Region Presence
  if (mPmuRegion.Address == 0 && mPmuRegion.Length == 0) {
    DEBUG ((EFI_D_WARN, "%a: The \"PMU\" Memory Region isn't Mapped!\n", __FUNCTION__));
    return;
  }

  // Set reboot reason to normal
  SecSetRebootMagic (SEC_REBOOT_NORMAL, SEC_REBOOT_END_OFFSET, 0xFF);

  // Clear RST_STAT
  MmioWrite32 (mPmuRegion.Address + RST_STAT, 0);

  // Set SWRESET
  MmioOr32 (mPmuRegion.Address + SWRESET, 2);
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
  EFI_STATUS Status;

  // Verify PMIC Extra Protocol Presence
  if (mPmicExtraProtocol == NULL) {
    DEBUG ((EFI_D_WARN, "%a: The PMIC Extra Protocol isn't Registered!\n", __FUNCTION__));
    return;
  }

  // Verify PMU Memory Region Presence
  if (mPmuRegion.Address == 0 && mPmuRegion.Length == 0) {
    DEBUG ((EFI_D_WARN, "%a: The \"PMU\" Memory Region isn't Mapped!\n", __FUNCTION__));
    return;
  }

  // Disable WTSR
  Status = mPmicExtraProtocol->SetWtsr (ID_S2MPS19, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable WTSR! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Disable SMPL
  Status = mPmicExtraProtocol->SetSmpl (ID_S2MPS19, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable SMPL! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Power Down PMIC
  Status = mPmicExtraProtocol->PowerDown (ID_S2MPS22);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Power Down PMIC! Status = %r\n", __FUNCTION__, Status));
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
  UINT32 Reason = SEC_RESET_REASON_UNKNOWN;

  if (DataSize == 0 || ResetData == NULL || DataSize < 8) {
    DEBUG ((EFI_D_WARN, "%a: Invalid Reset Data!\n", __FUNCTION__));
    goto reset;
  }

  // Check reboot reasons
  if (!StrnCmp (ResetData, L"RECOVERY", 8)) {
    Reason = SEC_RESET_REASON_RECOVERY;
    goto reset;
  }

  if (!StrnCmp (ResetData, L"DOWNLOAD", 8)) {
    Reason = SEC_RESET_REASON_DOWNLOAD;
    goto reset;
  }

  if (!StrnCmp (ResetData, L"BOOTLOADER", 10)) {
    Reason = SEC_RESET_REASON_BOOTLOADER;
    goto reset;
  }

  if (!StrnCmp (ResetData, L"FASTBOOTD", 9)) {
    Reason = SEC_RESET_REASON_RECOVERY;
    MmioWrite32 (mPmuRegion.Address + EXYNOS_PMU_SYSIP_DAT0, REBOOT_MODE_FASTBOOT);
    goto reset;
  }

reset:
  MmioWrite32 (mPmuRegion.Address + SEC_DEBUG_PANIC_INFORM, Reason);
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

  // Locate PMIC Extra Protocol
  Status = gBS->LocateProtocol (&gEfiPmicExtraProtocolGuid, NULL, (VOID *)&mPmicExtraProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate PMIC Extra Protocol! Status = %r\n", Status));
  }

  // Locate PMU Memory Region
  Status = LocateMemoryRegionByName ("PMU", &mPmuRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"PMU\" Memory Region! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}
