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
  EFI_KEY_DATA KeyData;
  UINT64       Time;
  KEY_STATE    State;
  BOOLEAN      Repeat;
  BOOLEAN      Longpress;
} KEY_CONTEXT;

#endif /* _KEYPAD_DEVICE_LIB_H_ */
