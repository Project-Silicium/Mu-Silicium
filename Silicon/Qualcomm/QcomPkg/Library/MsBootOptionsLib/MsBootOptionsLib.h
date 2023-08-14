#ifndef _MS_BOOT_OPTIONS_LIB_H_
#define _MS_BOOT_OPTIONS_LIB_H_

#define AB_SLOT_SWITCH_NAME       L"Switch Slot"

#define INTERNAL_UEFI_SHELL_NAME  L"Internal UEFI Shell 2.0"

#define MS_SDD_BOOT               L"Internal Storage"
#define MS_SDD_BOOT_PARM          "SDD"
#define MS_USB_BOOT               L"USB Storage"
#define MS_USB_BOOT_PARM          "USB"
#define MS_PXE_BOOT               L"PXE Network"
#define MS_PXE_BOOT_PARM          "PXE"

typedef struct {
  MEDIA_FW_VOL_DEVICE_PATH    FvDevPath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevPath;
} FV_PIWG_DEVICE_PATH;

FV_PIWG_DEVICE_PATH  mFvPIWGDevicePathTemplate = {
  {
    {
      MEDIA_DEVICE_PATH,
      MEDIA_PIWG_FW_VOL_DP,
      {
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH)),
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH) >> 8)
      }
    },
    { 0 }
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

#endif /* _MS_BOOT_OPTIONS_LIB_H_ */
