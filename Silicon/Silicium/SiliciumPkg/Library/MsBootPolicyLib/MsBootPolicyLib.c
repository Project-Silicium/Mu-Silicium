/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
//#include <Library/DeviceBootManagerLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/LoadFile.h>

STATIC MS_BUTTON_SERVICES_PROTOCOL *mMsButtonProtocol;
STATIC EFI_IMAGE_LOAD               gSystemLoadImage;

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
  // Check Device Path
  if (DevicePath != NULL) {
    // Check Device Path Boot State
    if (!MsBootPolicyLibIsDevicePathBootable (DevicePath)) {
      return EFI_ACCESS_DENIED;
    }
  }

  // Pass LoadImage Call to System LoadImage
  return gSystemLoadImage (BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);
}

EFI_STATUS
LocateMsButtonServiceProtocol ()
{
  EFI_STATUS Status;

  // Locate Ms Button Service Protocol
  if (mMsButtonProtocol == NULL) {
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID *)&mMsButtonProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Service Protocol! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsSettingsBoot ()
{
  EFI_STATUS Status;
  BOOLEAN    Pressed;

  // Locate Ms Button Service Protocol
  Status = LocateMsButtonServiceProtocol ();
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  // Check if Volume Up / ESC was Pressed
  Status = mMsButtonProtocol->PreBootVolumeUpButtonThenPowerButtonCheck (mMsButtonProtocol, &Pressed);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Get Volume Up State! Status = %r\n", __FUNCTION__, Status));
    Pressed = FALSE;
  }

  return Pressed;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsAltBoot ()
{
    EFI_STATUS Status;
  BOOLEAN    Pressed;

  // Locate Ms Button Service Protocol
  Status = LocateMsButtonServiceProtocol ();
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  // Check if Volume Down / ENTF was Pressed
  Status = mMsButtonProtocol->PreBootVolumeDownButtonThenPowerButtonCheck (mMsButtonProtocol, &Pressed);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Get Volume Down State! Status = %r\n", __FUNCTION__, Status));
    Pressed = FALSE;
  }

  return Pressed;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibClearBootRequests ()
{
  EFI_STATUS Status;

  // Locate Ms Button Service Protocol
  Status = LocateMsButtonServiceProtocol ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Clear Button States
  Status = mMsButtonProtocol->PreBootClearVolumeButtonState (mMsButtonProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Clear Button States! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathBootable (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  // Check Device Path
  if (!IsDevicePathValid (DevicePath, 0) || !DevicePath) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDeviceBootable (IN EFI_HANDLE ControllerHandle)
{
  // Check Device Path Boot State
  return MsBootPolicyLibIsDevicePathBootable (DevicePathFromHandle (ControllerHandle));
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathUsb (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  for (EFI_DEVICE_PATH_PROTOCOL *Node = DevicePath; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    // Compare Device Path Types
    if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH) && ((DevicePathSubType (Node) == MSG_USB_CLASS_DP) || (DevicePathSubType (Node) == MSG_USB_WWID_DP) || (DevicePathSubType( Node) == MSG_USB_DP))) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibGetBootSequence (
  OUT BOOT_SEQUENCE **BootSequence,
  IN  BOOLEAN         AltBootRequest)
{
  // Set Default Boot Chain
  BOOT_SEQUENCE DefaultBootSequence[] = { MsBootHDD, MsBootUSB, MsBootDone };

  // Check Parameters
  if (BootSequence == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Pass Default Boot Chain
  *BootSequence = DefaultBootSequence;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Check PCD Setting
  if (PcdGetBool (PcdBdsBootPolicy)) {
    EFI_TPL Tpl;
    UINT32  Crc;

    // Raise TPL Level
    Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);

    // Set New Function
    gSystemLoadImage = gBS->LoadImage;
    gBS->LoadImage   = LocalLoadImage;
    gBS->Hdr.CRC32   = 0;

    // Calculate CRC
    gBS->CalculateCrc32 ((UINT8 *)&gBS->Hdr, gBS->Hdr.HeaderSize, &Crc);

    // Set new CRC Value
    gBS->Hdr.CRC32 = Crc;

    // Restore TPL Level
    gBS->RestoreTPL (Tpl);
  }

  return EFI_SUCCESS;
}
