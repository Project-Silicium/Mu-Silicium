#include <Library/DebugLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include <Configuration/BootDevices.h>

#include <Protocol/EfiCoverScreen.h>

//
// Global Variables
//
STATIC FRAME_BUFFER_CONFIGURE *PrimaryFrameBufferConfiguration;
STATIC FRAME_BUFFER_CONFIGURE *SecondaryFrameBufferConfiguration;

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
  EFI_STATUS              Status;
  FRAME_BUFFER_CONFIGURE *FrameBufferConfiguration;
  EFI_TPL                 Tpl;

  // Set Frame Buffer Configuration
  switch (This->Mode->Mode) {
    case 0:
      FrameBufferConfiguration = PrimaryFrameBufferConfiguration;
      break;

    case 1:
      FrameBufferConfiguration = SecondaryFrameBufferConfiguration;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "%a: Unsupported Display!\n", __FUNCTION__));
      return EFI_UNSUPPORTED;
  }

  // Raise TPL Level
  Tpl = gBS->RaiseTPL (TPL_NOTIFY);

  // Draw to Frame Buffer
  Status = FrameBufferBlt (FrameBufferConfiguration, BltBuffer, BltOperation, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);

  // Restore TPL Level
  gBS->RestoreTPL (Tpl);

  return Status;
}

EFI_STATUS
CoverDisplayBlt (
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
  EFI_STATUS                   Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL mDisplay;

  // Alocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE), (VOID *)&mDisplay.Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for GOP Mode! Status = %r\n", Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (mDisplay.Mode, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));

  // Set Secondary Display Mode Number
  mDisplay.Mode->Mode = 1;

  // Draw Pixels
  Status = DisplayBlt (&mDisplay, BltBuffer, BltOperation, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Draw defined Pixels on the Cover Screen! Status = %r\n", __FUNCTION__, Status));
  }

  // Free Buffer
  gBS->FreePool (mDisplay.Mode);

  return Status;
}

VOID
CoverDisplayGetResolution (
  OUT UINT32 *ScreenWidth,
  OUT UINT32 *ScreenHeight)
{
  // Pass Resolution
  *ScreenWidth  = FixedPcdGet32 (PcdSecondaryFrameBufferWidth);
  *ScreenHeight = FixedPcdGet32 (PcdSecondaryFrameBufferHeight);
}

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL mPrimaryDisplay = {
  DisplayQueryMode,
  DisplaySetMode,
  DisplayBlt,
  NULL
};

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL mSecondaryDisplay = {
  DisplayQueryMode,
  DisplaySetMode,
  DisplayBlt,
  NULL
};

STATIC EFI_COVER_SCREEN_PROTOCOL mCoverDisplay = {
  CoverDisplayBlt,
  CoverDisplayGetResolution
};

EFI_STATUS
CreateFrameBufferConfiguration (
  IN EFI_PHYSICAL_ADDRESS                  Address,
  IN EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *DisplayInfo,
  IN BOOLEAN                               IsPrimary)
{
  EFI_STATUS              Status;
  FRAME_BUFFER_CONFIGURE *FrameBufferConfiguration;

  // Reset Configuration Size
  STATIC UINTN FrameBufferConfigurationSize = 0;

  // Set Frame Buffer Configuration
  if (IsPrimary) {
    FrameBufferConfiguration = PrimaryFrameBufferConfiguration;
  } else {
    FrameBufferConfiguration = SecondaryFrameBufferConfiguration;
  }

  // Create FrameBufferBltLib Configuration
  Status = FrameBufferBltConfigure ((VOID *)(UINTN)Address, DisplayInfo, FrameBufferConfiguration, &FrameBufferConfigurationSize);
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    // Allocate Memory
    FrameBufferConfiguration = AllocatePool (FrameBufferConfigurationSize);
    if (FrameBufferConfiguration == NULL) {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the Frame Buffer Configuration!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    // Create FrameBufferBltLib Configuration
    Status = FrameBufferBltConfigure ((VOID *)(UINTN)Address, DisplayInfo, FrameBufferConfiguration, &FrameBufferConfigurationSize);
    if (EFI_ERROR (Status)) {
      goto ConfError;
    }
  } else if (EFI_ERROR (Status)) {
ConfError:
    DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration! Status = %r\n", Status));

    // Free Buffer
    FreePool (FrameBufferConfiguration);

    return Status;
  }

  // Set Frame Buffer Configuration
  if (IsPrimary) {
    PrimaryFrameBufferConfiguration = FrameBufferConfiguration;
  } else {
    SecondaryFrameBufferConfiguration = FrameBufferConfiguration;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SetGopInformation (
  IN OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info,
  IN     UINT32                                Width,
  IN     UINT32                                Height,
  IN     UINT32                                ColorDepth)
{
  // Set Default Infos
  Info->Version              = 0;
  Info->HorizontalResolution = Width;
  Info->VerticalResolution   = Height;
  Info->PixelsPerScanLine    = Width;

  // Set Pixel Format
  switch (ColorDepth) {
    case 32:
      Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
      break;

    case 24:
      Info->PixelFormat                   = PixelBitMask;
      Info->PixelInformation.RedMask      = 0x00FF0000;
      Info->PixelInformation.GreenMask    = 0x0000FF00;
      Info->PixelInformation.BlueMask     = 0x000000FF;
      Info->PixelInformation.ReservedMask = 0x00000000;
      break;

    case 16:
      Info->PixelFormat                   = PixelBitMask;
      Info->PixelInformation.RedMask      = 0x0000F800;
      Info->PixelInformation.GreenMask    = 0x000007E0;
      Info->PixelInformation.BlueMask     = 0x0000001F;
      Info->PixelInformation.ReservedMask = 0x00000000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "Color Depth %u is not Supported!\n", ColorDepth));
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
InitCoverDisplay (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN UINT32               Width,
  IN UINT32               Height,
  IN UINT32               ColorDepth)
{
  EFI_STATUS                            Status;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *GopInfo;

  // Check Frame Buffer Address
  if (!Address) {
    DEBUG ((EFI_D_ERROR, "The Frame Buffer Base Address is Invalid!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Check Frame Buffer Resolution
  if (!Width || !Height) {
    DEBUG ((EFI_D_ERROR, "The Frame Buffer Resolution is Invalid!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)&GopInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Info! Status = %r\n", Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (GopInfo, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

  // Set GOP Information
  Status = SetGopInformation (GopInfo, Width, Height, ColorDepth);
  if (EFI_ERROR (Status)) {
    // Free Buffer
    gBS->FreePool (GopInfo);

    return Status;
  }

  // Create Frame Buffer Configuration
  Status = CreateFrameBufferConfiguration (Address, GopInfo, FALSE);
  if (EFI_ERROR (Status)) {
    // Free Buffer
    gBS->FreePool (GopInfo);

    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
InitDisplay (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN UINT32               Width,
  IN UINT32               Height,
  IN UINT32               ColorDepth,
  IN BOOLEAN              IsPrimary)
{
  EFI_STATUS                   Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL mDisplay;

  // Check Frame Buffer Address
  if (!Address) {
    DEBUG ((EFI_D_ERROR, "The Frame Buffer Base Address is Invalid!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Check Frame Buffer Resolution
  if (Width < 640 || Height < 475) {
    DEBUG ((EFI_D_ERROR, "The Frame Buffer Resolution is Invalid!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Set Protocol Instance
  if (IsPrimary) {
    mDisplay = mPrimaryDisplay;
  } else {
    mDisplay = mSecondaryDisplay;
  }

  // Alocate Memory
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

    // Free Buffer
    gBS->FreePool (mDisplay.Mode);

    return Status;
  }

  // Clear Memory
  ZeroMem (mDisplay.Mode->Info, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

  // Set Frame Buffer Infos
  mDisplay.Mode->MaxMode                                 = 1;
  mDisplay.Mode->Mode                                    = !IsPrimary;
  mDisplay.Mode->SizeOfInfo                              = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  mDisplay.Mode->FrameBufferBase                         = Address;
  mDisplay.Mode->FrameBufferSize                         = Width * Height * (ColorDepth / 8);

  // Set GOP Information
  Status = SetGopInformation (mDisplay.Mode->Info, Width, Height, ColorDepth);
  if (EFI_ERROR (Status)) {
    // Free Buffers
    gBS->FreePool (mDisplay.Mode->Info);
    gBS->FreePool (mDisplay.Mode);

    return Status;
  }

  // Create Frame Buffer Configuration
  Status = CreateFrameBufferConfiguration (Address, mDisplay.Mode->Info, IsPrimary);
  if (EFI_ERROR (Status)) {
    // Free Buffers
    gBS->FreePool (mDisplay.Mode->Info);
    gBS->FreePool (mDisplay.Mode);

    return Status;
  }

  // Set Protocol Instance
  if (IsPrimary) {
    mPrimaryDisplay = mDisplay;
  } else {
    mSecondaryDisplay = mDisplay;
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

#if USE_CUSTOM_DISPLAY_DRIVER == 0
  // Get Frame Buffer Infos
  Width      = FixedPcdGet32 (PcdPrimaryFrameBufferWidth);
  Height     = FixedPcdGet32 (PcdPrimaryFrameBufferHeight);
  ColorDepth = FixedPcdGet32 (PcdPrimaryFrameBufferColorDepth);

  // Init Primary Display
  Status = InitDisplay (DisplayMemoryRegion.Address, Width, Height, ColorDepth, TRUE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Register GOP Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiDevicePathProtocolGuid, &PrimaryDisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mPrimaryDisplay, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GOP Protocol of the Primary Display!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((EFI_D_WARN, "Successfully Init Primary Display\n"));
#endif

  // Get Frame Buffer Offset
  UINT64 FrameBufferOffset = FixedPcdGet64 (PcdSecondaryFrameBufferOffset);

  // Verify Frame Buffer Offset
  if (FrameBufferOffset) {
    EFI_HANDLE GopHandle = NULL;

    // Get Frame Buffer Infos
    Width      = FixedPcdGet32 (PcdSecondaryFrameBufferWidth);
    Height     = FixedPcdGet32 (PcdSecondaryFrameBufferHeight);
    ColorDepth = FixedPcdGet32 (PcdSecondaryFrameBufferColorDepth);

    // Check Resolution
    if (Width < 640 || Height < 475) {
      // Init Secondary Cover Display
      Status = InitCoverDisplay (DisplayMemoryRegion.Address + FrameBufferOffset, Width, Height, ColorDepth);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      // Register Cover Screen Protocol
      Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiCoverScreenProtocolGuid, EFI_NATIVE_INTERFACE, &mCoverDisplay);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Register Cover Screen Protocol!\n"));
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      // Init Secondary Display
      Status = InitDisplay (DisplayMemoryRegion.Address + FrameBufferOffset, Width, Height, ColorDepth, FALSE);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      // Register GOP Protocol
      Status = gBS->InstallMultipleProtocolInterfaces (&GopHandle, &gEfiDevicePathProtocolGuid, &SecondaryDisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mSecondaryDisplay, NULL);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Register GOP Protocol of the Secondary Display!\n"));
        ASSERT_EFI_ERROR (Status);
      }
    }

    DEBUG ((EFI_D_WARN, "Successfully Init Secondary Display\n"));
  }

  return EFI_SUCCESS;
}
