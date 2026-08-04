#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>

#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>

#include "UFPLoader.h"

EFI_DEVICE_PATH_PROTOCOL*
GetUfpDevicePath (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_GUID   *AppGuid)
{
  EFI_STATUS                         Status;
  EFI_LOADED_IMAGE_PROTOCOL         *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;

  // Get Current Loaded Image Protocol Handle
  Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID *)&LoadedImage);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  // Get Current Device Path Protocl Handle
  Status = gBS->HandleProtocol (LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID *)&DevicePath);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  // Initialize Device Path Node
  EfiInitializeFwVolDevicepathNode (&FileNode, AppGuid);

  // Return UFP App Device Path
  return AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL *)&FileNode);
}

EFI_STATUS
EFIAPI
EnterUfp (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 UfpHandle;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;

  // Get UFP App Device Path
  EFI_DEVICE_PATH_PROTOCOL *UfpDevicePath = GetUfpDevicePath (ImageHandle, FixedPcdGetPtr (PcdUfpDeviveFwFile));
  if (UfpDevicePath == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate UFP App Device Path!"));
    return EFI_NOT_FOUND;
  }

  // Load UFP App
  Status = gBS->LoadImage (FALSE, ImageHandle, UfpDevicePath, NULL, 0, &UfpHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Load UFP App! Status = %r\n", Status));
    return Status;
  }

  // Get UFP App Loaded Image Protocol
  Status = gBS->HandleProtocol (UfpHandle, &gEfiLoadedImageProtocolGuid, (VOID *)&LoadedImage);
  if (!EFI_ERROR (Status)) {
    // Apply UFP App Patches
    ApplyUfpPatches ((EFI_PHYSICAL_ADDRESS)LoadedImage->ImageBase, LoadedImage->ImageSize);
  }

  // Start UFP App
  return gBS->StartImage (UfpHandle, NULL, NULL);
}
