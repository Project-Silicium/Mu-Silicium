/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/BootGraphics.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/DxeServicesLib.h>

EFI_STATUS
EFIAPI
GetBootGraphic (
  IN  BOOT_GRAPHIC   Graphic,
  OUT UINTN         *ImageSize,
  OUT UINT8        **ImageData)
{
  EFI_GUID *GraphicGuid;

  switch (Graphic) {
    case BG_SYSTEM_LOGO:
      GraphicGuid = PcdGetPtr (PcdLogoFile);
      break;

    case BG_CRITICAL_OVER_TEMP:
      GraphicGuid = PcdGetPtr (PcdThermalFile);
      break;

    case BG_CRITICAL_LOW_BATTERY:
      GraphicGuid = PcdGetPtr (PcdLowBatteryFile);
      break;

    case BG_NO_BOOT_OS:
      GraphicGuid = FixedPcdGetPtr (PcdNoBootOSFile);
      break;
    
    case BG_MSD_DEFAULT:
      GraphicGuid = FixedPcdGetPtr (PcdMsdDefaultFile);
      break;
    
    case BG_MSD_CONNECTED:
      GraphicGuid = FixedPcdGetPtr (PcdMsdConnectedFile);
      break;
    
    case BG_MSD_DISCONNECTED:
      GraphicGuid = FixedPcdGetPtr (PcdMsdDisconnectedFile);
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  // Get the Specified Image from FV
  return GetSectionFromAnyFv (GraphicGuid, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);
}

UINT32
EFIAPI
GetBackgroundColor ()
{
  return 0;
}
