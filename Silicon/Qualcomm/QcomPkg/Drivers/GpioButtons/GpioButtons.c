/** @file GpioButtons.c

  This module installs the MsButtonServicesProtocol.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

#include <Configuration/BootDevices.h>

#include "GpioButtons.h"

/**
  Say volume up button is pressed because we want to go to Front Page.

  @param[in]     - Button Services protocol pointer
  @param[out]    - Pointer to a boolean value to receive the button state

  @retval        - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootVolumeUpButtonThenPowerButtonCheck(
    IN  MS_BUTTON_SERVICES_PROTOCOL *This,
    OUT BOOLEAN                     *PreBootVolumeUpButtonThenPowerButton)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  Bsp                                   = MS_BSP_FROM_BSP(This);
  *PreBootVolumeUpButtonThenPowerButton = (Bsp->ButtonState == VolUpButton);

  return EFI_SUCCESS;
}

/**
  Say volume down button is pressed because we want alt boot.

  @param[in]     - Button Services protocol pointer
  @param[out]    - Pointer to a boolean value to receive the button state

  @retval        - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootVolumeDownButtonThenPowerButtonCheck(
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

  @param[in]     - Button Services protocol pointer

  @retval        - EFI_SUCCESS;
**/
EFI_STATUS
EFIAPI
PreBootClearVolumeButtonState(IN MS_BUTTON_SERVICES_PROTOCOL *This)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  Bsp              = MS_BSP_FROM_BSP(This);
  Bsp->ButtonState = NoButtons;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KeyNotify(IN EFI_KEY_DATA *KeyData)
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
  GetButtonState gets the button state of the Vol+/Vol- buttons, and stores that
  state in gButtonState.

  @param[in]    - Gpio Button Services Protocol pointer

  @return EFI_SUCCESS - String buffer returned to caller
  @return EFI_ERROR   - Error the string
**/
EFI_STATUS
GetButtonState()
{
  EFI_STATUS                         Status               = EFI_SUCCESS;
  EFI_HANDLE                         Handle               = NULL;
  EFI_DEVICE_PATH_PROTOCOL          *ButtonsDxeDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath;
  EFI_KEY_DATA                       KeyData;
  VOID                              *NotifyHandle         = NULL;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx             = NULL;

  Status = gBS->LocateDevicePath(&gEfiSimpleTextInputExProtocolGuid, &ButtonsDxeDevicePath, &Handle);
  if (EFI_ERROR( Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Device Path of Buttons! Status = %r\n", Status));
    goto exit;
  }

  Status = gBS->OpenProtocol(Handle, &gEfiSimpleTextInputExProtocolGuid, (VOID **)&SimpleEx, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Open Buttons Driver Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Prevent Key persistence when chainloaded
  SimpleEx->Reset(SimpleEx, TRUE);

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;

  KeyData.Key.UnicodeChar        = 0;
  KeyData.Key.ScanCode           = SCAN_UP;

  Status = SimpleEx->RegisterKeyNotify(SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify of Volume Up! Status = %r\n", Status));
    goto exit;
  }

  KeyData.Key.ScanCode = SCAN_DOWN;

  Status = SimpleEx->RegisterKeyNotify(SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Key Notify of Volume Down! Status = %r\n", Status));
  }

exit:
  return Status;
}

/**
  Constructor routine to install button services protocol and initialize anything
  related to buttons

  @param[in]     - Image Handle of the process loading this module
  @param[in]     - Efi System Table pointer

  @retval        - EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ButtonsInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  gBsp = AllocateZeroPool(sizeof(GPIO_BUTTON_SERVICES_PROTOCOL));
  if (gBsp == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Button Service Protocol! Status = %r\n", Status));
    goto exit;
  }

  gBsp->ButtonServicesProtocol.PreBootVolumeDownButtonThenPowerButtonCheck = PreBootVolumeDownButtonThenPowerButtonCheck;
  gBsp->ButtonServicesProtocol.PreBootVolumeUpButtonThenPowerButtonCheck   = PreBootVolumeUpButtonThenPowerButtonCheck;
  gBsp->ButtonServicesProtocol.PreBootClearVolumeButtonState               = PreBootClearVolumeButtonState;
  gBsp->ButtonState                                                        = NoButtons;

  Status = GetButtonState();
  if (EFI_ERROR (Status)) {
    FreePool(gBsp);
    goto exit;
  }

  // Install the protocol
  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gMsButtonServicesProtocolGuid, gBsp, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Button Service Protocol! Status = %r\n", Status));
    FreePool(gBsp);
  }

exit:
  return Status;
}