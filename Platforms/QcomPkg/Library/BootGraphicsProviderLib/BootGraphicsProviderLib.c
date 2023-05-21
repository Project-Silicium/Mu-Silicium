/** @file
  This BootGraphicsProviderLib is intended to abstract the source of the
  BMP files from the caller

  This instance uses defined PCDs and RAW FFS files

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Pi/PiFirmwareFile.h>

#include <Library/BootGraphicsProviderLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>

#include <Library/BgraRgbaConvert.h>
#include <Library/BmpSupportLib.h>
#include <Library/lodepng.h>

/**
  Get the requested boot graphic

**/
EFI_STATUS
EFIAPI
GetBootGraphic(
    BOOT_GRAPHIC Graphic, OUT UINTN *ImageSize, OUT UINT8 **ImageData)
{
  EFI_GUID      *g = NULL;
  unsigned char *DecodedData;
  unsigned int   Width;
  unsigned int   Height;
  UINT32         DecoderError;
  BOOLEAN        IsPng  = FALSE;
  EFI_STATUS     Status = EFI_SUCCESS;

  switch (Graphic) {
  case BG_SYSTEM_LOGO:
    g     = PcdGetPtr(PcdLogoFile);
    IsPng = TRUE;
    break;
  case BG_CRITICAL_OVER_TEMP:
    g = PcdGetPtr(PcdThermalFile);
    break;
  case BG_CRITICAL_LOW_BATTERY:
    g = PcdGetPtr(PcdLowBatteryFile);
    break;
  default:
    DEBUG((DEBUG_ERROR, "Unsupported Boot Graphic Type 0x%X\n", Graphic));
    return EFI_UNSUPPORTED;
  }

  //
  // Get the specified image from FV.
  //
  Status =
      GetSectionFromAnyFv(g, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);

  if (!EFI_ERROR(Status) && IsPng) {
    // Decode PNG
    DecoderError = lodepng_decode32(
        &DecodedData, &Width, &Height, (unsigned char *)*ImageData,
        (size_t)*ImageSize);

    if (!DecoderError) {
      ConvertBetweenBGRAandRGBA(DecodedData, Width, Height);

      //
      // Convert GOP Blt buffer to BMP image.  Pass in ImageBuffer set to NULL
      // so the BMP image is allocated by TranslateGopBltToBmp().
      //
      *ImageData = NULL;
      *ImageSize = 0;

      Status = TranslateGopBltToBmp(
          (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)DecodedData, (UINT32)Height,
          (UINT32)Width, (VOID **)ImageData, (UINT32 *)ImageSize);
    }
    else {
      return EFI_UNSUPPORTED;
    }
  }

  return Status;
}

UINT32
EFIAPI
GetBackgroundColor() { return PcdGet32(PcdPostBackgroundColor); }
