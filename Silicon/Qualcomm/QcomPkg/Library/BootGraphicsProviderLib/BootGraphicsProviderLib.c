/** @file
  This BootGraphicsProviderLib is intended to abstract the source of the
  BMP files from the caller

  This instance uses defined PCDs and RAW FFS files

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/BootGraphics.h>
#include <Library/DxeServicesLib.h>

#include <Pi/PiFirmwareFile.h>

/**
  Get the requested boot graphic
**/
EFI_STATUS
EFIAPI
GetBootGraphic (
  BOOT_GRAPHIC Graphic,
  OUT UINTN   *ImageSize,
  OUT UINT8  **ImageData )
{
  EFI_GUID  *g = NULL;

  switch (Graphic) {
    case BG_SYSTEM_LOGO:
      if (PcdGet32(PcdMipiFrameBufferWidth) <= 720) {
        g = PcdGetPtr (PcdLogoFile_Small);
      } else if (PcdGet32(PcdMipiFrameBufferWidth) >= 1800) {
        g = PcdGetPtr (PcdLogoFile_Big);
      } else {
        g = PcdGetPtr (PcdLogoFile_Medium);
      }

      break;
    case BG_CRITICAL_OVER_TEMP:
      if (PcdGet32(PcdMipiFrameBufferWidth) <= 720) {
        g = PcdGetPtr (PcdThermalFile_Small);
      } else if (PcdGet32(PcdMipiFrameBufferWidth) >= 1800) {
        g = PcdGetPtr (PcdThermalFile_Big);
      } else {
        g = PcdGetPtr (PcdThermalFile_Medium);
      }

      break;
    case BG_CRITICAL_LOW_BATTERY:
      if (PcdGet32(PcdMipiFrameBufferWidth) <= 720) {
        g = PcdGetPtr (PcdLowBatteryFile_Small);
      } else if (PcdGet32(PcdMipiFrameBufferWidth) >= 1800) {
        g = PcdGetPtr (PcdLowBatteryFile_Big);
      } else {
        g = PcdGetPtr (PcdLowBatteryFile_Medium);
      }

      break;
    case BG_NO_BOOT_OS:
      if (PcdGet32(PcdMipiFrameBufferWidth) <= 720) {
        g = PcdGetPtr (PcdNoBootOSFile_Small);
      } else if (PcdGet32(PcdMipiFrameBufferWidth) >= 1800) {
        g = PcdGetPtr (PcdNoBootOSFile_Big);
      } else {
        g = PcdGetPtr (PcdNoBootOSFile_Medium);
      }

      break;
    default:
      DEBUG ((EFI_D_ERROR, "%a: Unsupported Boot Graphic Type! 0x%x\n", __FUNCTION__, Graphic));
      return EFI_UNSUPPORTED;
  }

  //
  // Get the specified image from FV.
  //
  return GetSectionFromAnyFv (g, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);
}

UINT32
EFIAPI
GetBackgroundColor () { return PcdGet32 (PcdPostBackgroundColor); }
