#ifndef _UFS_H_
#define _UFS_H_

//
// UFS LUN 0 GUID
//
#define EFI_UFS_LUN_0_GUID                             \
  {                                                    \
    0x860845C1, 0xBE09, 0x4355,                        \
    {                                                  \
      0x8B, 0xC1, 0x30, 0xD6, 0x4F, 0xF8, 0xE6, 0x3A   \
    }                                                  \
  }

//
// UFS LUN 1 GUID
//
#define EFI_UFS_LUN_1_GUID                             \
  {                                                    \
    0x8D90D477, 0x39A3, 0x4A38,                        \
    {                                                  \
      0xAB, 0x9E, 0x58, 0x6F, 0xF6, 0x9E, 0xD0, 0x51   \
    }                                                  \
  }

//
// UFS LUN 2 GUID
//
#define EFI_UFS_LUN_2_GUID                             \
  {                                                    \
    0xEDF85868, 0x87EC, 0x4F77,                        \
    {                                                  \
      0x9C, 0xDA, 0x5F, 0x10, 0xDF, 0x2F, 0xE6, 0x01   \
    }                                                  \
  }

//
// UFS LUN 3 GUID
//
#define EFI_UFS_LUN_3_GUID                             \
  {                                                    \
    0x1AE69024, 0x8AEB, 0x4DF8,                        \
    {                                                  \
      0xBC, 0x98, 0x00, 0x32, 0xDB, 0xDF, 0x50, 0x24   \
    }                                                  \
  }

//
// UFS LUN 4 GUID
//
#define EFI_UFS_LUN_4_GUID                             \
  {                                                    \
    0xD33F1985, 0xF107, 0x4A85,                        \
    {                                                  \
      0xBE, 0x38, 0x68, 0xDC, 0x7A, 0xD3, 0x2C, 0xEA   \
    }                                                  \
  }

//
// UFS LUN 5 GUID
//
#define EFI_UFS_LUN_5_GUID                             \
  {                                                    \
    0x4BA1D05F, 0x088E, 0x483F,                        \
    {                                                  \
      0xA9, 0x7E, 0xB1, 0x9B, 0x9C, 0xCF, 0x59, 0xB0   \
    }                                                  \
  }

//
// UFS LUN 6 GUID
//
#define EFI_UFS_LUN_6_GUID                             \
  {                                                    \
    0x4ACF98F6, 0x26FA, 0x44D2,                        \
    {                                                  \
      0x81, 0x32, 0x28, 0x2F, 0x2D, 0x19, 0xA4, 0xC5   \
    }                                                  \
  }

//
// UFS LUN 7 GUID
//
#define EFI_UFS_LUN_7_GUID                             \
  {                                                    \
    0x8598155F, 0x34DE, 0x415C,                        \
    {                                                  \
      0x8B, 0x55, 0x84, 0x3E, 0x33, 0x22, 0xD3, 0x6F   \
    }                                                  \
  }

//
// UFS LUN Device Path
//
typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_UFS_LUN_DEVICE_PATH;

//
// UFS LUN 0 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun0DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_0_GUID
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
// UFS LUN 1 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun1DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_1_GUID
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
// UFS LUN 2 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun2DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_2_GUID
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
// UFS LUN 3 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun3DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_3_GUID
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
// UFS LUN 4 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun4DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_4_GUID
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
// UFS LUN 5 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun5DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_5_GUID
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
// UFS LUN 6 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun6DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_6_GUID
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
// UFS LUN 7 Device Path
//
EFI_UFS_LUN_DEVICE_PATH UfsLun7DevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_UFS_LUN_7_GUID
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

#endif /* _UFS_H_ */
