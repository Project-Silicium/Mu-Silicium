/**
  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/PowerServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#include <Protocol/EFIChargerEx.h>

EFI_STATUS
EFIAPI
SystemPowerCheck (
  IN  POWER_CASE Case,
  OUT BOOLEAN   *Good)
{
  EFI_STATUS                Status                = EFI_SUCCESS;
  EFI_CHARGER_EX_PROTOCOL  *gEfiChargerExProtocol = NULL;
  EFI_CHARGER_EX_FLASH_INFO FlashInfo;

  if (FixedPcdGetBool(PcdIsPowerOkImplemented)) {
    // Locate Charger Ex Protocol
    Status = gBS->LocateProtocol (&gChargerExProtocolGuid, NULL, (VOID *)&gEfiChargerExProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Charger Ex Protocol! Status = %r\n", __FUNCTION__, Status));
      *Good = TRUE;
      goto exit;
    }

    // Check if Battery has Enough Power to Boot.
    Status = gEfiChargerExProtocol->IsPowerOk (EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE, &FlashInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Check if Battery has Enough Power! Status = %r\n", __FUNCTION__, Status));
      *Good = TRUE;
    } else {
      *Good = FlashInfo.bCanFlash;
    }
  } else {
    *Good = TRUE;
  }

exit:
  return Status;
}

EFI_STATUS
EFIAPI
SystemPowerMitigate (IN POWER_CASE Case) { return EFI_UNSUPPORTED; }
