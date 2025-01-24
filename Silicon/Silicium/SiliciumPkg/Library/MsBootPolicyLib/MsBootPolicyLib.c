/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsPlatformDevicesLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/LoadFile.h>

#ifdef EFI_DEBUG
#define MAX_DEVICE_PATH_SIZE 0x100000
#else
#define MAX_DEVICE_PATH_SIZE 0
#endif

STATIC BOOT_SEQUENCE DefaultBootSequence[] = {
  MsBootHDD,
  MsBootUSB,
  MsBootDone
};

STATIC MS_BUTTON_SERVICES_PROTOCOL *mMsButtonProtocol = NULL;
STATIC EFI_IMAGE_LOAD               gSystemLoadImage  = NULL;

EFI_STATUS
EFIAPI
LocalLoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  VOID                     *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE               *ImageHandle)
{
  if (DevicePath != NULL) {
    if (!MsBootPolicyLibIsDevicePathBootable (DevicePath)) {
      return EFI_ACCESS_DENIED;
    }
  }

  // Pass LoadImage Call to System LoadImage
  return gSystemLoadImage (BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsSettingsBoot ()
{
  EFI_STATUS Status;
  BOOLEAN    VolumeUp;

  // Check if the Protocol Exists
  if (!mMsButtonProtocol) {
    // Locate Ms Button Services Protocol
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID *)&mMsButtonProtocol);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Services Protocol! Status = %r\n", __FUNCTION__, Status));
      return FALSE;
    }
  }

  // Check if Volume Up was Pressed
  Status = mMsButtonProtocol->PreBootVolumeUpButtonThenPowerButtonCheck (mMsButtonProtocol, &VolumeUp);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Get Volume Up State! Status = %r\n", __FUNCTION__, Status));
  }

  return VolumeUp;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsAltBoot ()
{
  EFI_STATUS Status;
  BOOLEAN    VolumeDown;

  // Check if the Protocol Exists
  if (!mMsButtonProtocol) {
    // Locate Ms Button Services Protocol
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID *)&mMsButtonProtocol);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Services Protocol! Status = %r\n", __FUNCTION__, Status));
      return FALSE;
    }
  }

  // Check if Volume Down was Pressed
  Status = mMsButtonProtocol->PreBootVolumeDownButtonThenPowerButtonCheck (mMsButtonProtocol, &VolumeDown);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Get Volume Down State! Status = %r\n", __FUNCTION__, Status));
  }

  return VolumeDown;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibClearBootRequests ()
{
  EFI_STATUS Status;

  // Check if the Protocol Exists
  if (!mMsButtonProtocol) {
    // Locate Ms Button Services Protocol
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID *)&mMsButtonProtocol);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Services Protocol! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  // Clear Volume Button States
  Status = mMsButtonProtocol->PreBootClearVolumeButtonState (mMsButtonProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Clear Volume Button States! Status = %r\n", __FUNCTION__, Status));
  }

  return Status;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathBootable (EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  if (!IsDevicePathValid (DevicePath, MAX_DEVICE_PATH_SIZE) || !DevicePath) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDeviceBootable (EFI_HANDLE ControllerHandle)
{
  return MsBootPolicyLibIsDevicePathBootable (DevicePathFromHandle (ControllerHandle));
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathUsb (EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  return PlatformIsDevicePathUsb (DevicePath);
}

EFI_STATUS
EFIAPI
MsBootPolicyLibGetBootSequence (
  BOOT_SEQUENCE **BootSequence,
  BOOLEAN         AltBootRequest)
{
  if (BootSequence == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *BootSequence = DefaultBootSequence;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Take over gBS->LoadImage.
  if (PcdGetBool (PcdBdsBootPolicy)) {
    UINT32 Crc = 0;

    // Raise TPL Level
    EFI_TPL OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);

    // Set New Function
    gSystemLoadImage = gBS->LoadImage;
    gBS->LoadImage   = LocalLoadImage;
    gBS->Hdr.CRC32   = 0;

    // Calculate CRC
    gBS->CalculateCrc32 ((UINT8 *)&gBS->Hdr, gBS->Hdr.HeaderSize, &Crc);

    // Set new CRC Value
    gBS->Hdr.CRC32 = Crc;

    // Restore TPL Level
    gBS->RestoreTPL (OldTpl);
  }

  return EFI_SUCCESS;
}
