/** @file
  Dxe Ms Early Graphics Library instance

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/MsPlatformEarlyGraphicsLib.h>

#include <Library/UefiLib.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

BOOLEAN  IsInitialized            = FALSE;
EFI_GUID gGopOverrideProtocolGuid = {
    0xBE8EE323,
    0x184C,
    0x4E24,
    {0x8E, 0x18, 0x2E, 0x6D, 0xAD, 0xD7, 0x01, 0x60}};

/**
 * GetFrameBufferInfo - PEI - Only needs to be called once.
 * GetFrameBufferInfo - DXE - Must be called for every new upper layer call
 *                            as the frame buffer address will change after
 *                            PciBus scan
 *   In PEI mode, this Library is used to get the GOP_MODE or guild
 *   a GOP_MODE from the graphics adapter interface.
 *   In DXE mode, this Library is used to update the FrameBuffer Base.
 *
 *   The MsEarlyGraphics driver abstracts the graphics adapter though a portion
 *   of the GOP protocol - that is, the framebuffer address and the mode
 * information
 *
 *
 * @param GraphicsMode            - Pointer to receive GOP_MODE structure
 *
 * @return EFI_SUCCESS            - FrameBufferInfo returned
 * @return EFI_INVALID_PARAMETERS - GraphicsMide is NULL
 * @return EFI_NOT_FOUND          - Underlying support of early graphics not
 * found
 */
EFI_STATUS
EFIAPI
MsEarlyGraphicsGetFrameBufferInfo(
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE **GraphicsMode)
{
  EFI_STATUS                            Status = EFI_OUT_OF_RESOURCES;
  UINT32                                ModeNumber;
  UINT32                                CurrentModeNumber;
  UINT32                                CurrentHorizontalResolution;
  UINT32                                CurrentVerticalResolution;
  INT32                                 MaxMode;
  UINT32                                NewHorizontalResolution;
  UINT32                                NewVerticalResolution;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         *Gop = NULL;

  if (!GraphicsMode) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsInitialized) {
    return EFI_SUCCESS;
  }

  *GraphicsMode = (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *)AllocateZeroPool(
      sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  if (!*GraphicsMode ||
      ((*GraphicsMode)->Info =
           (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *)AllocateZeroPool(
               sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION))) == NULL) {
    DEBUG((EFI_D_ERROR, "%a out of resources\n", __FUNCTION__));
    return Status;
  }

  Status = gBS->LocateProtocol(&gGopOverrideProtocolGuid, NULL, (VOID *)&Gop);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Unable to locate GOP. Code=%r.\n", Status));
    return Status;
  }

  if (Gop->Mode->Mode != -1) {
    goto LABEL_33;
  }

  DEBUG((EFI_D_INFO, "GOP mode is invalid. Attempting to set mode.\n"));

  ModeNumber                  = 0;
  CurrentModeNumber           = 0;
  CurrentHorizontalResolution = 0;
  CurrentVerticalResolution   = 0;
  MaxMode                     = Gop->Mode->MaxMode;

  while (ModeNumber != MaxMode) {
    Status = Gop->QueryMode(Gop, ModeNumber, &SizeOfInfo, &Info);
    if (EFI_ERROR(Status)) {
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
    }
    else {
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

  if (!CurrentHorizontalResolution || !CurrentVerticalResolution) {
    DEBUG((EFI_D_ERROR, "No valid GOP mode found!\n"));
  }

  if (CurrentModeNumber != Gop->Mode->Mode) {
    Status = Gop->SetMode(Gop, CurrentModeNumber);
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "Failed to set GOP mode!\n"));
    }
  }

LABEL_33:
  (*GraphicsMode)->MaxMode         = Gop->Mode->MaxMode;
  (*GraphicsMode)->Mode            = Gop->Mode->Mode;
  (*GraphicsMode)->SizeOfInfo      = Gop->Mode->SizeOfInfo;
  (*GraphicsMode)->FrameBufferSize = Gop->Mode->FrameBufferSize;
  (*GraphicsMode)->FrameBufferBase = Gop->Mode->FrameBufferBase;
  (*GraphicsMode)->Info->Version   = Gop->Mode->Info->Version;
  (*GraphicsMode)->Info->HorizontalResolution =
      Gop->Mode->Info->HorizontalResolution;
  (*GraphicsMode)->Info->VerticalResolution =
      Gop->Mode->Info->VerticalResolution;
  (*GraphicsMode)->Info->PixelFormat       = Gop->Mode->Info->PixelFormat;
  (*GraphicsMode)->Info->PixelInformation  = Gop->Mode->Info->PixelInformation;
  (*GraphicsMode)->Info->PixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;

  DEBUG((EFI_D_INFO, "MaxMode:0x%x \n", (UINT32)(*GraphicsMode)->MaxMode));

  DEBUG((EFI_D_INFO, "Mode:0x%x \n", (UINT32)(*GraphicsMode)->Mode));

  DEBUG((EFI_D_INFO, "SizeOfInfo:0x%x \n", (*GraphicsMode)->SizeOfInfo));

  DEBUG((
      EFI_D_INFO, "FrameBufferBase:0x%x \n", (*GraphicsMode)->FrameBufferBase));

  DEBUG((
      EFI_D_INFO, "FrameBufferSize:0x%x \n", (*GraphicsMode)->FrameBufferSize));

  DEBUG((EFI_D_INFO, "Version:0x%x \n", (*GraphicsMode)->Info->Version));

  DEBUG(
      (EFI_D_INFO, "HorizontalResolution:0x%x \n",
       (UINT32)(*GraphicsMode)->Info->HorizontalResolution));

  DEBUG(
      (EFI_D_INFO, "VerticalResolution:0x%x \n",
       (UINT32)(*GraphicsMode)->Info->VerticalResolution));

  DEBUG(
      (EFI_D_INFO, "PixelFormat:0x%x \n",
       (UINT32)(*GraphicsMode)->Info->PixelFormat));

  DEBUG(
      (EFI_D_INFO, "PixelsPerScanLine:0x%x \n",
       (UINT32)(*GraphicsMode)->Info->PixelsPerScanLine));

  if ((*GraphicsMode)->Mode == -1) {
    DEBUG((
        EFI_D_INFO, "GOP has invalid mode. Will attempt init on next call.\n"));
  }
  else {
    IsInitialized = 1;
  }

  return Status;
}
