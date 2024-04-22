/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiBootManagerLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

#include <Configuration/BootDevices.h>

#include "MsButtonService.h"

#if HAS_BUILD_IN_KEYBOARD == 1
/**
  Say ESC Button is Pressed because we want to go to Front Page.

  @param[in]  This          - Button Services Protocol Pointer.
  @param[out] ButtonPressed - Pointer to a boolean Value to receive the Button State.

  @retval Status            - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootESCButtonThenPowerButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  MS_BUTTON_SERVICES *ButtonService;

   ButtonService  = MS_BSP_FROM_BSP(This);
  *ButtonPressed  = (ButtonService->ButtonState == ESCButton);

  return EFI_SUCCESS;
}

/**
  Say Entf Button is Pressed because we want to run Special Application.

  @param[in]  This          - Button Services Protocol Pointer.
  @param[out] ButtonPressed - Pointer to a boolean Value to receive the Button State.

  @retval Status            - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootEntfButtonThenPowerButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  MS_BUTTON_SERVICES *ButtonService;

   ButtonService  = MS_BSP_FROM_BSP (This);
  *ButtonPressed  = (ButtonService->ButtonState == EntfButton);

  return EFI_SUCCESS;
}
#else
/**
  Say Volume Up Button is Pressed because we want to go to Front Page.

  @param[in]  This          - Button Services Protocol Pointer.
  @param[out] ButtonPressed - Pointer to a boolean Value to receive the Button State.

  @retval Status            - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootVolumeUpButtonThenPowerButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  MS_BUTTON_SERVICES *ButtonService;

   ButtonService  = MS_BSP_FROM_BSP(This);
  *ButtonPressed  = (ButtonService->ButtonState == VolUpButton);

  return EFI_SUCCESS;
}

/**
  Say Volume Down Button is Pressed because we want to run Special Application.

  @param[in]  This          - Button Services Protocol Pointer.
  @param[out] ButtonPressed - Pointer to a boolean Value to receive the Button State.

  @retval Status            - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootVolumeDownButtonThenPowerButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *ButtonPressed)
{
  MS_BUTTON_SERVICES *ButtonService;

   ButtonService  = MS_BSP_FROM_BSP (This);
  *ButtonPressed  = (ButtonService->ButtonState == VolDownButton);

  return EFI_SUCCESS;
}
#endif

/**
  Clear current button state.

  @param[in] This           - Button Services protocol pointer.

  @retval Status            - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootClearVolumeButtonState (IN MS_BUTTON_SERVICES_PROTOCOL *This)
{
  MS_BUTTON_SERVICES *ButtonService;

  ButtonService              = MS_BSP_FROM_BSP (This);
  ButtonService->ButtonState = NoButtons;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KeyNotify (IN EFI_KEY_DATA *KeyData)
{
  if (gButtonService == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

#if HAS_BUILD_IN_KEYBOARD == 1
  if (KeyData->Key.ScanCode == SCAN_ESC) {
    gButtonService->ButtonState = ESCButton;
  } else if (KeyData->Key.ScanCode == SCAN_DELETE) {
    gButtonService->ButtonState = EntfButton;
  }
#else
  if (KeyData->Key.ScanCode == SCAN_UP) {
    gButtonService->ButtonState = VolUpButton;
  } else if (KeyData->Key.ScanCode == SCAN_DOWN) {
    gButtonService->ButtonState = VolDownButton;
  }
#endif

  return EFI_SUCCESS;
}

/**
  GetButtonState gets the Button State of the Vol+/Vol- Buttons, and Stores that State in gButtonState.

  @return Status            - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
GetButtonState ()
{
  EFI_STATUS                         Status            = EFI_SUCCESS;
  EFI_HANDLE                         Handle            = NULL;
  EFI_DEVICE_PATH_PROTOCOL          *ButtonsDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath;
  EFI_KEY_DATA                       KeyData;
  VOID                              *NotifyHandle      = NULL;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx          = NULL;

  // Locate Device Path of Buttons
  Status = gBS->LocateDevicePath (&gEfiSimpleTextInputExProtocolGuid, &ButtonsDevicePath, &Handle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Device Path of Buttons! Status = %r\n", Status));
    goto exit;
  }

  // Open the Protocol of the Buttons Driver
  Status = gBS->OpenProtocol (Handle, &gEfiSimpleTextInputExProtocolGuid, (VOID **)&SimpleEx, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Open Buttons Driver Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Prevent Key Persistence when Chainloaded
  SimpleEx->Reset(SimpleEx, TRUE);

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;

  KeyData.Key.UnicodeChar         = 0;
#if HAS_BUILD_IN_KEYBOARD == 1
  KeyData.Key.ScanCode            = SCAN_ESC;
#else
  KeyData.Key.ScanCode            = SCAN_UP;
#endif

  // Register Key Notify for UEFI Menu Button
  Status = SimpleEx->RegisterKeyNotify (SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify of UEFI Menu Button! Status = %r\n", Status));
    goto exit;
  }

#if HAS_BUILD_IN_KEYBOARD == 1
  KeyData.Key.ScanCode            = SCAN_DELETE;
#else
  KeyData.Key.ScanCode            = SCAN_DOWN;
#endif

  // Register Key Notify for Special Application Button
  Status = SimpleEx->RegisterKeyNotify (SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify of Special Application Button! Status = %r\n", Status));
  }

exit:
  return Status;
}

/**
  Constructor Routine to install Button Services Protocol and Initialize anything
  related to Buttons.

  @param[in] ImageHandle    - Image Handle of the Process Loading this Driver.
  @param[in] SystemTable    - EFI System Table Pointer.

  @retval Status            - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
InitButtonService (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Allocate Memory for the Ms Button Service Protocol
  gButtonService = AllocateZeroPool (sizeof(MS_BUTTON_SERVICES));
  if (gButtonService == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Button Service Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Define each Button Checks
#if HAS_BUILD_IN_KEYBOARD == 1
  gButtonService->ButtonServicesProtocol.PreBootVolumeDownButtonThenPowerButtonCheck = PreBootESCButtonThenPowerButtonCheck;
  gButtonService->ButtonServicesProtocol.PreBootVolumeUpButtonThenPowerButtonCheck   = PreBootEntfButtonThenPowerButtonCheck;
#else
  gButtonService->ButtonServicesProtocol.PreBootVolumeDownButtonThenPowerButtonCheck = PreBootVolumeDownButtonThenPowerButtonCheck;
  gButtonService->ButtonServicesProtocol.PreBootVolumeUpButtonThenPowerButtonCheck   = PreBootVolumeUpButtonThenPowerButtonCheck;
#endif
  gButtonService->ButtonServicesProtocol.PreBootClearVolumeButtonState               = PreBootClearVolumeButtonState;
  gButtonService->ButtonState                                                        = NoButtons;

  // Start all Controllers
  // TODO: Replace with better Code
  EfiBootManagerConnectAll ();

  // Get the State of the Buttons
  Status = GetButtonState ();
  if (EFI_ERROR (Status)) {
    FreePool (gButtonService);
    goto exit;
  }

  // Register the Ms Button Service Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMsButtonServicesProtocolGuid, gButtonService, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Button Service Protocol! Status = %r\n", Status));
    FreePool (gButtonService);
  }

exit:
  return Status;
}
