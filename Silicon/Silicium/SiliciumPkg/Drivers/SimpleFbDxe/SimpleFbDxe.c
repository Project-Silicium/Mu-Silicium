#include <Library/DebugLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include <Configuration/BootDevices.h>

#include "SimpleFb.h"

//
// Global Variables
//
STATIC FRAME_BUFFER_CONFIGURE *FrameBufferConfiguration;
STATIC EFI_DISPLAY_DETAILS     DisplayDetails;

EFI_STATUS
DisplayQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info)
{
  EFI_STATUS Status;

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)Info);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for GOP Info! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Pass Infos
  *SizeOfInfo                   = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  (*Info)->Version              = This->Mode->Info->Version;
  (*Info)->HorizontalResolution = This->Mode->Info->HorizontalResolution;
  (*Info)->VerticalResolution   = This->Mode->Info->VerticalResolution;
  (*Info)->PixelFormat          = This->Mode->Info->PixelFormat;
  (*Info)->PixelsPerScanLine    = This->Mode->Info->PixelsPerScanLine;

  return EFI_SUCCESS;
}

EFI_STATUS
DisplaySetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN UINT32                        ModeNumber)
{
  return EFI_SUCCESS;
}

EFI_STATUS
DisplayBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL      *This,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *BltBuffer,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation OPTIONAL,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta OPTIONAL)
{
  EFI_STATUS Status;
  EFI_TPL    Tpl;

  // Raise TPL Level
  Tpl = gBS->RaiseTPL (TPL_NOTIFY);

  // Draw to Frame Buffer
  Status = FrameBufferBlt (FrameBufferConfiguration, BltBuffer, BltOperation, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);

  // Restore TPL Level
  gBS->RestoreTPL (Tpl);

  return Status;
}

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL mDisplay = {
  DisplayQueryMode,
  DisplaySetMode,
  DisplayBlt,
  NULL
};

EFI_STATUS
GetDisplayDetails ()
{
  EFI_STATUS Status;

  // Clear Display Details
  ZeroMem (&DisplayDetails, sizeof (EFI_DISPLAY_DETAILS));

  // Locate "Display Reserved" Memory Region
  Status  = LocateMemoryMapAreaByName ("Display Reserved", &DisplayDetails.MemoryRegion);
  Status |= LocateMemoryMapAreaByName ("Display_Reserved", &DisplayDetails.MemoryRegion);
  if (EFI_ERROR (Status) && !DisplayDetails.MemoryRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'Display Reserved' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Get Display Resolution
  DisplayDetails.Width  = FixedPcdGet32 (PcdFrameBufferWidth);
  DisplayDetails.Height = FixedPcdGet32 (PcdFrameBufferHeight);

  // Get Display Color Depth
  DisplayDetails.ColorDepth = FixedPcdGet32 (PcdFrameBufferColorDepth);

  // Check Display Resolution
  if (DisplayDetails.Width < 640 || DisplayDetails.Height < 475) {
    DEBUG ((EFI_D_ERROR, "The Display Resolution is too Small!\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AllocateGopMemory ()
{
  EFI_STATUS Status;

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE), (VOID *)&mDisplay.Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Mode! Status = %r\n", Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (mDisplay.Mode, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)&mDisplay.Mode->Info);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Info! Status = %r\n", Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (mDisplay.Mode->Info, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

  return EFI_SUCCESS;
}

EFI_STATUS
CreateFrameBufferConfig ()
{
  EFI_STATUS Status;

  // Set Configuration Size
  STATIC UINTN FrameBufferConfigurationSize = 0;

  // Get Frame Buffer Configuration Size
  Status = FrameBufferBltConfigure ((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, FrameBufferConfiguration, &FrameBufferConfigurationSize);
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    // Allocate Memory
    FrameBufferConfiguration = AllocatePool (FrameBufferConfigurationSize);
    if (FrameBufferConfiguration == NULL) {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the Frame Buffer Configuration!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    // Create FrameBufferBltLib Configuration
    Status = FrameBufferBltConfigure ((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, FrameBufferConfiguration, &FrameBufferConfigurationSize);
  }

  // Check for Errors
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration! Status = %r\n", Status));

    // Free Buffer
    FreePool (FrameBufferConfiguration);

    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RegisterDisplays (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Display Details
  Status = GetDisplayDetails ();
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Allocate Memory
  Status = AllocateGopMemory ();
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Set Mode Information
  mDisplay.Mode->MaxMode         = 1;
  mDisplay.Mode->Mode            = 0;
  mDisplay.Mode->SizeOfInfo      = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  mDisplay.Mode->FrameBufferBase = DisplayDetails.MemoryRegion.Address;
  mDisplay.Mode->FrameBufferSize = DisplayDetails.Width * DisplayDetails.Height * (DisplayDetails.ColorDepth / 8);

  // Set Display Infos
  mDisplay.Mode->Info->Version              = 0;
  mDisplay.Mode->Info->HorizontalResolution = DisplayDetails.Width;
  mDisplay.Mode->Info->VerticalResolution   = DisplayDetails.Height;
  mDisplay.Mode->Info->PixelsPerScanLine    = DisplayDetails.Width;

  // Set Pixel Format
  switch (DisplayDetails.ColorDepth) {
    case 32:
      mDisplay.Mode->Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
      break;

    case 24:
      mDisplay.Mode->Info->PixelFormat                   = PixelBitMask;
      mDisplay.Mode->Info->PixelInformation.RedMask      = 0x00FF0000;
      mDisplay.Mode->Info->PixelInformation.GreenMask    = 0x0000FF00;
      mDisplay.Mode->Info->PixelInformation.BlueMask     = 0x000000FF;
      mDisplay.Mode->Info->PixelInformation.ReservedMask = 0x00000000;
      break;

    case 16:
      mDisplay.Mode->Info->PixelFormat                   = PixelBitMask;
      mDisplay.Mode->Info->PixelInformation.RedMask      = 0x0000F800;
      mDisplay.Mode->Info->PixelInformation.GreenMask    = 0x000007E0;
      mDisplay.Mode->Info->PixelInformation.BlueMask     = 0x0000001F;
      mDisplay.Mode->Info->PixelInformation.ReservedMask = 0x00000000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "Color Depth %u is not Supported!\n", DisplayDetails.ColorDepth));
      goto cleanup;
  }

  // Create Frame Buffer Config
  Status = CreateFrameBufferConfig ();
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Register GOP Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiDevicePathProtocolGuid, &DisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mDisplay, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GOP Protocol of the Display!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  goto exit;

cleanup:
  // Free Buffer
  if (mDisplay.Mode->Info != NULL) {
    gBS->FreePool (mDisplay.Mode->Info);
  }

  // Free Buffer
  if (mDisplay.Mode != NULL) {
    gBS->FreePool (mDisplay.Mode);
  }

exit:
  return Status;
}
