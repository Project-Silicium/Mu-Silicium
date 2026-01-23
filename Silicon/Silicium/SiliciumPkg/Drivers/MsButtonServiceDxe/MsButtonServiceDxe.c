/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

#include <Configuration/BootDevices.h>

//
// Global Variables
//
STATIC UINT16 CurrentButton;

EFI_STATUS
EFIAPI
SpecialAppButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  // Pass Button State
  *ButtonPressed = (CurrentButton == SCAN_DOWN) || (CurrentButton == SCAN_DELETE);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
UefiMenuButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  // Pass Button State
  *ButtonPressed = (CurrentButton == SCAN_UP) || (CurrentButton == SCAN_ESC);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ClearButtonState (IN MS_BUTTON_SERVICES_PROTOCOL *This)
{
  // Set Dummy Button State
  CurrentButton = SCAN_NULL;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KeyNotify (IN EFI_KEY_DATA *KeyData)
{
  // Set Button State
  switch (KeyData->Key.ScanCode) {
    case SCAN_UP:
    case SCAN_DOWN:
    case SCAN_ESC:
    case SCAN_DELETE:
      CurrentButton = KeyData->Key.ScanCode;
      break;

    default:
      break;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SetupKeyNotify (IN EFI_HANDLE ImageHandle)
{
  EFI_STATUS                         Status               = EFI_SUCCESS;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *mSimpleInExProtocol  = NULL;
  EFI_DEVICE_PATH_PROTOCOL          *TempKeypadDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath;
  EFI_HANDLE                         KeypadHandle         = NULL;
  EFI_KEY_DATA                       KeyData              = {0};
  VOID                              *NotifyHandle         = NULL;

  // Set General Key Data
  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;
  KeyData.Key.UnicodeChar         = 0;

  // Locate Device Path of Keypad
  Status = gBS->LocateDevicePath (&gEfiSimpleTextInputExProtocolGuid, &TempKeypadDevicePath, &KeypadHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate the Device Path of Keypad! Status = %r\n", Status));
    return Status;
  }

  // Get SimpleInEx Protocol
  Status = gBS->OpenProtocol (KeypadHandle, &gEfiSimpleTextInputExProtocolGuid, (VOID *)&mSimpleInExProtocol, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get SimpleInEx Protocol! Status = %r\n", Status));
    return Status;
  }

  // Reset Keypad Device
  Status = mSimpleInExProtocol->Reset (mSimpleInExProtocol, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Reset Keypad Device! Status = %r\n", Status));
  }

  // Set Key Scan Code
  KeyData.Key.ScanCode = SCAN_UP;

  // Register Key Notify for UEFI Menu
  Status = mSimpleInExProtocol->RegisterKeyNotify (mSimpleInExProtocol, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify for the UEFI Menu! Status = %r\n", Status));
    return Status;
  }

  // Set Key Scan Code
  KeyData.Key.ScanCode = SCAN_DOWN;

  // Register Key Notify for Special App
  Status = mSimpleInExProtocol->RegisterKeyNotify (mSimpleInExProtocol, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify for the Special App! Status = %r\n", Status));
    return Status;
  }

  return Status;
}

STATIC MS_BUTTON_SERVICES_PROTOCOL mButtonServicesProtocol = {
  SpecialAppButtonCheck,
  UefiMenuButtonCheck,
  ClearButtonState
};

EFI_STATUS
EFIAPI
InitButtonService (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS  Status;
  EFI_HANDLE *HandleBuffer;
  UINTN       HandleCount;

  // Locate Keypad Controller Protocols
  Status = gBS->LocateHandleBuffer (ByProtocol, &gKeypadDeviceProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (!EFI_ERROR (Status)) {
    // Start all Keypad Controllers
    for (UINTN i = 0; i < HandleCount; i++) {
      gBS->ConnectController (HandleBuffer[i], NULL, NULL, TRUE);
    }
  }

  // Setup Key Notify
  Status = SetupKeyNotify (ImageHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Register Button Service Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMsButtonServicesProtocolGuid, &mButtonServicesProtocol, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Button Service Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
