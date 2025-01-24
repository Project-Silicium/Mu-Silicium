/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/BootGraphics.h>
#include <Library/DxeServicesLib.h>

EFI_STATUS
EFIAPI
GetBootGraphic (
  BOOT_GRAPHIC Graphic,
  OUT UINTN   *ImageSize,
  OUT UINT8  **ImageData)
{
  EFI_GUID *g = NULL;

  switch (Graphic) {
    case BG_SYSTEM_LOGO:
      g = PcdGetPtr (PcdLogoFile);
      break;

    case BG_CRITICAL_OVER_TEMP:
      g = PcdGetPtr (PcdThermalFile);
      break;

    case BG_CRITICAL_LOW_BATTERY:
      g = PcdGetPtr (PcdLowBatteryFile);
      break;

    case BG_NO_BOOT_OS:
      g = PcdGetPtr (PcdNoBootOSFile);
      break;
    
    case BG_MSD_WARNING:
      g = PcdGetPtr (PcdMsdWarningFile);
      break;
    
    case BG_MSD_CONNECTED:
      g = PcdGetPtr (PcdMsdConnectedFile);
      break;
    
    case BG_MSD_DISCONNECTED:
      g = PcdGetPtr (PcdMsdDisconnectedFile);
      break;

    case BG_MSD_ERROR:
      g = PcdGetPtr (PcdMsdErrorFile);
      break;

    default:
      DEBUG ((EFI_D_ERROR, "%a: Unsupported Boot Graphic Type! 0x%x\n", __FUNCTION__, Graphic));
      return EFI_UNSUPPORTED;
  }

  // Get the Specified Image from FV
  return GetSectionFromAnyFv (g, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);
}

UINT32
EFIAPI
GetBackgroundColor ()
{
  return PcdGet32 (PcdPostBackgroundColor);
}
