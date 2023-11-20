#ifndef __PROTOCOL_KEYPAD_DEVICE_H__
#define __PROTOCOL_KEYPAD_DEVICE_H__

#include <Protocol/SimpleTextInEx.h>

#define KEYPAD_DEVICE_PROTOCOL_GUID                                            \
  {                                                                            \
    0xD7F58A0E, 0xBED2, 0x4B5A,                                                \
    {                                                                          \
      0xBB, 0x43, 0x8A, 0xB2, 0x3D, 0xD0, 0xE2, 0xB0                           \
    }                                                                          \
  }

typedef struct _KEYPAD_DEVICE_PROTOCOL KEYPAD_DEVICE_PROTOCOL;
typedef struct _KEYPAD_RETURN_API      KEYPAD_RETURN_API;

typedef
VOID(EFIAPI *PUSH_EFI_KEY_BUF_TAIL)(
  KEYPAD_RETURN_API *This,
  EFI_KEY_DATA      *KeyData
  );

struct _KEYPAD_RETURN_API {
  PUSH_EFI_KEY_BUF_TAIL PushEfikeyBufTail;
};

typedef
EFI_STATUS(EFIAPI *KEYPAD_RESET)(KEYPAD_DEVICE_PROTOCOL *This);

typedef
EFI_STATUS(EFIAPI *KEYPAD_GET_KEYS)(
  KEYPAD_DEVICE_PROTOCOL *This,
  KEYPAD_RETURN_API      *KeypadReturnApi,
  UINT64                  Delta);

struct _KEYPAD_DEVICE_PROTOCOL {
  KEYPAD_RESET    Reset;
  KEYPAD_GET_KEYS GetKeys;
};

extern EFI_GUID gQcomKeypadDeviceProtocolGuid;

#endif