#ifndef _PLATFORM_BOOT_MANAGER_H_
#define _PLATFORM_BOOT_MANAGER_H_

EFI_STATUS
RegisterKeyCallback (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath);

VOID
UnregisterKeyCallback ();

VOID
GetKeyStates (
  OUT BOOLEAN *FfuMode,
  OUT BOOLEAN *AlternativeApp
  );

#endif /* _PLATFORM_BOOT_MANAGER_H_ */
