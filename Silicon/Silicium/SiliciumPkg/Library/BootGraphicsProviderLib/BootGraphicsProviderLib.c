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
  BOOT_GRAPHIC   Graphic,
  OUT UINTN     *ImageSize,
  OUT UINT8    **ImageData)
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
    
    case BG_MSD_SELECT_LUN:
      GraphicGuid = FixedPcdGetPtr (PcdMsdSelectLunFile);
      break;
    
    case BG_MSD_CONNECTED:
      GraphicGuid = FixedPcdGetPtr (PcdMsdConnectedFile);
      break;
    
    case BG_MSD_DISCONNECTED:
      GraphicGuid = FixedPcdGetPtr (PcdMsdDisconnectedFile);
      break;

    case BG_MSD_UNKNOWN_STATE:
      GraphicGuid = FixedPcdGetPtr (PcdMsdUnknownStateFile);
      break;

    default:
      DEBUG ((EFI_D_ERROR, "%a: Unsupported Boot Graphic Type! Got %u\n", __FUNCTION__, Graphic));
      return EFI_UNSUPPORTED;
  }

  // Get the Specified Image from FV
  return GetSectionFromAnyFv (GraphicGuid, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);
}

UINT32
EFIAPI
GetBackgroundColor ()
{
  return PcdGet32 (PcdPostBackgroundColor);
}
