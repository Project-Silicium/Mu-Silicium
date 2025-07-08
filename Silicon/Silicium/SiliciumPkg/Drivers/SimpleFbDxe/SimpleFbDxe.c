#include <Library/DebugLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include <Configuration/BootDevices.h>

#include <Protocol/GraphicsOutput.h>

//
// Global Variables
//
STATIC FRAME_BUFFER_CONFIGURE *FrameBufferConfiguration;

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
InitDisplay (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN UINT32               Width,
  IN UINT32               Height,
  IN UINT32               ColorDepth)
{
  EFI_STATUS Status;

  // Reset Configuration Size
  STATIC UINTN FrameBufferConfigurationSize = 0;

  // Check Frame Buffer Address
  if (!Address) {
    DEBUG ((EFI_D_ERROR, "The Frame Buffer Base Address is Invalid!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Check Frame Buffer Resolution
  if (!Width || !Height) {
    DEBUG ((EFI_D_ERROR, "The Frame Buffer Resolution is Invalid of 0x%llx!\n", Address));
    return EFI_INVALID_PARAMETER;
  }

  // Free Buffers
  if (mDisplay.Mode != NULL) {
    if (mDisplay.Mode->Info != NULL) {
      // Free GOP Mode Info Buffer
      gBS->FreePool (mDisplay.Mode->Info);
    }

    // Free GOP Mode Buffer
    gBS->FreePool (mDisplay.Mode);
  }

  // Alocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE), (VOID *)&mDisplay.Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Mode of 0x%llx! Status = %r\n", Address, Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (mDisplay.Mode, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)&mDisplay.Mode->Info);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Mode Info of 0x%llx! Status = %r\n", Address, Status));

    // Free Buffer
    gBS->FreePool (mDisplay.Mode);

    return Status;
  }

  // Clear Memory
  ZeroMem (mDisplay.Mode->Info, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

  // Set Frame Buffer Infos
  mDisplay.Mode->MaxMode                                 = 1;
  mDisplay.Mode->Mode                                    = 0;
  mDisplay.Mode->SizeOfInfo                              = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  mDisplay.Mode->FrameBufferBase                         = Address;
  mDisplay.Mode->FrameBufferSize                         = Width * Height * (ColorDepth / 8);
  mDisplay.Mode->Info->Version                           = 0;
  mDisplay.Mode->Info->HorizontalResolution              = Width;
  mDisplay.Mode->Info->VerticalResolution                = Height;
  mDisplay.Mode->Info->PixelsPerScanLine                 = Width;

  // Set Pixel Format
  switch (ColorDepth) {
    case 32:
      mDisplay.Mode->Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
      break;

    case 24:
      mDisplay.Mode->Info->PixelFormat                     = PixelBitMask;
      mDisplay.Mode->Info->PixelInformation.RedMask        = 0x00FF0000;
      mDisplay.Mode->Info->PixelInformation.GreenMask      = 0x0000FF00;
      mDisplay.Mode->Info->PixelInformation.BlueMask       = 0x000000FF;
      mDisplay.Mode->Info->PixelInformation.ReservedMask   = 0x00000000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "Color Depth %u is not Supported for 0x%llx!\n", ColorDepth, Address));

      // Free Buffers
      gBS->FreePool (mDisplay.Mode->Info);
      gBS->FreePool (mDisplay.Mode);

      return EFI_UNSUPPORTED;
  }

  // Create FrameBufferBltLib Configuration
  Status = FrameBufferBltConfigure ((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, FrameBufferConfiguration, &FrameBufferConfigurationSize);
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    // Allocate Memory
    FrameBufferConfiguration = AllocatePool (FrameBufferConfigurationSize);
    if (FrameBufferConfiguration == NULL) {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the Frame Buffer Configuration of 0x%llx!\n", Address));

      // Free Buffers
      gBS->FreePool (mDisplay.Mode->Info);
      gBS->FreePool (mDisplay.Mode);

      return EFI_OUT_OF_RESOURCES;
    }

    // Create FrameBufferBltLib Configuration
    Status = FrameBufferBltConfigure ((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, FrameBufferConfiguration, &FrameBufferConfigurationSize);
    if (EFI_ERROR (Status)) {
      goto ConfError;
    }
  } else if (EFI_ERROR (Status)) {
ConfError:
    DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration of 0x%llx! Status = %r\n", Address, Status));

    // Free Buffers
    gBS->FreePool (mDisplay.Mode->Info);
    gBS->FreePool (mDisplay.Mode);

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
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegion;
  UINT32                          Width;
  UINT32                          Height;
  UINT32                          ColorDepth;

  // Locate "Display Reserved" Memory Region
  Status  = LocateMemoryMapAreaByName ("Display Reserved", &DisplayMemoryRegion);
  Status |= LocateMemoryMapAreaByName ("Display_Reserved", &DisplayMemoryRegion);
  if (EFI_ERROR (Status) && !DisplayMemoryRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'Display Reserved' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Get Frame Buffer Infos
  Width      = FixedPcdGet32 (PcdPrimaryFrameBufferWidth);
  Height     = FixedPcdGet32 (PcdPrimaryFrameBufferHeight);
  ColorDepth = FixedPcdGet32 (PcdPrimaryFrameBufferColorDepth);

  // Init Primary Display
  Status = InitDisplay (DisplayMemoryRegion.Address, Width, Height, ColorDepth);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Register GOP Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiDevicePathProtocolGuid, &DisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mDisplay, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GOP Protocol of the Primary Display!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((EFI_D_WARN, "Successfully Init Primary Display\n"));

  return EFI_SUCCESS;
}
