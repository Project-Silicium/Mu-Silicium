#ifndef _EMMC_H_
#define _EMMC_H_

//
// eMMC User GUID
//
#define EFI_EMMC_USER_GUID                             \
  {                                                    \
    0xB615F1F5, 0x5088, 0x43CD,                        \
    {                                                  \
      0x80, 0x9C, 0xA1, 0x6E, 0x52, 0x48, 0x7D, 0x00   \
    }                                                  \
  }

//
// eMMC Device Path
//
typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_EMMC_USER_PATH;

//
// eMMC User Device Path
//
EFI_EMMC_USER_PATH EmmcUserDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_EMMC_USER_GUID
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

#endif /* _EMMC_H_ */
