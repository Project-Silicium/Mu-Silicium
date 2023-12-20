/** @file
  Power Services library class

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>                                     // UEFI base types

#include <Library/UefiBootServicesTableLib.h>         // gBS
#include <Library/PowerServicesLib.h>
#include <Library/DebugLib.h>

#include <Protocol/EFIChargerEx.h>

STATIC EFI_CHARGER_EX_PROTOCOL *gEfiChargerExProtocol = NULL;

EFI_STATUS
EFIAPI
SystemPowerCheck (
  IN  POWER_CASE  Case,
  OUT BOOLEAN    *Good)
{
  EFI_STATUS                Status;
  EFI_CHARGER_EX_FLASH_INFO FlashInfo;

  if (gEfiChargerExProtocol == NULL) {
    Status = gBS->LocateProtocol (&gChargerExProtocolGuid, NULL, (VOID **)&gEfiChargerExProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to locate ChargerEx Protocol! Status = %r\n", Status));
      gEfiChargerExProtocol = NULL;
    }
  }

  if (gEfiChargerExProtocol == NULL) {
    *Good = TRUE;
  } else {
    Status = gEfiChargerExProtocol->IsPowerOk(EFI_CHARGER_EX_POWER_FLASH_BATTERY_VOLTAGE_TYPE, &FlashInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Check if Power is Ok! Status = %r\n", Status));
      *Good = TRUE;
    } else {
      *Good = FlashInfo.bCanFlash;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SystemPowerMitigate (IN POWER_CASE Case)
{
  return EFI_UNSUPPORTED;
}
