#ifndef _MASS_STORAGE_H_
#define _MASS_STORAGE_H_

//
// Menu Action
//
typedef
VOID
(*EFI_MENU_ACTION) ();

//
// Menu Details
//
typedef struct {
  CHAR16          *Label;
  EFI_MENU_ACTION  Action;
} EFI_UI_MENU_DETAILS;

//
// Storage Device Path
//
typedef struct {
  VENDOR_DEVICE_PATH       VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL EndDevicePath;
} EFI_STORAGE_DEVICE_PATH;

//
// Storage Device Path
//
EFI_STORAGE_DEVICE_PATH StorageVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    {0}
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
