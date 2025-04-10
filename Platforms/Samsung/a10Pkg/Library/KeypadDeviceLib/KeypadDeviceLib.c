#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/BitmapLib.h>
#include <Library/KeypadDeviceLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiGpio.h>

EFI_GPIO_PROTOCOL *mGpioProtocol;

typedef struct {
  KEY_CONTEXT EfiKeyContext;
  UINT64      PinctrlBase;
  UINT32      BankOffset;
  INT32       Pin;
} KEY_CONTEXT_PRIVATE;

UINTN gBitmapScanCodes[BITMAP_NUM_WORDS(0x18)]    = {0};
UINTN gBitmapUnicodeChars[BITMAP_NUM_WORDS(0x7f)] = {0};

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

  if (UnicodeChar && UnicodeChar < 0x7f) {
    if (Value) {
      BitmapSet(gBitmapUnicodeChars, ScanCode);
    } else {
      BitmapClear(gBitmapUnicodeChars, ScanCode);
    }
  }
}

STATIC
inline
BOOLEAN
KeyGetState (
  UINT16 ScanCode,
  CHAR16 UnicodeChar)
{
  if (ScanCode && ScanCode < 0x18) {
    if (!BitmapTest(gBitmapScanCodes, ScanCode)) {
      return FALSE;
    }
  }

  if (UnicodeChar && UnicodeChar < 0x7f) {
    if (!BitmapTest(gBitmapUnicodeChars, ScanCode)) {
      return FALSE;
    }
  }

  return TRUE;
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
          // Handle Key Combos
          if (Context->KeyData.Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            if (KeyGetState (SCAN_DOWN, 0)) {
              // Report SCAN_RIGHT
              KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &gKeyDataPowerDown);
            } else if (KeyGetState (SCAN_UP, 0)) {
              // Report SCAN_LEFT
              KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &gKeyDataPowerUp);
            } else {
              // Report SCAN_ESC
              KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &gKeyDataPowerLongpress);
            }
          } else { // Post first Keyrepeat Event
            // only Start Keyrepeat if we're not doing a Combo
            if (!KeyGetState (0, CHAR_CARRIAGE_RETURN)) {
              KeypadReturnApi->PushEfikeyBufTail (KeypadReturnApi, &Context->KeyData);
              Context->Time   = 0;
              Context->Repeat = TRUE;
            }
          }

          Context->Longpress = TRUE;
        }
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

STATIC KEY_CONTEXT_PRIVATE *KeyList[] = { &KeyContextVolumeUp, &KeyContextVolumeDown, &KeyContextPower };

STATIC
VOID
KeypadInitializeKeyContextPrivate (KEY_CONTEXT_PRIVATE *Context)
{
  Context->PinctrlBase = 0;
  Context->BankOffset  = 0;
  Context->Pin         = 0;
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

  // Reset all keys
  for (Index = 0; Index < (sizeof(KeyList) / sizeof(KeyList[0])); Index++) {
    KeypadInitializeKeyContextPrivate(KeyList[Index]);
  }

  // Locate Gpio Protocol
  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID *)&mGpioProtocol);
  if (!EFI_ERROR (Status)) {
    // Configure keys
    /// Volume Up Button
    StaticContext              = KeypadKeyCodeToKeyContext (115);
    StaticContext->PinctrlBase = 0x11CB0000;
    StaticContext->BankOffset  = 0x60;
    StaticContext->Pin         = 0x5;

    /// Volume Down Button
    StaticContext              = KeypadKeyCodeToKeyContext (116);
    StaticContext->PinctrlBase = 0x11CB0000;
    StaticContext->BankOffset  = 0x60;
    StaticContext->Pin         = 0x6;

    /// Power Button
    StaticContext              = KeypadKeyCodeToKeyContext (117);
    StaticContext->PinctrlBase = 0x11CB0000;
    StaticContext->BankOffset  = 0x60;
    StaticContext->Pin         = 0x7;
  } else {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Exynos GPIO Protocol! Status = %r\n", __FUNCTION__, Status));
  }

  return RETURN_SUCCESS;
}

EFI_STATUS
EFIAPI
KeypadDeviceReset (KEYPAD_DEVICE_PROTOCOL *This)
{
  LibKeyInitializeKeyContext(&KeyContextVolumeUp.EfiKeyContext);
  KeyContextVolumeUp.EfiKeyContext.KeyData.Key.ScanCode   = SCAN_UP;

  LibKeyInitializeKeyContext(&KeyContextVolumeDown.EfiKeyContext);
  KeyContextVolumeDown.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  LibKeyInitializeKeyContext(&KeyContextPower.EfiKeyContext);
  KeyContextPower.EfiKeyContext.KeyData.Key.UnicodeChar   = CHAR_CARRIAGE_RETURN;

  return EFI_SUCCESS;
}

EFI_STATUS
KeypadDeviceGetKeys (
  KEYPAD_DEVICE_PROTOCOL *This,
  KEYPAD_RETURN_API      *KeypadReturnApi,
  UINT64                  Delta)
{
  EFI_STATUS Status    = EFI_SUCCESS;
  UINT32     IsPressed = FALSE;

  if (mGpioProtocol == NULL) {
    return EFI_SUCCESS;
  }

  for (UINTN Index = 0; Index < (sizeof(KeyList) / sizeof(KeyList[0])); Index++) {
    KEY_CONTEXT_PRIVATE *Context = KeyList[Index];

    Status = mGpioProtocol->GetPin ((GpioBank *)Context->PinctrlBase, Context->BankOffset, Context->Pin, &IsPressed);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    LibKeyUpdateKeyStatus (&Context->EfiKeyContext, KeypadReturnApi, !IsPressed, Delta);
  }

  return EFI_SUCCESS;
}