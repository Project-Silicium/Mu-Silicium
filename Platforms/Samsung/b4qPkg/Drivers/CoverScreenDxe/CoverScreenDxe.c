/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  Copyright (c) 2011 - 2018, Intel Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BmpSupportLib.h>
#include <Library/PcdLib.h>

#include <Protocol/EfiCoverScreen.h>
#include <Protocol/EFIDisplayPwrEx.h>

//
// Global Variables
//
STATIC FRAME_BUFFER_CONFIGURE *FrameBufferConfiguration;

EFI_STATUS
GetPicture (
  IN  BOOT_GRAPHIC   Picture,
  OUT UINT8        **PictureData,
  OUT UINTN         *PictureDataSize)
{
  EFI_GUID *PictureGuid;

  switch (Picture) {
    case BG_SYSTEM_LOGO:
      PictureGuid = FixedPcdGetPtr (PcdCoverBootLogoFile);
      break;

    case BG_NO_BOOT_OS:
      PictureGuid = FixedPcdGetPtr (PcdCoverNoBootOSFile);
      break;

    case BG_MSD_SELECT_LUN:
    case BG_MSD_CONNECTED:
    case BG_MSD_DISCONNECTED:
    case BG_MSD_UNKNOWN_STATE:
      PictureGuid = FixedPcdGetPtr (PcdCoverMassStorageFile);
      break;

    default:
      DEBUG ((EFI_D_ERROR, "%a: Unsupported Picture defined!\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
  }

  // Get Specified Picture from FV
  return GetSectionFromAnyFv (PictureGuid, EFI_SECTION_RAW, 0, (VOID **)PictureData, PictureDataSize);
}

EFI_STATUS
DrawPicture (IN BOOT_GRAPHIC Picture)
{
  EFI_STATUS                     Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltData = NULL;
  EFI_TPL                        Tpl;
  UINT8                         *PictureData;
  UINTN                          PictureDataSize;
  UINTN                          PictureWidth;
  UINTN                          PictureHeight;
  UINTN                          BltDataSize;

  // Get defined Picture
  Status = GetPicture (Picture, &PictureData, &PictureDataSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Translate Picture Data to Blt Data
  Status = TranslateBmpToGopBlt (PictureData, PictureDataSize, &BltData, &BltDataSize, &PictureHeight, &PictureWidth);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to Translate Picture Data to BLT Data! Status = %r\n", __FUNCTION__, Status));

    // Free Buffer
    FreePool (PictureData);

    return Status;
  }

  // Set X & Y Position
  UINTN XPos = (FixedPcdGet32 (PcdSecondaryFrameBufferWidth) - PictureWidth) / 2;
  UINTN YPos = (FixedPcdGet32 (PcdSecondaryFrameBufferHeight) - PictureHeight) / 2;

  // Clear Cover Screen
  ZeroMem ((VOID *)0xB8AE0600, 0x00082000);

  // Raise TPL Level
  Tpl = gBS->RaiseTPL (TPL_NOTIFY);

  // Draw Picture
  Status = FrameBufferBlt (FrameBufferConfiguration, BltData, EfiBltBufferToVideo, 0, 0, XPos, YPos, PictureWidth, PictureHeight, PictureWidth * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  // Restore TPL Level
  gBS->RestoreTPL (Tpl);

  // Check for Errors
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Draw defined Picture on the Cover Screen! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Free Buffers
  FreePool (PictureData);
  FreePool (BltData);

  return EFI_SUCCESS;
}

EFI_STATUS
SetPowerState (IN BOOLEAN Enable)
{
  EFI_STATUS                   Status;
  EFI_DISPLAY_PWR_EX_PROTOCOL *mDisplayPwrExProtocol;
  UINT32                       NewPowerState;

  // Locate Display Power Ex Protocol
  Status = gBS->LocateProtocol (&gEfiDisplayPowerStateExProtocolGuid, NULL, (VOID *)&mDisplayPwrExProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Display Power Ex Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set new Power State
  if (Enable) {
    NewPowerState = DISPLAY_POWER_STATE_ON;
  } else {
    NewPowerState = DISPLAY_POWER_STATE_OFF;
  }

  // Set new Cover Screen Power State
  Status = mDisplayPwrExProtocol->SetPowerState (DISPLAY_TYPE_SECONDARY, NewPowerState);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set new Cover Screen Power State! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC EFI_COVER_SCREEN_PROTOCOL mCoverScreen = {
  DrawPicture,
  SetPowerState
};

VOID
EFIAPI
TurnOffCoverScreen (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  // Turn Off Cover Screen
  SetPowerState (FALSE);
}

EFI_STATUS
EFIAPI
InitCoverScreen (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                            Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX       FrameBufferRegion;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *FrameBufferInfos;
  EFI_EVENT                             EbsEvent;

  // Set Default Configuration Size
  STATIC UINTN FrameBufferConfigurationSize = 0;

  // Locate "Display Reserved" Memory Region
  Status = LocateMemoryMapAreaByName ("Display Reserved", &FrameBufferRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'Display Reserved' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Get Cover Screen Infos
  UINT32 Width      = FixedPcdGet32 (PcdSecondaryFrameBufferWidth);
  UINT32 Height     = FixedPcdGet32 (PcdSecondaryFrameBufferHeight);
  UINT8  ColorDepth = FixedPcdGet8  (PcdSecondaryFrameBufferColorDepth);

  // Verify Cover Screen Resolution
  if (!Width || !Height) {
    DEBUG ((EFI_D_ERROR, "Invalid Cover Screen Resolution Defined!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Get Frame Buffer Offset
  UINT64 FrameBufferOffset = FixedPcdGet64 (PcdSecondaryFrameBufferOffset);

  // Verify Frame Buffer Offset
  if (!FrameBufferOffset) {
    DEBUG ((EFI_D_ERROR, "Frame Buffer Offset is not Defined!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Update Frame Buffer Region
  FrameBufferRegion.Address += FrameBufferOffset;
  FrameBufferRegion.Length   = Width * Height * (ColorDepth / 8);

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)&FrameBufferInfos);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Frame Buffer Infos! Status = %r\n", Status));
    return Status;
  }

  // Set Frame Buffer Infos
  FrameBufferInfos->Version              = 0;
  FrameBufferInfos->HorizontalResolution = Width;
  FrameBufferInfos->VerticalResolution   = Height;
  FrameBufferInfos->PixelsPerScanLine    = Width;

  // Set Pixel Format
  switch (ColorDepth) {
    case 32:
      FrameBufferInfos->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
      break;

    case 24:
      FrameBufferInfos->PixelFormat                   = PixelBitMask;
      FrameBufferInfos->PixelInformation.RedMask      = 0x00FF0000;
      FrameBufferInfos->PixelInformation.GreenMask    = 0x0000FF00;
      FrameBufferInfos->PixelInformation.BlueMask     = 0x000000FF;
      FrameBufferInfos->PixelInformation.ReservedMask = 0x00000000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "Color Depth %u is not Supported!\n", ColorDepth));

      // Free Buffer
      gBS->FreePool (FrameBufferInfos);

      return EFI_UNSUPPORTED;
  }

  // Create FrameBufferBltLib Configuration
  Status = FrameBufferBltConfigure ((VOID *)(UINTN)FrameBufferRegion.Address, FrameBufferInfos, FrameBufferConfiguration, &FrameBufferConfigurationSize);
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    // Allocate Memory
    FrameBufferConfiguration = AllocatePool (FrameBufferConfigurationSize);
    if (FrameBufferConfiguration == NULL) {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the Frame Buffer Configuration!\n"));

      // Free Buffer
      gBS->FreePool (FrameBufferInfos);

      return EFI_OUT_OF_RESOURCES;
    }

    // Create FrameBufferBltLib Configuration
    Status = FrameBufferBltConfigure ((VOID *)(UINTN)FrameBufferRegion.Address, FrameBufferInfos, FrameBufferConfiguration, &FrameBufferConfigurationSize);
    if (EFI_ERROR (Status)) {
      goto ConfError;
    }
  } else if (EFI_ERROR (Status)) {
ConfError:
    DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration! Status = %r\n", Status));

    // Free Buffer
    gBS->FreePool (FrameBufferInfos);

    // Free Buffer
    FreePool (FrameBufferConfiguration);

    return Status;
  }

  // Clear Cover Screen
  ZeroMem ((VOID *)FrameBufferRegion.Address, FrameBufferRegion.Length);

  // Register EBS Event
  Status = gBS->CreateEvent (EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_NOTIFY, TurnOffCoverScreen, NULL, &EbsEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register EBS Event!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Register Cover Screen Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiCoverScreenProtocolGuid, EFI_NATIVE_INTERFACE, &mCoverScreen);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Cover Screen Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
