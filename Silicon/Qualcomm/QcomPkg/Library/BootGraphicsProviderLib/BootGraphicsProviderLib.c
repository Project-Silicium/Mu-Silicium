/** @file
  This BootGraphicsProviderLib is intended to abstract the source of the
  BMP files from the caller

  This instance uses defined PCDs and RAW FFS files

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Pi/PiFirmwareFile.h>

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/BootGraphics.h>
#include <Library/DxeServicesLib.h>

/**
  Get the requested boot graphic

**/
EFI_STATUS
EFIAPI
GetBootGraphic (
  BOOT_GRAPHIC  Graphic,
  OUT UINTN     *ImageSize,
  OUT UINT8     **ImageData
  )
{
  EFI_GUID  *g = NULL;

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
    default:
      DEBUG ((DEBUG_ERROR, "Unsupported Boot Graphic Type 0x%X\n", Graphic));
      return EFI_UNSUPPORTED;
  }

  //
  // Get the specified image from FV.
  //
  return GetSectionFromAnyFv (g, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);
}

UINT32
EFIAPI
GetBackgroundColor (
  )
{
  return PcdGet32 (PcdPostBackgroundColor);
}