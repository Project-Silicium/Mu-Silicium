#ifndef _BOOT_DEVICES_H_
#define _BOOT_DEVICES_H_

#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>

#include <Protocol/DevicePath.h>

//
// Keypad Controller Protocol GUID
//
#define EFI_KEYPAD_DEVICE_GUID                                                 \
  {                                                                            \
    0xD7F58A0E, 0xBED2, 0x4B5A,                                                \
    {                                                                          \
      0xBB, 0x43, 0x8A, 0xB2, 0x3D, 0xD0, 0xE2, 0xB0                           \
    }                                                                          \
  }

//
// Generic Device Path
//
typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_GENERIC_DEVICE_PATH;

//
// USB Keyboard Device Path
//
typedef struct {
  USB_CLASS_DEVICE_PATH    UsbClassDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_USB_KEYBOARD_DEVICE_PATH;

//
// Keypad Controller Device Path
//
EFI_GENERIC_DEVICE_PATH KeypadDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_KEYPAD_DEVICE_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

//
// Display Device Path
//
EFI_GENERIC_DEVICE_PATH DisplayDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

//
// USB Keyboard Controller Device Path
//
EFI_USB_KEYBOARD_DEVICE_PATH mUsbKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8)(sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8)((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xFFFF,
    0xFFFF,
    0x0003,
    0x0001,
    0x0001
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

#endif /* _BOOT_DEVICES_H_ */
