#ifndef _KEYPAD_DEVICE_H_
#define _KEYPAD_DEVICE_H_

#include <Protocol/SimpleTextInEx.h>

#define KEYPAD_DEVICE_PROTOCOL_GUID                                            \
  {                                                                            \
    0xa412c1ac, 0x0295, 0x4ae3,                                                \
    {                                                                          \
      0xb5, 0x9f, 0x52, 0x23, 0x1f, 0x26, 0x55, 0x4d                           \
    }                                                                          \
  }

typedef struct _KEYPAD_DEVICE_PROTOCOL KEYPAD_DEVICE_PROTOCOL;
typedef struct _KEYPAD_RETURN_API      KEYPAD_RETURN_API;

typedef
VOID
(EFIAPI *PUSH_EFI_KEY_BUF_TAIL)(
  KEYPAD_RETURN_API *This,
  EFI_KEY_DATA      *KeyData
  );

struct _KEYPAD_RETURN_API {
  PUSH_EFI_KEY_BUF_TAIL PushEfikeyBufTail;
};

typedef
EFI_STATUS
(EFIAPI *KEYPAD_RESET)(
  KEYPAD_DEVICE_PROTOCOL *This
  );

typedef
EFI_STATUS
(EFIAPI *KEYPAD_GET_KEYS)(
  KEYPAD_DEVICE_PROTOCOL *This,
  KEYPAD_RETURN_API      *KeypadReturnApi,
  UINT64                  Delta
  );

struct _KEYPAD_DEVICE_PROTOCOL {
  KEYPAD_RESET    Reset;
  KEYPAD_GET_KEYS GetKeys;
};

extern EFI_GUID gKeypadDeviceProtocolGuid;

#endif /* _KEYPAD_DEVICE_H_ */
