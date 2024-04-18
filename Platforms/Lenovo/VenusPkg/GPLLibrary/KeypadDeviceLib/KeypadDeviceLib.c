/**
  Copyright (c) 2009-2011, NVIDIA Corporation.

  SPDX-License-Identifier: GPL-2.0-or-later
**/
#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/BitmapLib.h>
#include <Library/KeypadDeviceLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>

#include <Protocol/EfiTegraGpio.h>

// Core Variables
EFI_TEGRA_GPIO_PROTOCOL         *mTegraGpioProtocol;
ARM_MEMORY_REGION_DESCRIPTOR_EX  KbcMemoryRegion;

// Enable Values
BOOLEAN EnableKeyboard    = TRUE;
BOOLEAN EnableGpioButtons = TRUE;

// KBC Registers
#define KBC_MAX_KPENT   8
#define KBC_RPT_DLY_0   0x2C
#define KBC_INIT_DLY_0  0x28
#define KBC_KP_ENT0_0   0x30
#define KBC_INT_0       0x4

// Scan Code Key Map
STATIC UINT16 KeyMapScanCode[16][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {SCAN_F2, SCAN_F1, SCAN_ESC, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, SCAN_F11, SCAN_F10, 0, 0, 0, 0, SCAN_DOWN},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {SCAN_F7, SCAN_F6, SCAN_F5, 0, 0, 0, 0, 0},
  {SCAN_F8, SCAN_F9, 0, 0, 0, SCAN_LEFT, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, SCAN_UP},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, SCAN_DELETE, SCAN_PAGE_UP, SCAN_INSERT, SCAN_F12, SCAN_PAGE_DOWN, SCAN_RIGHT}
};

// Unicode Char Key Map
STATIC CHAR16 KeyMapUnicodeChar[16][8] = {
  {0, CHAR_TAB, '`', '1', 'q', 'a', 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '3', 'e', 'd', 'c', ' '},
  {0, 0, 0, '2', 'w', 's', 'x', 'z'},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'g', 't', '5', '4', 'r', 'f', 'v', 'b'},
  {'h', 'y', '6', '7', 'u', 'j', 'm', 'n'},
  {0, 0, 0, '9', 'o', 'l', '.', 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '8', 'i', 'k', ',', 0},
  {0, 0, '\\', 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'-', 0, '0', 'p', 0, ';', '/', 0},
  {0, 0, '=', CHAR_BACKSPACE, 0, 0, 0, CHAR_CARRIAGE_RETURN},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

/*
// Unicode Char Key Map (Shift State)
STATIC CHAR16 KeyMapUnicodeCharShift[16][8] = {
  {0, CHAR_TAB, '~', '!', 'Q', 'A', 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '#', 'E', 'D', 'C', ' '},
  {0, 0, 0, '@', 'W', 'S', 'X', 'Z'},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'G', 'T', '%', '$', 'R', 'F', 'V', 'B'},
  {'H', 'Y', '^', '&', 'U', 'J', 'M', 'N'},
  {0, 0, 0, '(', 'O', 'L', '>', 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, '*', 'I', 'K', '<', 0},
  {0, 0, '|', 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {'_', 0, ')', 'P', 0, ':', '?', 0},
  {0, 0, '+', CHAR_BACKSPACE, 0, 0, '"', CHAR_CARRIAGE_RETURN},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
*/

typedef struct {
  KEY_CONTEXT EfiKeyContext;
  UINTN       Gpio;
} KEY_CONTEXT_PRIVATE;

UINTN gBitmapScanCodes[BITMAP_NUM_WORDS(0x18)]    = {0};
UINTN gBitmapUnicodeChars[BITMAP_NUM_WORDS(0x7F)] = {0};

EFI_KEY_DATA gKeyDataPowerDown      = {.Key = {.ScanCode = SCAN_RIGHT,}};
EFI_KEY_DATA gKeyDataPowerUp        = {.Key = {.ScanCode = SCAN_LEFT,}};
EFI_KEY_DATA gKeyDataPowerLongpress = {.Key = {.ScanCode = SCAN_ESC,}};

#define MS2NS(ms) (((UINT64)(ms)) * 1000000ULL)

STATIC
inline
VOID
KeySetState (
  UINT16  ScanCode,
  CHAR16  UnicodeChar,
  BOOLEAN Value)
{
  if (ScanCode && ScanCode < 0x18) {
    if (Value) {
      BitmapSet(gBitmapScanCodes, ScanCode);
    } else {
      BitmapClear(gBitmapScanCodes, ScanCode);
    }
  }

  if (UnicodeChar && UnicodeChar < 0x7F) {
    if (Value) {
      BitmapSet(gBitmapUnicodeChars, ScanCode);
    } else {
      BitmapClear(gBitmapUnicodeChars, ScanCode);
    }
  }
}

STATIC
inline
VOID
LibKeyInitializeKeyContext (KEY_CONTEXT *Context)
{
  SetMem(&Context->KeyData, sizeof(Context->KeyData), 0);

  Context->Time      = 0;
  Context->State     = KEYSTATE_RELEASED;
  Context->Repeat    = FALSE;
  Context->Longpress = FALSE;
}

STATIC
inline
VOID
LibKeyUpdateKeyStatus (
  KEY_CONTEXT       *Context,
  KEYPAD_RETURN_API *KeypadReturnApi,
  BOOLEAN            IsPressed,
  UINT64             Delta)
{
  // Keep Track of the Actual State
  KeySetState (Context->KeyData.Key.ScanCode, Context->KeyData.Key.UnicodeChar, IsPressed);

  // Update Key Time
  Context->Time += Delta;

  switch (Context->State) {
    case KEYSTATE_RELEASED:
      if (IsPressed) {
        // Change to Pressed
        Context->Time  = 0;
        Context->State = KEYSTATE_PRESSED;
      }

      break;

    case KEYSTATE_PRESSED:
      if (IsPressed) {
        // Key Repeat
        if (Context->Repeat && Context->Time >= MS2NS(100)) {
          KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &Context->KeyData);
          Context->Time   = 0;
          Context->Repeat = TRUE;
        } else if (!Context->Longpress && Context->Time >= MS2NS(500)) {
          KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &Context->KeyData);
          Context->Time   = 0;
          Context->Repeat = TRUE;
        }

        Context->Longpress = TRUE;
      } else {
        if (!Context->Longpress) {
          // We Supressed Down, so Report it Now
          KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &Context->KeyData);
          Context->State = KEYSTATE_LONGPRESS_RELEASE;
        } else if (Context->Time >= MS2NS(10)) {
          // We Reported another Key Already
          Context->Time      = 0;
          Context->Repeat    = FALSE;
          Context->Longpress = FALSE;
          Context->State     = KEYSTATE_RELEASED;
        }
      }

      break;

    case KEYSTATE_LONGPRESS_RELEASE:
      // Change to Released
      Context->Time      = 0;
      Context->Repeat    = FALSE;
      Context->Longpress = FALSE;
      Context->State     = KEYSTATE_RELEASED;
      break;

    default:
      ASSERT(FALSE);
      break;
  }
}

STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeUp;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeDown;
STATIC KEY_CONTEXT_PRIVATE KeyContextPower;
STATIC KEY_CONTEXT_PRIVATE KeyContextScreenRotate;
STATIC KEY_CONTEXT_PRIVATE KeyContextWindowsScreen;

STATIC KEY_CONTEXT_PRIVATE *KeyList[] = { &KeyContextVolumeUp, &KeyContextVolumeDown, &KeyContextPower, &KeyContextScreenRotate, &KeyContextWindowsScreen };

STATIC
VOID
KeypadInitializeKeyContextPrivate (KEY_CONTEXT_PRIVATE *Context)
{
  Context->Gpio = 0;
}

STATIC
KEY_CONTEXT_PRIVATE*
KeypadKeyCodeToKeyContext (UINT32 KeyCode)
{
  if (KeyCode == 115) {
    return &KeyContextVolumeUp;
  } else if (KeyCode == 116) {
    return &KeyContextVolumeDown;
  } else if (KeyCode == 117) {
    return &KeyContextPower;
  } else if (KeyCode == 118) {
    return &KeyContextScreenRotate;
  } else if (KeyCode == 119) {
    return &KeyContextWindowsScreen;
  }

  return NULL;
}

RETURN_STATUS
EFIAPI
KeypadDeviceConstructor ()
{
  EFI_STATUS           Status;
  KEY_CONTEXT_PRIVATE *StaticContext;
  UINTN                Index;

  // Get KBC Memory Region
  Status = LocateMemoryMapAreaByName ("Keyboard Controller", &KbcMemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get Keyboard Controller Memory Region! Status = %r\n", Status));
    EnableKeyboard = FALSE;
  } else {
    // Set Hardware KBC Delay
    MmioWrite32(KbcMemoryRegion.Address + KBC_RPT_DLY_0,  1200);
    MmioWrite32(KbcMemoryRegion.Address + KBC_INIT_DLY_0, 1200);
  }

  // Locate Tegra Gpio Protocol
  Status = gBS->LocateProtocol (&gEfiTegraGpioProtocolGuid, NULL, (VOID *)&mTegraGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Tegra GPIO Protocol! Status = %r\n", Status));
    EnableGpioButtons = FALSE;
  } else {
    // Reset all keys
    for (Index = 0; Index < (sizeof(KeyList) / sizeof(KeyList[0])); Index++) {
      KeypadInitializeKeyContextPrivate(KeyList[Index]);
    }

    // Configure keys
    /// Volume Up Button
    StaticContext       = KeypadKeyCodeToKeyContext (115);
    StaticContext->Gpio = TEGRA_GPIO(K, 5);

    /// Volume Down Button
    StaticContext       = KeypadKeyCodeToKeyContext (116);
    StaticContext->Gpio = TEGRA_GPIO(K, 6);

    /// Power Button
    StaticContext       = KeypadKeyCodeToKeyContext (117);
    StaticContext->Gpio = TEGRA_GPIO(V, 0);

    /// Screen Rotate Button
    StaticContext       = KeypadKeyCodeToKeyContext (118);
    StaticContext->Gpio = TEGRA_GPIO(K, 4);

    /// Windows Screen Button
    StaticContext       = KeypadKeyCodeToKeyContext (119);
    StaticContext->Gpio = TEGRA_GPIO(O, 5);
  }

  return RETURN_SUCCESS;
}

EFI_STATUS
EFIAPI
KeypadDeviceReset (KEYPAD_DEVICE_PROTOCOL *This)
{
  if (EnableGpioButtons) {
    LibKeyInitializeKeyContext(&KeyContextVolumeUp.EfiKeyContext);
    KeyContextVolumeUp.EfiKeyContext.KeyData.Key.ScanCode        = SCAN_VOLUME_UP;

    LibKeyInitializeKeyContext(&KeyContextVolumeDown.EfiKeyContext);
    KeyContextVolumeDown.EfiKeyContext.KeyData.Key.ScanCode      = SCAN_VOLUME_DOWN;

    LibKeyInitializeKeyContext(&KeyContextPower.EfiKeyContext);
    KeyContextPower.EfiKeyContext.KeyData.Key.ScanCode           = SCAN_SUSPEND;

    LibKeyInitializeKeyContext(&KeyContextScreenRotate.EfiKeyContext);
    KeyContextScreenRotate.EfiKeyContext.KeyData.Key.ScanCode    = SCAN_TOGGLE_DISPLAY;

    LibKeyInitializeKeyContext(&KeyContextWindowsScreen.EfiKeyContext);
    KeyContextWindowsScreen.EfiKeyContext.KeyData.Key.ScanCode   = SCAN_HOME;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
KeypadDeviceGetKeys (
  KEYPAD_DEVICE_PROTOCOL *This,
  KEYPAD_RETURN_API      *KeypadReturnApi,
  UINT64                  Delta)
{
  UINT32 fifo_cnt;

  if (EnableKeyboard) {
    fifo_cnt = (MmioRead32(KbcMemoryRegion.Address + KBC_INT_0) >> 4) & 0xF;
  }

  if (EnableKeyboard && fifo_cnt) {
    for (UINT32 i = 0; i < KBC_MAX_KPENT; i++) {
      UINT32 kp_ent0;

      // Get Next Word
      if ((i % 4) == 0) {
        kp_ent0 = MmioRead32 (KbcMemoryRegion.Address + KBC_KP_ENT0_0 + i);
      }

      if (kp_ent0 & 0x80) {
        // Get the Location of the Pressed Key
        UINT32 Column = kp_ent0 & 0x7;
        UINT32 Row    = (kp_ent0 >> 3) & 0xF;

        // Wait a Bit before Processing the Next Key
        MicroSecondDelay (80000);

        // TODO: Add Shift and Shift Lock Checks here.

        // Send Key Input
        if (KeyMapScanCode[Row][Column] != 0) {
          EFI_KEY_DATA KeyPressed = {.Key = {.ScanCode = KeyMapUnicodeChar[Row][Column]}};
          KeypadReturnApi->PushEfikeyBufTail(KeypadReturnApi, &KeyPressed);
        } else if (KeyMapUnicodeChar[Row][Column] != 0) {
          EFI_KEY_DATA KeyPressed = {.Key = {.UnicodeChar = KeyMapUnicodeChar[Row][Column]}};
          KeypadReturnApi->PushEfikeyBufTail(KeypadReturnApi, &KeyPressed);
        }
      }
    }
  } else if (EnableGpioButtons) {
    BOOLEAN IsPressed = FALSE;

    for (UINTN Index = 0; Index < (sizeof(KeyList) / sizeof(KeyList[0])); Index++) {
      KEY_CONTEXT_PRIVATE *Context    = KeyList[Index];

      // Get Current State of GPIO Key
      IsPressed = !mTegraGpioProtocol->GetState (Context->Gpio);

      LibKeyUpdateKeyStatus (&Context->EfiKeyContext, KeypadReturnApi, IsPressed, Delta);
    }
  }

  return EFI_SUCCESS;
}