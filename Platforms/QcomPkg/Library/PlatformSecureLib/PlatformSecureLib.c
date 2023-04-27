/** @file

Copyright (c) 2011 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>

#include <Protocol/DevicePath.h>

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Configuration/BootDevices.h>
#include <Protocol/SimpleTextInEx.h>

BOOLEAN  mUserPhysicalPresence = TRUE;
static EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx = NULL;

EFI_STATUS
EFIAPI
KeyNotify(IN EFI_KEY_DATA *KeyData)
{
  mUserPhysicalPresence = TRUE;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LocateButtonProtocol (
  VOID
)
{
  EFI_STATUS                         Status   = EFI_SUCCESS;
  EFI_HANDLE                         Handle   = NULL;
  EFI_DEVICE_PATH_PROTOCOL          *ButtonsDxeDevicePath;

  ButtonsDxeDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath;

  Status = gBS->LocateDevicePath(
      &gEfiSimpleTextInputExProtocolGuid, &ButtonsDxeDevicePath, &Handle);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to locate ButtonsDxe device path for button service protocol, "
         "Status = %r.\n",
         Status));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->OpenProtocol(
      Handle, &gEfiSimpleTextInputExProtocolGuid, (VOID **)&SimpleEx,
      gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to open ButtonsDxe protocol for button service protocol, "
         "Status = %r.\n",
         Status));
    return Status;
  }
  
  mUserPhysicalPresence = FALSE;

  return Status;
}

EFI_STATUS
EFIAPI
RegisterKeyNotification (
  VOID
)
{
  EFI_STATUS                         Status   = EFI_SUCCESS;
  EFI_KEY_DATA                       KeyData;
  VOID                              *NotifyHandle;

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;
  KeyData.Key.UnicodeChar         = 0;

  KeyData.Key.ScanCode = SCAN_UP;

  if (SimpleEx == NULL) {
    Status = LocateButtonProtocol ();
    if (EFI_ERROR(Status) || SimpleEx == NULL) {
      return Status;
    }
  }

  // Prevent Key persistence when chainload
  SimpleEx->Reset(SimpleEx, TRUE);

  Status = SimpleEx->RegisterKeyNotify(
      SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to register volume up notification for button service "
         "protocol, "
         "Status = %r.\n",
         Status));
    return Status;
  }

  KeyData.Key.ScanCode = SCAN_DOWN;

  Status = SimpleEx->RegisterKeyNotify(
      SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to register volume down notification for button service "
         "protocol, "
         "Status = %r.\n",
         Status));
    return Status;
  }

  return Status;
}

/**

  This function provides a platform-specific method to detect whether the platform
  is operating by a physically present user.

  Programmatic changing of platform security policy (such as disable Secure Boot,
  or switch between Standard/Custom Secure Boot mode) MUST NOT be possible during
  Boot Services or after exiting EFI Boot Services. Only a physically present user
  is allowed to perform these operations.

  NOTE THAT: This function cannot depend on any EFI Variable Service since they are
  not available when this function is called in AuthenticateVariable driver.

  @retval  TRUE       The platform is operated by a physically present user.
  @retval  FALSE      The platform is NOT operated by a physically present user.

**/
BOOLEAN
EFIAPI
UserPhysicalPresent (
  VOID
  )
{
  if (SimpleEx == NULL) {
    RegisterKeyNotification ();
  }
  
  return mUserPhysicalPresence;
}

/**
  Save user physical presence state from a PCD to mUserPhysicalPresence.

  @retval  EFI_SUCCESS          PcdUserPhysicalPresence is got successfully.

**/
RETURN_STATUS
EFIAPI
PlatformSecureLibConstructor (
  VOID
  )
{
  RegisterKeyNotification ();
  return EFI_SUCCESS;
}
