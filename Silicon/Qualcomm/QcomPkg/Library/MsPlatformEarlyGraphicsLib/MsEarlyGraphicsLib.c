/**
  Dxe Ms Early Graphics Library Instance.

  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MsPlatformEarlyGraphicsLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

BOOLEAN  IsInitialized            = FALSE;
EFI_GUID gGopOverrideProtocolGuid = { 0xBE8EE323, 0x184C, 0x4E24, { 0x8E, 0x18, 0x2E, 0x6D, 0xAD, 0xD7, 0x01, 0x60 } };

/**
  This Function gets the Frame Buffer Infos from the Device.

  @param GraphicsMode            - Pointer to receive GOP_MODE structure

  @return Status                 - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsEarlyGraphicsGetFrameBufferInfo (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE **GraphicsMode)
{
  EFI_STATUS                            Status;
  UINT32                                ModeNumber;
  UINT32                                CurrentModeNumber;
  UINT32                                CurrentHorizontalResolution;
  UINT32                                CurrentVerticalResolution;
  UINT32                                MaxMode;
  UINT32                                NewHorizontalResolution;
  UINT32                                NewVerticalResolution;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         *Gop;

  if (!GraphicsMode) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsInitialized) {
    return EFI_SUCCESS;
  }

  *GraphicsMode = (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *)AllocateZeroPool(sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));

  if (!*GraphicsMode || ((*GraphicsMode)->Info = (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *)AllocateZeroPool(sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION))) == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Out of Resources!\n", __FUNCTION__));
    Status = EFI_OUT_OF_RESOURCES;
    goto exit;
  }

  // Locate GOP Override Protocol
  Status = gBS->LocateProtocol (&gGopOverrideProtocolGuid, NULL, (VOID *)&Gop);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Override Protocol! Status = %r\n", __FUNCTION__, Status));
    goto exit;
  }

  if (Gop->Mode->Mode != -1) {
    goto LABEL_33;
  }

  ModeNumber                  = 0;
  CurrentModeNumber           = 0;
  CurrentHorizontalResolution = 0;
  CurrentVerticalResolution   = 0;
  MaxMode                     = Gop->Mode->MaxMode;

  while (ModeNumber != MaxMode) {
    Status = Gop->QueryMode (Gop, ModeNumber, &SizeOfInfo, &Info);
    if (EFI_ERROR (Status)) {
      goto LABEL_21;
    }

    NewHorizontalResolution = Info->HorizontalResolution;

    if (NewHorizontalResolution <= CurrentHorizontalResolution) {
      if (NewHorizontalResolution != CurrentHorizontalResolution) {
        goto LABEL_20;
      }

      NewVerticalResolution = Info->VerticalResolution;

      if (NewVerticalResolution <= CurrentVerticalResolution) {
        goto LABEL_20;
      }
    } else {
      NewVerticalResolution = Info->VerticalResolution;
    }

    CurrentModeNumber           = ModeNumber;
    CurrentHorizontalResolution = Info->HorizontalResolution;
    CurrentVerticalResolution   = NewVerticalResolution;

  LABEL_20:
    FreePool(Info);

  LABEL_21:
    ++ModeNumber;
  }

  if (CurrentModeNumber != Gop->Mode->Mode) {
    Status = Gop->SetMode (Gop, CurrentModeNumber);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to set GOP Mode! Status = %r\n", __FUNCTION__, Status));
    }
  }

LABEL_33:
  (*GraphicsMode)->MaxMode                    = Gop->Mode->MaxMode;
  (*GraphicsMode)->Mode                       = Gop->Mode->Mode;
  (*GraphicsMode)->SizeOfInfo                 = Gop->Mode->SizeOfInfo;
  (*GraphicsMode)->FrameBufferSize            = Gop->Mode->FrameBufferSize;
  (*GraphicsMode)->FrameBufferBase            = Gop->Mode->FrameBufferBase;
  (*GraphicsMode)->Info->Version              = Gop->Mode->Info->Version;
  (*GraphicsMode)->Info->HorizontalResolution = Gop->Mode->Info->HorizontalResolution;
  (*GraphicsMode)->Info->VerticalResolution   = Gop->Mode->Info->VerticalResolution;
  (*GraphicsMode)->Info->PixelFormat          = Gop->Mode->Info->PixelFormat;
  (*GraphicsMode)->Info->PixelInformation     = Gop->Mode->Info->PixelInformation;
  (*GraphicsMode)->Info->PixelsPerScanLine    = Gop->Mode->Info->PixelsPerScanLine;

  IsInitialized = 1;

exit:
  return Status;
}
