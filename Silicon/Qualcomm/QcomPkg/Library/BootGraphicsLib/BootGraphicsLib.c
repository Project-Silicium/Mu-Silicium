/** @file
  This BootGraphicsLib  is only intended to be used by BDS to draw
  and the main boot graphics to the screen.

  Implementation borrows from Edk2 BootLogoLib

  Copyright (c) 2011 - 2018, Intel Corporation. All rights reserved.<BR>
  Copyright (C) Microsoft Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/DisplayDeviceStateLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/UefiLib.h>

#include <Protocol/GraphicsOutput.h>
#include <Protocol/BootLogo2.h>

#define MS_MAX_HEIGHT_PERCENTAGE  40
#define MS_MAX_WIDTH_PERCENTAGE   40

EFI_STATUS
EFIAPI
DisplayBootGraphic (BOOT_GRAPHIC Graphic)
{
  EFI_STATUS                     Status         = EFI_SUCCESS;
  UINTN                          Height         = 0;
  UINTN                          Width          = 0;
  UINT32                         SizeOfX        = 0;
  UINT32                         SizeOfY        = 0;
  INTN                           DestX          = 0;
  INTN                           DestY          = 0;
  UINT8                         *ImageData      = NULL;
  UINTN                          ImageSize      = 0;
  UINT32                         Color          = 0;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt            = NULL;
  UINTN                          BltSize        = 0;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput = NULL;
  EDKII_BOOT_LOGO2_PROTOCOL     *BootLogo2      = NULL;

  //
  // Try to open GOP first
  //
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID **)&GraphicsOutput);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Handle Protocol! (1st Try) Status = %r\n", __FUNCTION__, Status));

    // failed on console out.  Try globally within system
    Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&GraphicsOutput);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Handle Protocol! (2nd Try) Status = %r\n", __FUNCTION__, Status));
      goto CleanUp;
    }
  }

  //
  // Try to open Boot Logo 2 Protocol.
  //
  Status = gBS->LocateProtocol (&gEdkiiBootLogo2ProtocolGuid, NULL, (VOID **)&BootLogo2);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate BootLogo2 Protocol! Status = %r\n", __FUNCTION__, Status));
  }

  //
  // Erase Cursor from screen
  //
  if (gST->ConOut != NULL) {
    gST->ConOut->EnableCursor (gST->ConOut, FALSE);
  }

  SizeOfX = GraphicsOutput->Mode->Info->HorizontalResolution;
  SizeOfY = GraphicsOutput->Mode->Info->VerticalResolution;

  // allow for custom bg color
  Color = GetBackgroundColor ();

  // Color The Background
  Blt = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(&Color); // only pixel (0,0) is used for EfiBltVideoFill

  GraphicsOutput->Blt (GraphicsOutput, Blt, EfiBltVideoFill, 0, 0, 0, 0, SizeOfX, SizeOfY, 0);

  Blt = NULL;

  // Draw our device state
  DisplayDeviceState ((INT32)SizeOfX, (INT32)SizeOfY);

  Status = GetBootGraphic (Graphic, &ImageSize, &ImageData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed Get Boot Graphic! Status = %r\n", __FUNCTION__, Status));
    goto CleanUp;
  }

  //
  // Convert Bmp To Blt Buffer
  //
  Status = TranslateBmpToGopBlt (ImageData, ImageSize, &Blt, &BltSize, &Height, &Width);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Translate BMP to Blt! Status = %r\n", __FUNCTION__, Status));
    goto CleanUp;
  }

  // If system logo it must meet size requirements.
  if (Graphic == BG_SYSTEM_LOGO) {
    // check if the image is appropriate size as per data defined in the windows engineering guide.
    if ((Width > ((SizeOfX * MS_MAX_WIDTH_PERCENTAGE) / 100)) || (Height > ((SizeOfY * MS_MAX_HEIGHT_PERCENTAGE) / 100))) {
      Status = EFI_INVALID_PARAMETER;
      goto CleanUp;
    }
  }

  DestX = (SizeOfX - Width) / 2;
  DestY = (SizeOfY - Height) / 2;

  // Blt to screen
  if ((DestX >= 0) && (DestY >= 0)) {
    Status = GraphicsOutput->Blt (GraphicsOutput, Blt, EfiBltBufferToVideo, 0, 0, (UINTN)DestX, (UINTN)DestY, Width, Height, Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Graphic! Status = %r\n", __FUNCTION__, Status));
      goto CleanUp;
    }
  } else {
    goto CleanUp;
  }

  //
  // Attempt to register logo with Boot Logo 2 Protocol
  //
  if ((Graphic == BG_SYSTEM_LOGO) && (BootLogo2 != NULL)) {
    Status = BootLogo2->SetBootLogo (BootLogo2, Blt, DestX, DestY, Width, Height);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Set Boot Logo in BootLogo2 Protocol! Status = %r\n", __FUNCTION__, Status));
    }
  }

  // Signals that boot graphics has been displayed
  EfiEventGroupSignal (&gLogoDisplayedEventGroup);

  //
  // Status of this function is EFI_SUCCESS even if registration with Boot
  // Logo 2 Protocol fails.
  //
  Status = EFI_SUCCESS;

CleanUp:
  if (Blt != NULL) {
    FreePool (Blt);
  }

  if (ImageData != NULL) {
    FreePool (ImageData);
  }

  return Status;
}
