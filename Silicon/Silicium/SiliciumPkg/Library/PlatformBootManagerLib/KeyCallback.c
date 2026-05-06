/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/SimpleTextInEx.h>

#include "PlatformBootManager.h"

//
// Global Variables
//
STATIC EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *mSimpleInExProtocol = NULL;
STATIC VOID                              *KeyNotifyHandle[]   = { NULL, NULL };
STATIC BOOLEAN                            EnterFfuMode        = FALSE;
STATIC BOOLEAN                            EnterAlternativeApp = FALSE;

EFI_STATUS
EFIAPI
KeyNotify (IN EFI_KEY_DATA *KeyData)
{
  // Check for SCAN_UP
  if (KeyData->Key.ScanCode == SCAN_UP) {
    EnterFfuMode = TRUE;
  }

  // Check for SCAN_DOWN
  if (KeyData->Key.ScanCode == SCAN_DOWN) {
    EnterAlternativeApp = TRUE;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SetupKeypad (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  EFI_STATUS  Status;
  EFI_HANDLE  StiHandle;
  EFI_HANDLE *KeypadBuffer;
  UINTN       KeypadCount;

  // Locate Keypad Controller Protocols
  Status = gBS->LocateHandleBuffer (ByProtocol, &gKeypadDeviceProtocolGuid, NULL, &KeypadCount, &KeypadBuffer);
  if (!EFI_ERROR (Status)) {
    // Start all Keypad Controllers
    for (UINTN i = 0; i < KeypadCount; i++) {
      gBS->ConnectController (KeypadBuffer[i], NULL, NULL, TRUE);
    }

    // Free Buffer
    FreePool (KeypadBuffer);
  }

  // Locate STI Protocol Handle
  Status = gBS->LocateDevicePath (&gEfiSimpleTextInputExProtocolGuid, &DevicePath, &StiHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate STI Device Path! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get STI Protocol from Handle
  Status = gBS->HandleProtocol (StiHandle, &gEfiSimpleTextInputExProtocolGuid, (VOID *)&mSimpleInExProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get STI Protocol from Handle! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Reset STI Device
  Status = mSimpleInExProtocol->Reset (mSimpleInExProtocol, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to STI Device! Status = %r\n", __FUNCTION__, Status));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
RegisterKeyNotify ()
{
  EFI_STATUS Status;

  // Set Key Scan Codes
  UINT8 KeyScanCodes[] = { SCAN_UP, SCAN_DOWN };

  // Verify Array Sizes
  STATIC_ASSERT (ARRAY_SIZE (KeyNotifyHandle) == ARRAY_SIZE (KeyScanCodes), "KeyNotifyHandle and KeyScanCodes don't have the same amount of Elements!");

  // Go thru each Key
  for (UINT8 i = 0; i < ARRAY_SIZE (KeyNotifyHandle); i++) {
    EFI_KEY_DATA KeyData;

    // Set Inital Key Data
    KeyData.KeyState.KeyToggleState = 0;
    KeyData.KeyState.KeyShiftState  = 0;
    KeyData.Key.UnicodeChar         = CHAR_NULL;
    KeyData.Key.ScanCode            = KeyScanCodes[i];

    // Register Key Notify
    Status = mSimpleInExProtocol->RegisterKeyNotify (mSimpleInExProtocol, &KeyData, &KeyNotify, &KeyNotifyHandle[i]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Register Key Notify for Scan Code %u! Status = %r\n", __FUNCTION__, KeyScanCodes[i], Status));
    }
  }

  // Reset Key States
  EnterFfuMode        = FALSE;
  EnterAlternativeApp = FALSE;

  return EFI_SUCCESS;
}

EFI_STATUS
RegisterKeyCallback (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  EFI_STATUS Status;

  // Setup Keypad
  Status = SetupKeypad (DevicePath);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Register Key Notify
  Status = RegisterKeyNotify ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
UnregisterKeyCallback ()
{
  EFI_STATUS Status;

  // Go thru each Key
  for (UINT8 i = 0; i < ARRAY_SIZE (KeyNotifyHandle); i++) {
    // Verify Key Notify
    if (KeyNotifyHandle[i] == NULL) {
      continue;
    }

    // Unregister Key Notify
    Status = mSimpleInExProtocol->UnregisterKeyNotify (mSimpleInExProtocol, KeyNotifyHandle[i]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Unregister Key Notify Nr. %u! Status = %r\n", __FUNCTION__, i, Status));
    }
  }
}

VOID
GetKeyStates (
  OUT BOOLEAN *FfuMode,
  OUT BOOLEAN *AlternativeApp)
{
  // Pass Data
  *FfuMode        = EnterFfuMode;
  *AlternativeApp = EnterAlternativeApp;
}
