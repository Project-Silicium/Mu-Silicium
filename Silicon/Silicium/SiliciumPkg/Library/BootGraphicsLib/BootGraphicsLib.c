/**
  Copyright (c) 2011 - 2018, Intel Corporation. All rights reserved.
  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/BootGraphics.h>
#include <Library/UefiLib.h>

#include <Protocol/EfiCoverScreen.h>
#include <Protocol/BootLogo2.h>

EFI_STATUS
EFIAPI
DisplayBootGraphic (IN BOOT_GRAPHIC Graphic)
{
  EFI_STATUS                     Status               = EFI_SUCCESS;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *mGopProtocol         = NULL;
  EFI_COVER_SCREEN_PROTOCOL     *mCoverScreenProtocol = NULL;
  EDKII_BOOT_LOGO2_PROTOCOL     *mBootLogoProtocol    = NULL;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer            = NULL;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  Color                = {0};
  UINTN                          BltBufferSize        = 0;
  UINTN                          PictureHeight        = 0;
  UINTN                          PictureWidth         = 0;
  UINT8                         *ImageData            = NULL;
  UINTN                          ImageSize            = 0;

  // Locate GOP Protocol
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&mGopProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate Cover Screen Protocol
  Status = gBS->LocateProtocol (&gEfiCoverScreenProtocolGuid, NULL, (VOID *)&mCoverScreenProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Cover Screen Protocol! Status = %r\n", __FUNCTION__, Status));
  }

  // Locate Boot Logo 2 Protocol
  Status = gBS->LocateProtocol (&gEdkiiBootLogo2ProtocolGuid, NULL, (VOID *)&mBootLogoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Locate Boot Logo 2 Protocol! Status = %r\n", __FUNCTION__, Status));
  }

  // Set Screen Width & Height
  UINT32 ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  // Get Specified Picture
  Status = GetBootGraphic (Graphic, &ImageSize, &ImageData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Specified Picture! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Translate Picture
  Status = TranslateBmpToGopBlt (ImageData, ImageSize, &BltBuffer, &BltBufferSize, &PictureHeight, &PictureWidth);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Translate Picture Data! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set Position
  UINTN XPos = (ScreenWidth - PictureWidth) / 2;
  UINTN YPos = (ScreenHeight - PictureHeight) / 2;

  // Clear Screen
  mGopProtocol->Blt (mGopProtocol, &Color, EfiBltVideoFill, 0, 0, 0, 0, ScreenWidth, ScreenHeight, 0);

  // Draw Picture on Screen
  mGopProtocol->Blt (mGopProtocol, BltBuffer, EfiBltBufferToVideo, 0, 0, XPos, YPos, PictureWidth, PictureHeight, 0);

  // Save Boot Logo
  if (Graphic == BG_SYSTEM_LOGO && mBootLogoProtocol != NULL) {
    Status = mBootLogoProtocol->SetBootLogo (mBootLogoProtocol, BltBuffer, XPos, YPos, PictureWidth, PictureHeight);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Save Boot Logo! Status = %r\n", __FUNCTION__, Status));
    }
  }

  // Free Buffers
  FreePool (BltBuffer);
  FreePool (ImageData);

  if (mCoverScreenProtocol != NULL) {
    // Get Cover Screen Resolution
    mCoverScreenProtocol->GetResolution (&ScreenWidth, &ScreenHeight);

    // Get Cover Screen Picture
    Status = GetCoverBootGraphic (Graphic, &ImageSize, &ImageData);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to get Specified Cover Picture! Status = %r\n", __FUNCTION__, Status));
      return EFI_SUCCESS;
    }

    // Translate Picture
    Status = TranslateBmpToGopBlt (ImageData, ImageSize, &BltBuffer, &BltBufferSize, &PictureHeight, &PictureWidth);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Translate Picture Data! Status = %r\n", __FUNCTION__, Status));
      return EFI_SUCCESS;
    }

    // Set Position
    UINTN XPos = (ScreenWidth - PictureWidth) / 2;
    UINTN YPos = (ScreenHeight - PictureHeight) / 2;

    // Clear Cover Screen
    mCoverScreenProtocol->Blt (&Color, EfiBltVideoFill, 0, 0, 0, 0, ScreenWidth, ScreenHeight, 0);

    // Draw Picture on Screen
    mCoverScreenProtocol->Blt (BltBuffer, EfiBltBufferToVideo, 0, 0, XPos, YPos, PictureWidth, PictureHeight, 0);

    // Free Buffers
    FreePool (BltBuffer);
    FreePool (ImageData);
  }

  return EFI_SUCCESS;
}
