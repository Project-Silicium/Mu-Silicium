#ifndef _KEYPAD_DEVICE_LIB_H_
#define _KEYPAD_DEVICE_LIB_H_

#include <Protocol/EfiKeypadDevice.h>

EFI_STATUS
EFIAPI
KeypadDeviceReset (KEYPAD_DEVICE_PROTOCOL *This);

EFI_STATUS
KeypadDeviceGetKeys (
  KEYPAD_DEVICE_PROTOCOL *This,
  KEYPAD_RETURN_API      *KeypadReturnApi,
  UINT64                  Delta
  );

typedef enum {
  KEYSTATE_RELEASED,
  KEYSTATE_PRESSED,
  KEYSTATE_LONGPRESS_RELEASE,
} KEY_STATE;

typedef struct {
  EFI_KEY_DATA KeyData;     // Keydata that gets Send to the Driver
  UINT64       Time;        // The Time the Current Action has been Running
  KEY_STATE    State;       // The Current State of the Key
  BOOLEAN      Repeat;      // The Current Key has to Repeat Sending Events
  BOOLEAN      Longpress;   // The User did a Longpress
} KEY_CONTEXT;

#endif /* _KEYPAD_DEVICE_LIB_H_ */
