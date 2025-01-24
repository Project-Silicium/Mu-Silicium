#ifndef _BOOT_DEVICES_H_
#define _BOOT_DEVICES_H_

#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>

#include <Protocol/DevicePath.h>

typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_KEYPAD_DEVICE_PATH;

typedef struct {
  VENDOR_DEVICE_PATH       DisplayDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_DISPLAY_DEVICE_PATH;

typedef struct {
   VENDOR_DEVICE_PATH SdCardDevicePath;
   EFI_DEVICE_PATH    EndDevicePath;
} EFI_SDCARD_DEVICE_PATH;

#define EFI_KEYPAD_DEVICE_GUID                                                 \
  {                                                                            \
    0xD7F58A0E, 0xBED2, 0x4B5A,                                                \
    {                                                                          \
      0xBB, 0x43, 0x8A, 0xB2, 0x3D, 0xD0, 0xE2, 0xB0                           \
    }                                                                          \
  }

#define EFI_SDCARD_DEVICE_PATH_GUID                                            \
  {                                                                            \
    0xD1531D41, 0x3F80, 0x4091,                                                \
    {                                                                          \
      0x8D, 0x0A, 0x54, 0x1F, 0x59, 0x23, 0x6D, 0x66                           \
    }                                                                          \
  }

EFI_KEYPAD_DEVICE_PATH KeypadDevicePath = {
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

EFI_DISPLAY_DEVICE_PATH DisplayDevicePath = {
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

EFI_SDCARD_DEVICE_PATH SdcardDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_SDCARD_DEVICE_PATH_GUID
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
