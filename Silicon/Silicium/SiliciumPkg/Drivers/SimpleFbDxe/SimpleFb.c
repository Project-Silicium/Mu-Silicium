#include <Library/DebugLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include <Configuration/BootDevices.h>

//
// Global Variables
//
STATIC FRAME_BUFFER_CONFIGURE *FrameBufferConfiguration;

EFI_STATUS
FrameBufferQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info)
{
  EFI_STATUS Status;

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)Info);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Info Size
  *SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

  // Pass Frame Buffer Infos
  (*Info)->Version              = This->Mode->Info->Version;
  (*Info)->HorizontalResolution = This->Mode->Info->HorizontalResolution;
  (*Info)->VerticalResolution   = This->Mode->Info->VerticalResolution;
  (*Info)->PixelFormat          = This->Mode->Info->PixelFormat;
  (*Info)->PixelsPerScanLine    = This->Mode->Info->PixelsPerScanLine;

  return EFI_SUCCESS;
}

EFI_STATUS
FrameBufferSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN UINT32                        ModeNumber)
{
  return EFI_SUCCESS;
}

EFI_STATUS
FrameBufferGopBlt (
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

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL mFrameBuffer = {
  FrameBufferQueryMode,
  FrameBufferSetMode,
  FrameBufferGopBlt,
  NULL
};

EFI_STATUS
GetFrameBufferInfos (
  OUT EFI_PHYSICAL_ADDRESS *BaseAddress,
  OUT UINT32               *Width,
  OUT UINT32               *Height,
  OUT UINT32               *ColorDepth)
{
  EFI_MEMORY_REGION_DESCRIPTOR FrameBufferRegion;

  // Locate "Display Reserved" Memory Region
  LocateMemoryRegionByName ("Display Reserved", &FrameBufferRegion);
  LocateMemoryRegionByName ("Display_Reserved", &FrameBufferRegion);

  // Verify Frame Buffer Region
  if (!FrameBufferRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"Display Reserved\" Memory Region!\n"));
    return EFI_NOT_FOUND;
  }

  // Get Frame Buffer Details
  *Width      = FixedPcdGet32 (PcdFrameBufferWidth);
  *Height     = FixedPcdGet32 (PcdFrameBufferHeight);
  *ColorDepth = FixedPcdGet32 (PcdFrameBufferColorDepth);

  // Verify Frame Buffer Resolution
  if (*Width < 640 || *Height < 475) {
    DEBUG ((EFI_D_ERROR, "Frame Buffer Resolution is too Small!\n"));
    DEBUG ((EFI_D_ERROR, "Minimal Supported Resolution = 640x475\n"));

    return EFI_UNSUPPORTED;
  }

  // Pass Frame Buffer Address
  *BaseAddress = FrameBufferRegion.Address;

  return EFI_SUCCESS;
}

EFI_STATUS
AllocateGopMemory ()
{
  EFI_STATUS Status;

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE), (VOID *)&mFrameBuffer.Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Mode! Status = %r\n", Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (mFrameBuffer.Mode, sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));

  // Allocate Memory
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID *)&mFrameBuffer.Mode->Info);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for GOP Info! Status = %r\n", Status));
    return Status;
  }

  // Clear Memory
  ZeroMem (mFrameBuffer.Mode->Info, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

  return EFI_SUCCESS;
}

EFI_STATUS
SetFrameBufferModeDetails (
  IN EFI_PHYSICAL_ADDRESS BaseAddress,
  IN UINT32               Width,
  IN UINT32               Height,
  IN UINT32               ColorDepth)
{
  // Set Info Size
  mFrameBuffer.Mode->SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

  // Set Mode Infos
  mFrameBuffer.Mode->MaxMode = 1;
  mFrameBuffer.Mode->Mode    = 0;

  // Set Frame Buffer Range
  mFrameBuffer.Mode->FrameBufferBase = BaseAddress;
  mFrameBuffer.Mode->FrameBufferSize = Width * Height * (ColorDepth / 8);

  // Set Mode Info Version
  mFrameBuffer.Mode->Info->Version = 0;

  // Set Frame Buffer Infos
  mFrameBuffer.Mode->Info->HorizontalResolution = Width;
  mFrameBuffer.Mode->Info->VerticalResolution   = Height;
  mFrameBuffer.Mode->Info->PixelsPerScanLine    = Width;

  // Set Pixel Format
  switch (ColorDepth) {
    case 32:
      mFrameBuffer.Mode->Info->PixelFormat = PixelBlueGreenRedReserved8BitPerColor;
      break;

    case 24:
      mFrameBuffer.Mode->Info->PixelFormat                   = PixelBitMask;
      mFrameBuffer.Mode->Info->PixelInformation.RedMask      = 0x00FF0000;
      mFrameBuffer.Mode->Info->PixelInformation.GreenMask    = 0x0000FF00;
      mFrameBuffer.Mode->Info->PixelInformation.BlueMask     = 0x000000FF;
      mFrameBuffer.Mode->Info->PixelInformation.ReservedMask = 0x00000000;
      break;

    case 16:
      mFrameBuffer.Mode->Info->PixelFormat                   = PixelBitMask;
      mFrameBuffer.Mode->Info->PixelInformation.RedMask      = 0x0000F800;
      mFrameBuffer.Mode->Info->PixelInformation.GreenMask    = 0x000007E0;
      mFrameBuffer.Mode->Info->PixelInformation.BlueMask     = 0x0000001F;
      mFrameBuffer.Mode->Info->PixelInformation.ReservedMask = 0x00000000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "Color Depth %u is not Supported!\n", ColorDepth));
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
CreateFrameBufferConfig (IN EFI_PHYSICAL_ADDRESS BaseAddress)
{
  EFI_STATUS Status;

  // Set Configuration Size
  STATIC UINTN FrameBufferConfigurationSize = 0;

  // Get Frame Buffer Configuration Size
  FrameBufferBltConfigure ((VOID *)(UINTN)BaseAddress, mFrameBuffer.Mode->Info, FrameBufferConfiguration, &FrameBufferConfigurationSize);

  // Allocate Memory
  FrameBufferConfiguration = AllocatePool (FrameBufferConfigurationSize);
  if (FrameBufferConfiguration == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the Frame Buffer Configuration!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Create FrameBufferBltLib Configuration
  Status = FrameBufferBltConfigure ((VOID *)(UINTN)BaseAddress, mFrameBuffer.Mode->Info, FrameBufferConfiguration, &FrameBufferConfigurationSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration! Status = %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
FreeMemoryBuffers ()
{
  // Free Buffer
  if (mFrameBuffer.Mode->Info != NULL) {
    gBS->FreePool (mFrameBuffer.Mode->Info);
  }

  // Free Buffer
  if (mFrameBuffer.Mode != NULL) {
    gBS->FreePool (mFrameBuffer.Mode);
  }

  // Free Buffer
  if (FrameBufferConfiguration != NULL) {
    FreePool (FrameBufferConfiguration);
  }
}

EFI_STATUS
EFIAPI
RegisterFrameBuffer (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS BaseAddress;
  UINT32               Width;
  UINT32               Height;
  UINT32               ColorDepth;

  // Get Frame Buffer Infos
  Status = GetFrameBufferInfos (&BaseAddress, &Width, &Height, &ColorDepth);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Allocate Memory
  Status = AllocateGopMemory ();
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Set Frame Buffer Mode Details
  Status = SetFrameBufferModeDetails (BaseAddress, Width, Height, ColorDepth);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Create Frame Buffer Config
  Status = CreateFrameBufferConfig (BaseAddress);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Register GOP Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiDevicePathProtocolGuid, &DisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mFrameBuffer, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GOP Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;

cleanup:
  // Free Memory Buffers
  FreeMemoryBuffers ();

  return Status;
}
