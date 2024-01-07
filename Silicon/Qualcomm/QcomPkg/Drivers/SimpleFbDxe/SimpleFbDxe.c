#include <PiDxe.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/FrameBufferBltLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/GraphicsOutput.h>

#include <Configuration/BootDevices.h>

#include "SimpleFbDxe.h"

STATIC
EFI_STATUS
EFIAPI
DisplayQueryMode(
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                 ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info)
{
  EFI_STATUS Status;

  Status = gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID **)Info);
  ASSERT_EFI_ERROR(Status);

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
DisplaySetMode(
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
  IN UINT32                        ModeNumber)
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
DisplayBlt(
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
  // We have to raise to TPL_NOTIFY, so we make an atomic write to the frame
  // buffer. We would not want a timer based event (Cursor, ...) to come in
  // while we are doing this operation.
  //
  Tpl    = gBS->RaiseTPL(TPL_NOTIFY);
  Status = FrameBufferBlt(mFrameBufferBltLibConfigure, BltBuffer, BltOperation, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
  gBS->RestoreTPL(Tpl);

  return RETURN_ERROR(Status) ? EFI_INVALID_PARAMETER : EFI_SUCCESS;
}

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL mDisplay = {
  DisplayQueryMode,
  DisplaySetMode,
  DisplayBlt,
  NULL
};

EFI_STATUS
EFIAPI
SimpleFbDxeInitialize(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{

  EFI_STATUS Status             = EFI_SUCCESS;
  EFI_HANDLE mUEFIDisplayHandle = NULL;

  // Retrieve simple frame buffer from pre-SEC bootloader
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegion;
  LocateMemoryMapAreaByName("Display Reserved", &DisplayMemoryRegion);
  UINT32 MipiFrameBufferAddr   = DisplayMemoryRegion.Address;
  UINT32 MipiFrameBufferWidth  = FixedPcdGet32(PcdMipiFrameBufferWidth);
  UINT32 MipiFrameBufferHeight = FixedPcdGet32(PcdMipiFrameBufferHeight);

  // Sanity check
  if (MipiFrameBufferAddr == 0 || MipiFrameBufferWidth == 0 || MipiFrameBufferHeight == 0) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((EFI_D_ERROR, "Invalid Resolution! Status = %r\n", Status));
    goto exit;
  }

  // Prepare struct
  if (mDisplay.Mode == NULL) {
    Status = gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE), (VOID **)&mDisplay.Mode);
    ASSERT_EFI_ERROR(Status);

    ZeroMem(mDisplay.Mode, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  }

  if (mDisplay.Mode->Info == NULL) {
    Status = gBS->AllocatePool(EfiBootServicesData, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (VOID **)&mDisplay.Mode->Info);
    ASSERT_EFI_ERROR(Status);

    ZeroMem(mDisplay.Mode->Info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  }

  // Set information
  mDisplay.Mode->MaxMode                                 = 1;
  mDisplay.Mode->Mode                                    = 0;
  mDisplay.Mode->Info->Version                           = 0;

  mDisplay.Mode->Info->HorizontalResolution              = MipiFrameBufferWidth;
  mDisplay.Mode->Info->VerticalResolution                = MipiFrameBufferHeight;

  // SimpleFB runs on BGR for WoA Devices
  UINT32               FrameBufferSize                   = DisplayMemoryRegion.Length;
  EFI_PHYSICAL_ADDRESS FrameBufferAddress                = MipiFrameBufferAddr;

  mDisplay.Mode->Info->PixelsPerScanLine                 = MipiFrameBufferWidth;
  if (FixedPcdGet32(PcdMipiFrameBufferPixelBpp) == 32) {
    mDisplay.Mode->Info->PixelFormat                     = PixelBlueGreenRedReserved8BitPerColor;
  } else {
    mDisplay.Mode->Info->PixelFormat                     = PixelBitMask;
    mDisplay.Mode->Info->PixelInformation.RedMask        = 0x00FF0000; // Red
    mDisplay.Mode->Info->PixelInformation.GreenMask      = 0x0000FF00; // Green
    mDisplay.Mode->Info->PixelInformation.BlueMask       = 0x000000FF; // Blue
    mDisplay.Mode->Info->PixelInformation.ReservedMask   = 0;          // Reserved
  }

  mDisplay.Mode->SizeOfInfo                              = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
  mDisplay.Mode->FrameBufferBase                         = FrameBufferAddress;
  mDisplay.Mode->FrameBufferSize                         = FrameBufferSize;

  // Create the FrameBufferBltLib configuration.
  Status = FrameBufferBltConfigure((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, mFrameBufferBltLibConfigure, &mFrameBufferBltLibConfigureSize);
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    mFrameBufferBltLibConfigure = AllocatePool(mFrameBufferBltLibConfigureSize);
    if (mFrameBufferBltLibConfigure != NULL) {
      Status = FrameBufferBltConfigure((VOID *)(UINTN)mDisplay.Mode->FrameBufferBase, mDisplay.Mode->Info, mFrameBufferBltLibConfigure, &mFrameBufferBltLibConfigureSize);
      ASSERT_EFI_ERROR(Status);
    }
  }

  // Register handle
  Status = gBS->InstallMultipleProtocolInterfaces(&mUEFIDisplayHandle, &gEfiDevicePathProtocolGuid, &DisplayDevicePath, &gEfiGraphicsOutputProtocolGuid, &mDisplay, NULL);
  ASSERT_EFI_ERROR(Status);

exit:
  return Status;
}
