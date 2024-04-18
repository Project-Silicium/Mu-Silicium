/**
  Header file for BDS Platform specific code

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _BDS_PLATFORM_H
#define _BDS_PLATFORM_H

#define CLASS_HID          3
#define SUBCLASS_BOOT      1
#define PROTOCOL_KEYBOARD  1

STATIC EFI_BOOT_MODE              mBootMode;
STATIC EFI_DEVICE_PATH_PROTOCOL **mPlatformConnectSequence;

typedef struct {
  USB_CLASS_DEVICE_PATH       UsbClass;
  EFI_DEVICE_PATH_PROTOCOL    End;
} USB_CLASS_FORMAT_DEVICE_PATH;

STATIC USB_CLASS_FORMAT_DEVICE_PATH mUsbClassKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8)(sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8)((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xFFFF,           // VendorId
    0xFFFF,           // ProductId
    CLASS_HID,        // DeviceClass
    SUBCLASS_BOOT,    // DeviceSubClass
    PROTOCOL_KEYBOARD // DeviceProtocol
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

#endif
