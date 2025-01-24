#ifndef _MASS_STORAGE_H_
#define _MASS_STORAGE_H_

#include <Library/DevicePathLib.h>

// UFS Lun 0 Device Path
typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_UFS_LUN_0_DEVICE_PATH;

// eMMC User Partition Device Path
typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_EMMC_USER_PARTITION_DEVICE_PATH;

#define EFI_UFS_LUN_0_DEVICE_GUID                                              \
  {                                                                            \
    0x860845C1, 0xBE09, 0x4355,                                                \
    {                                                                          \
      0x8B, 0xC1, 0x30, 0xD6, 0x4F, 0xF8, 0xE6, 0x3A                           \
    }                                                                          \
  }

#define EFI_EMMC_USER_PARTITION_DEVICE_GUID                                    \
  {                                                                            \
    0xB615F1F5, 0x5088, 0x43CD,                                                \
    {                                                                          \
      0x80, 0x9C, 0xA1, 0x6E, 0x52, 0x48, 0x7D, 0x00                           \
    }                                                                          \
  }

EFI_UFS_LUN_0_DEVICE_PATH UFSLun0DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_0_DEVICE_GUID
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

EFI_EMMC_USER_PARTITION_DEVICE_PATH eMMCUserPartitionDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_EMMC_USER_PARTITION_DEVICE_GUID
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

#endif /* _MASS_STORAGE_H_ */
