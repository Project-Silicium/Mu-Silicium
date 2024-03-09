#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/GraphicsOutput.h>

#include <Configuration/BootDevices.h>

STATIC FRAME_BUFFER_CONFIGURE *mFrameBufferBltLibConfigure;

STATIC UINT32 gBpp = FixedPcdGet32(PcdMipiFrameBufferColorDepth);

STATIC
EFI_STATUS
EFIAPI
DisplayQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info)
{
  EFI_STATUS Status;

  // Allocate Memory for Graphics Output Info
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID **)Info);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Graphics Output Info! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  *SizeOfInfo                   = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  (*Info)->Version              = This->Mode->Info->Version;
  (*Info)->HorizontalResolution = This->Mode->Info->HorizontalResolution;
  (*Info)->VerticalResolution   = This->Mode->Info->VerticalResolution;
  (*Info)->PixelFormat          = This->Mode->Info->PixelFormat;
  (*Info)->PixelsPerScanLine    = This->Mode->Info->PixelsPerScanLine;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DisplaySetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN UINT32                        ModeNumber)
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DisplayBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL     *This,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL    *BltBuffer,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation OPTIONAL,
  IN UINTN                             SourceX,
  IN UINTN                             SourceY,
  IN UINTN                             DestinationX,
  IN UINTN                             DestinationY,
  IN UINTN                             Width,
  IN UINTN                             Height,
  IN UINTN                             Delta OPTIONAL)
{
  RETURN_STATUS Status;
  EFI_TPL       Tpl;

  //
  // We have to Raise to TPL_NOTIFY, So we Make an Atomic Write to the Frame Buffer.
  // We would not want a Timer based Event (Cursor, ...) to come in while we are doing this Operation.
  //
  Tpl    = gBS->RaiseTPL (TPL_NOTIFY);
  Status = FrameBufferBlt (mFrameBufferBltLibConfigure, BltBuffer, BltOperation, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
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
EFIAPI
InitializeDisplay (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS   Status;
  STATIC UINTN mFrameBufferBltLibConfigureSize;

  // Get the Frame Buffer Address
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegion;
  Status = LocateMemoryMapAreaByName ("Display Reserved", &DisplayMemoryRegion);
  if (EFI_ERROR (Status)) {
    // Get Primary Frame Buffer Address
    Status = LocateMemoryMapAreaByName ("Display Reserved-1", &DisplayMemoryRegion);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Display Reserved Memory Region!\n"));
      goto exit;
    }
  }

  // Get the Width and Height of the Frame Buffer
  UINT32 MipiFrameBufferWidth  = FixedPcdGet32(PcdMipiFrameBufferWidth);
  UINT32 MipiFrameBufferHeight = FixedPcdGet32(PcdMipiFrameBufferHeight);

  // Check if Frame Buffer Infos aren't Invalid
  if (MipiFrameBufferWidth == 0 || MipiFrameBufferHeight == 0) {
    DEBUG ((EFI_D_ERROR, "Invalid Frame Buffer Information!\n"));
    Status = EFI_UNSUPPORTED;
    goto exit;
  }

  if (mDisplay.Mode == NULL) {
    // Alocate Memory for the Graphics Output Mode
    Status = gBS->AllocatePool (EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE), (VOID **)&mDisplay.Mode);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Graphics Output Mode!\n"));
      goto exit;
    }

    ZeroMem(mDisplay.Mode, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  }

  if (mDisplay.Mode->Info == NULL) {
    // Allocate Memory for the Graphics Output Mode Info
    Status = gBS->AllocatePool (EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID **)&mDisplay.Mode->Info);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Graphics Output Mode Info!\n"));
      goto exit;
    }

    ZeroMem(mDisplay.Mode->Info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  }

  // Set Informations about Frame Buffer
  mDisplay.Mode->MaxMode                                 = 1;
  mDisplay.Mode->Mode                                    = 0;
  mDisplay.Mode->SizeOfInfo                              = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  mDisplay.Mode->FrameBufferBase                         = DisplayMemoryRegion.Address;
  mDisplay.Mode->FrameBufferSize                         = DisplayMemoryRegion.Length;
  mDisplay.Mode->Info->Version                           = 0;
  mDisplay.Mode->Info->HorizontalResolution              = MipiFrameBufferWidth;
  mDisplay.Mode->Info->VerticalResolution                = MipiFrameBufferHeight;
  mDisplay.Mode->Info->PixelsPerScanLine                 = MipiFrameBufferWidth;

  // Set Pixel Format according to Color Depth
  if (gBpp == 32) {
    mDisplay.Mode->Info->PixelFormat                     = PixelBlueGreenRedReserved8BitPerColor;
  } else if (gBpp == 24) {
    mDisplay.Mode->Info->PixelFormat                     = PixelBitMask;
    mDisplay.Mode->Info->PixelInformation.RedMask        = 0x00FF0000; // Red
    mDisplay.Mode->Info->PixelInformation.GreenMask      = 0x0000FF00; // Green
    mDisplay.Mode->Info->PixelInformation.BlueMask       = 0x000000FF; // Blue
    mDisplay.Mode->Info->PixelInformation.ReservedMask   = 0;          // Reserved
  } else {
    DEBUG ((EFI_D_ERROR, "There is no Valid Pixel Format for Color Depth %d!\n", gBpp));
    Status = EFI_UNSUPPORTED;
    goto exit;
  }

  // Create the FrameBufferBltLib Configuration.
  Status = FrameBufferBltConfigure ((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, mFrameBufferBltLibConfigure, &mFrameBufferBltLibConfigureSize);
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    mFrameBufferBltLibConfigure = AllocatePool (mFrameBufferBltLibConfigureSize);
    if (mFrameBufferBltLibConfigure != NULL) {
      Status = FrameBufferBltConfigure ((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, mFrameBufferBltLibConfigure, &mFrameBufferBltLibConfigureSize);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration! 2\n"));
        goto exit;
      }
    } else {
      DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the Frame Buffer Configuration!\n"));
      Status = EFI_ABORTED;
      goto exit;
    }
  } else if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Create the Frame Buffer Configuration! 1\n"));
    goto exit;
  }

  // Register Handle
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiDevicePathProtocolGuid, &DisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mDisplay, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Graphics Output Protocol! Status = %r\n", Status));
  }

exit:
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}
