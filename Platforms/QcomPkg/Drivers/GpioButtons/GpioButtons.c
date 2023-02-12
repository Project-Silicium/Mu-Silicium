/** @file GpioButtons.c

  This module installs the MsButtonServicesProtocol.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Configuration/BootDevices.h>
#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

BOOLEAN VolumeUpState   = FALSE;
BOOLEAN VolumeDownState = FALSE;

/*
Say volume button is pressed because we wan to go to frontpage.
*/
EFI_STATUS
EFIAPI
PreBootVolumeUpButtonThenPowerButtonCheck(
    IN MS_BUTTON_SERVICES_PROTOCOL *This,
    OUT BOOLEAN *PreBootVolumeUpButtonThenPowerButton // TRUE if button combo
                                                      // set else FALSE
)
{
  DEBUG((DEBUG_ERROR, "%a \n", __FUNCTION__));
  *PreBootVolumeUpButtonThenPowerButton = VolumeUpState;
  return EFI_SUCCESS;
}

/*
Say no because we don't want alt boot.
*/
EFI_STATUS
EFIAPI
PreBootVolumeDownButtonThenPowerButtonCheck(
    IN MS_BUTTON_SERVICES_PROTOCOL *This,
    OUT BOOLEAN *PreBootVolumeDownButtonThenPowerButton // TRUE if button combo
                                                        // set else FALSE
)
{
  DEBUG((DEBUG_ERROR, "%a \n", __FUNCTION__));
  *PreBootVolumeDownButtonThenPowerButton = VolumeDownState;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PreBootClearVolumeButtonState(MS_BUTTON_SERVICES_PROTOCOL *This)
{
  DEBUG((DEBUG_ERROR, "%a \n", __FUNCTION__));
  VolumeUpState   = FALSE;
  VolumeDownState = FALSE;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KeyNotify(IN EFI_KEY_DATA *KeyData)
{
  if (KeyData->Key.ScanCode == SCAN_UP) {
    VolumeUpState = !VolumeUpState;
  }
  else if (KeyData->Key.ScanCode == SCAN_DOWN) {
    VolumeDownState = !VolumeDownState;
  }
  return EFI_SUCCESS;
}

/**
 Init routine to install protocol and init anything related to buttons

 **/
EFI_STATUS
EFIAPI
ButtonsInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  MS_BUTTON_SERVICES_PROTOCOL       *Protocol = NULL;
  EFI_STATUS                         Status   = EFI_SUCCESS;
  EFI_HANDLE                         Handle   = NULL;
  EFI_DEVICE_PATH_PROTOCOL          *ButtonsDxeDevicePath;
  EFI_KEY_DATA                       KeyData;
  VOID                              *NotifyHandle;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;

  DEBUG((DEBUG_ERROR, "%a \n", __FUNCTION__));

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

  VolumeUpState   = FALSE;
  VolumeDownState = FALSE;

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;
  KeyData.Key.UnicodeChar         = 0;

  KeyData.Key.ScanCode = SCAN_UP;

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

  Protocol = AllocateZeroPool(sizeof(MS_BUTTON_SERVICES_PROTOCOL));
  if (Protocol == NULL) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to allocate memory for button service protocol.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Protocol->PreBootVolumeDownButtonThenPowerButtonCheck =
      PreBootVolumeDownButtonThenPowerButtonCheck;
  Protocol->PreBootVolumeUpButtonThenPowerButtonCheck =
      PreBootVolumeUpButtonThenPowerButtonCheck;
  Protocol->PreBootClearVolumeButtonState = PreBootClearVolumeButtonState;

  // Install the protocol
  Status = gBS->InstallMultipleProtocolInterfaces(
      &ImageHandle, &gMsButtonServicesProtocolGuid, Protocol, NULL);

  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Button Services Protocol Publisher: install protocol error, Status = "
         "%r.\n",
         Status));
    FreePool(Protocol);
    return Status;
  }

  DEBUG((DEBUG_INFO, "Button Services Protocol Installed!\n"));
  return Status;
}