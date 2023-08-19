/** @file GpioButtons.c

 This module installs the MsButtonServicesProtocol.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi.h>

#include <Configuration/BootDevices.h>
#include <Protocol/ButtonServices.h>
#include <Protocol/SimpleTextInEx.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

typedef enum { NoButtons = 0, VolUpButton = 1, VolDownButton = 2 } BUTTON_STATE;

typedef struct {
  MS_BUTTON_SERVICES_PROTOCOL ButtonServicesProtocol;
  BUTTON_STATE                ButtonState;
} GPIO_BUTTON_SERVICES_PROTOCOL;

GPIO_BUTTON_SERVICES_PROTOCOL *gBsp = NULL;

#define MS_BSP_FROM_BSP(a)                                                     \
  BASE_CR(a, GPIO_BUTTON_SERVICES_PROTOCOL, ButtonServicesProtocol)

/*
Say volume up button is pressed because we want to go to Front Page.

@param[in]     - Button Services protocol pointer
@param[out]    - Pointer to a boolean value to receive the button state

@retval        - EFI_SUCCESS;
*/
EFI_STATUS
EFIAPI
PreBootVolumeUpButtonThenPowerButtonCheck(
    IN MS_BUTTON_SERVICES_PROTOCOL *This,
    OUT BOOLEAN *PreBootVolumeUpButtonThenPowerButton // TRUE if button combo
                                                      // set else FALSE
)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  DEBUG((DEBUG_VERBOSE, "%a \n", __FUNCTION__));

  Bsp                                   = MS_BSP_FROM_BSP(This);
  *PreBootVolumeUpButtonThenPowerButton = (Bsp->ButtonState == VolUpButton);
  return EFI_SUCCESS;
}

/*
Say volume down button is pressed because we want alt boot.

@param[in]     - Button Services protocol pointer
@param[out]    - Pointer to a boolean value to receive the button state

@retval        - EFI_SUCCESS;
*/
EFI_STATUS
EFIAPI
PreBootVolumeDownButtonThenPowerButtonCheck(
    IN MS_BUTTON_SERVICES_PROTOCOL *This,
    OUT BOOLEAN *PreBootVolumeDownButtonThenPowerButton // TRUE if button combo
                                                        // set else FALSE
)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  DEBUG((DEBUG_VERBOSE, "%a \n", __FUNCTION__));
  Bsp                                     = MS_BSP_FROM_BSP(This);
  *PreBootVolumeDownButtonThenPowerButton = (Bsp->ButtonState == VolDownButton);
  return EFI_SUCCESS;
}

/*
Clear current button state.

@param[in]     - Button Services protocol pointer

@retval        - EFI_SUCCESS;
*/
EFI_STATUS
EFIAPI
PreBootClearVolumeButtonState(IN MS_BUTTON_SERVICES_PROTOCOL *This)
{
  GPIO_BUTTON_SERVICES_PROTOCOL *Bsp;

  DEBUG((DEBUG_VERBOSE, "%a \n", __FUNCTION__));
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
    DEBUG((DEBUG_INFO, "%a: Vol+ Button Detected\n", __FUNCTION__));
  }
  else if (KeyData->Key.ScanCode == SCAN_DOWN) {
    gBsp->ButtonState = VolDownButton;
    DEBUG((DEBUG_INFO, "%a: Vol- Button Detected\n", __FUNCTION__));
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
  EFI_STATUS                         Status;
  EFI_HANDLE                         Handle = NULL;
  EFI_DEVICE_PATH_PROTOCOL          *ButtonsDxeDevicePath;
  EFI_KEY_DATA                       KeyData;
  VOID                              *NotifyHandle;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;

  Status = EFI_SUCCESS;

  DEBUG((DEBUG_VERBOSE, "%a: Entry\n", __FUNCTION__));

  ButtonsDxeDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath;

  Status = gBS->LocateDevicePath(
      &gEfiSimpleTextInputExProtocolGuid, &ButtonsDxeDevicePath, &Handle);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to locate ButtonsDxe device path for button service protocol, "
         "Status = %r.\n",
         Status));
    goto Exit;
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
    goto Exit;
  }

  // Prevent Key persistence when chainloaded
  SimpleEx->Reset(SimpleEx, TRUE);

  KeyData.KeyState.KeyToggleState = 0;
  KeyData.KeyState.KeyShiftState  = 0;

  KeyData.Key.UnicodeChar = 0;
  KeyData.Key.ScanCode    = SCAN_UP;

  Status = SimpleEx->RegisterKeyNotify(
      SimpleEx, &KeyData, &KeyNotify, &NotifyHandle);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to register volume up notification for button service "
         "protocol, "
         "Status = %r.\n",
         Status));
    goto Exit;
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
    goto Exit;
  }

Exit:
  return Status;
}

/**
 Constructor routine to install button services protocol and initialize anything
related to buttons


@param[in]     - Image Handle of the process loading this module
@param[in]     - Efi System Table pointer

@retval        - EFI_SUCCESS (always for a constructor)
 **/
EFI_STATUS
EFIAPI
ButtonsInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG((DEBUG_INFO, "%a \n", __FUNCTION__));

  gBsp = AllocateZeroPool(sizeof(GPIO_BUTTON_SERVICES_PROTOCOL));
  if (gBsp == NULL) {
    DEBUG(
        (DEBUG_ERROR,
         "Failed to allocate memory for button service protocol.\n"));
    return EFI_SUCCESS;
  }

  gBsp->ButtonServicesProtocol.PreBootVolumeDownButtonThenPowerButtonCheck =
      PreBootVolumeDownButtonThenPowerButtonCheck;
  gBsp->ButtonServicesProtocol.PreBootVolumeUpButtonThenPowerButtonCheck =
      PreBootVolumeUpButtonThenPowerButtonCheck;
  gBsp->ButtonServicesProtocol.PreBootClearVolumeButtonState =
      PreBootClearVolumeButtonState;
  gBsp->ButtonState = NoButtons;

  Status = GetButtonState();
  if (EFI_ERROR(Status)) {
    FreePool(gBsp);
    return Status;
  }

  // Install the protocol
  Status = gBS->InstallMultipleProtocolInterfaces(
      &ImageHandle, &gMsButtonServicesProtocolGuid, gBsp, NULL);

  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "Button Services Protocol Publisher: install protocol error, Status = "
         "%r.\n",
         Status));
    FreePool(gBsp);
    return Status;
  }

  DEBUG((DEBUG_INFO, "Button Services Protocol Installed!\n"));
  return EFI_SUCCESS;
}