/**

  This module installs the MsButtonServicesProtocol.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
//#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

#include <Configuration/BootDevices.h>

#include "MsButtonServiceDxe.h"

/**
  Say Volume Up Button is Pressed because we want to go to Front Page.

  @param[in]     - Button Services Protocol Pointer.
  @param[out]    - Pointer to a boolean Value to receive the Button State.

  @retval        - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootVolumeUpButtonThenPowerButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *PreBootVolumeUpButtonThenPowerButton)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  Bsp                                   = MS_BSP_FROM_BSP(This);
  *PreBootVolumeUpButtonThenPowerButton = (Bsp->ButtonState == VolUpButton);

  return EFI_SUCCESS;
}

/**
  Say Volume Down Button is Pressed because we want to Switch Slots.

  @param[in]     - Button Services Protocol Pointer.
  @param[out]    - Pointer to a boolean Value to receive the Button State.

  @retval        - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootVolumeDownButtonThenPowerButtonCheck (
  IN  MS_BUTTON_SERVICES_PROTOCOL *This,
  OUT BOOLEAN                     *PreBootVolumeDownButtonThenPowerButton)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  Bsp                                     = MS_BSP_FROM_BSP(This);
  *PreBootVolumeDownButtonThenPowerButton = (Bsp->ButtonState == VolDownButton);

  return EFI_SUCCESS;
}

/**
  Clear current button state.

  @param[in]     - Button Services protocol pointer.

  @retval        - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootClearVolumeButtonState (IN MS_BUTTON_SERVICES_PROTOCOL *This)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  Bsp              = MS_BSP_FROM_BSP(This);
  Bsp->ButtonState = NoButtons;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KeyNotify (IN EFI_KEY_DATA *KeyData)
{
  if (gBsp == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (KeyData->Key.ScanCode == SCAN_UP) {
    gBsp->ButtonState = VolUpButton;
  } else if (KeyData->Key.ScanCode == SCAN_DOWN) {
    gBsp->ButtonState = VolDownButton;
  }

  return EFI_SUCCESS;
}

/**
  GetButtonState gets the Button State of the Vol+/Vol- Buttons, and Stores that
  State in gButtonState.

  @param[in]     - Gpio Button Services Protocol pointer.

  @return Status - The EFI_STATUS returned by this Function.
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
  KeyData.Key.ScanCode            = SCAN_UP;

  // Register Key Notify for Volume Up
  Status = SimpleEx->RegisterKeyNotify (SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify of Volume Up! Status = %r\n", Status));
    goto exit;
  }

  KeyData.Key.ScanCode            = SCAN_DOWN;

  // Register Key Notify for Volume Down
  Status = SimpleEx->RegisterKeyNotify (SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify of Volume Down! Status = %r\n", Status));
  }

exit:
  return Status;
}

/**
  Constructor Routine to install Button Services Protocol and Initialize anything
  related to Buttons.

  @param[in]     - Image Handle of the Process Loading this Driver.
  @param[in]     - EFI System Table Pointer.

  @retval Status - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
InitButtonService (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Allocate Memory for the Ms Button Service Protocol
  gBsp = AllocateZeroPool(sizeof(GPIO_BUTTON_SERVICES_PROTOCOL));
  if (gBsp == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Button Service Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Define each Button Checks
  gBsp->ButtonServicesProtocol.PreBootVolumeDownButtonThenPowerButtonCheck = PreBootVolumeDownButtonThenPowerButtonCheck;
  gBsp->ButtonServicesProtocol.PreBootVolumeUpButtonThenPowerButtonCheck   = PreBootVolumeUpButtonThenPowerButtonCheck;
  gBsp->ButtonServicesProtocol.PreBootClearVolumeButtonState               = PreBootClearVolumeButtonState;
  gBsp->ButtonState                                                        = NoButtons;

  // Get the State of the Buttons
  Status = GetButtonState();
  if (EFI_ERROR (Status)) {
    FreePool(gBsp);
    goto exit;
  }

  // Register the Ms Button Service Protocol
  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gMsButtonServicesProtocolGuid, gBsp, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Button Service Protocol! Status = %r\n", Status));
    FreePool(gBsp);
  }

exit:
  return Status;
}
