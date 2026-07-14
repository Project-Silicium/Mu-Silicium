#ifndef _PLATFORM_BOOT_OPTIONS_LIB_H_
#define _PLATFORM_BOOT_OPTIONS_LIB_H_

//
// Platform Boot Option
//
typedef struct {
  CHAR16   *AppName;
  EFI_GUID *AppGuid;
} EFI_PLATFORM_BOOT_OPTION;

/**
  This Function Returns the Boot Options Specified by the Platform.

  @param[out] BootOption                   - The Platform Boot Options.
  @param[out] BootOptionCount              - The Number of Platform Boot Options.
**/
VOID
GetPlatformBootOptions (
  OUT EFI_PLATFORM_BOOT_OPTION **BootOption,
  OUT UINT8                     *BootOptionCount
  );

#endif /* _PLATFORM_BOOT_OPTIONS_LIB_H_ */
