/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

#include <Configuration/BootDevices.h>

#include "MsButtonService.h"

//
// Global Variables
//
STATIC MS_BUTTON_SERVICES *gButtonService;

EFI_STATUS
EFIAPI
UefiMenuButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  // Get Button Service
  MS_BUTTON_SERVICES *ButtonService = MS_BSP_FROM_BSP (This);

  // Get Button Press State
  *ButtonPressed = (ButtonService->ButtonState == VolUpButton);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SpecialAppButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  // Get Button Service
  MS_BUTTON_SERVICES *ButtonService = MS_BSP_FROM_BSP (This);

  // Get Button Press State
  *ButtonPressed = (ButtonService->ButtonState == VolDownButton);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ClearButtonState (IN MS_BUTTON_SERVICES_PROTOCOL *This)
{
  // Get Button Service
  MS_BUTTON_SERVICES *ButtonService = MS_BSP_FROM_BSP (This);

  // Set Button Press State
  ButtonService->ButtonState = NoButtons;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KeyNotify (IN EFI_KEY_DATA *KeyData)
{
  // Check for Key Press
  if (KeyData->Key.ScanCode == SCAN_UP) {
    gButtonService->ButtonState = VolUpButton;
  } else if (KeyData->Key.ScanCode == SCAN_DOWN) {
    gButtonService->ButtonState = VolDownButton;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetButtonStates ()
{
  EFI_STATUS                         Status            = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL          *ButtonsDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *mSimpleExProtocol = NULL;
  EFI_KEY_DATA                       KeyData           = {0};
  EFI_HANDLE                         Handle            = NULL;
  VOID                              *NotifyHandle      = NULL;

  // Locate Device Path of Buttons
  Status = gBS->LocateDevicePath (&gEfiSimpleTextInputExProtocolGuid, &ButtonsDevicePath, &Handle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate the Device Path of Buttons! Status = %r\n", Status));
    return Status;
  }

  // Open the Protocol of the Buttons Driver
  Status = gBS->OpenProtocol (Handle, &gEfiSimpleTextInputExProtocolGuid, (VOID *)&mSimpleExProtocol, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Open Buttons Driver Protocol! Status = %r\n", Status));
    return Status;
  }

  // Reset Button States
  mSimpleExProtocol->Reset (mSimpleExProtocol, TRUE);

  // Set Dummy UEFI Menu Button State
  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;
  KeyData.Key.UnicodeChar         = 0;
  KeyData.Key.ScanCode            = SCAN_UP;

  // Register Key Notify for UEFI Menu Button
  Status = mSimpleExProtocol->RegisterKeyNotify (mSimpleExProtocol, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify for the UEFI Menu Button! Status = %r\n", Status));
    return Status;
  }

  // Set Dummy Special App Button State
  KeyData.Key.ScanCode = SCAN_DOWN;

  // Register Key Notify for Special App Button
  Status = mSimpleExProtocol->RegisterKeyNotify (mSimpleExProtocol, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify for the Special App Button! Status = %r\n", Status));
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
InitButtonService (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS  Status;
  EFI_HANDLE *HandleBuffer;
  UINTN       HandleCount;

  // Allocate Memory
  gButtonService = AllocateZeroPool (sizeof (MS_BUTTON_SERVICES));
  if (gButtonService == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Button Service Protocol!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Set Protocol Functions
  gButtonService->ButtonServicesProtocol.PreBootVolumeUpButtonThenPowerButtonCheck   = UefiMenuButtonCheck;
  gButtonService->ButtonServicesProtocol.PreBootVolumeDownButtonThenPowerButtonCheck = SpecialAppButtonCheck;
  gButtonService->ButtonServicesProtocol.PreBootClearVolumeButtonState               = ClearButtonState;
  gButtonService->ButtonState                                                        = NoButtons;

  // Locate Keypad Controller Protocols
  Status = gBS->LocateHandleBuffer (ByProtocol, &gKeypadDeviceProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (!EFI_ERROR (Status)) {
    // Start all Keypad Controllers
    for (UINTN i = 0; i < HandleCount; i++) {
      gBS->ConnectController (HandleBuffer[i], NULL, NULL, TRUE);
    }
  }

  // Get Button States
  Status = GetButtonStates ();
  if (EFI_ERROR (Status)) {
    FreePool (gButtonService);
    return Status;
  }

  // Register Button Service Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMsButtonServicesProtocolGuid, gButtonService, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Button Service Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
