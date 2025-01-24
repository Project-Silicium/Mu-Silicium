#ifndef _BDS_EXTENSION_LIB_H_
#define _BDS_EXTENSION_LIB_H_

#include <Library/UefiBootManagerLib.h>

VOID
BdsExtensionAfterConsole ();

VOID
BdsExtensionPlatformConnectList ();

EFI_STATUS
EFIAPI
BdsExtensionPriorityBoot (EFI_BOOT_MANAGER_LOAD_OPTION *BootOption);

#endif /* _BDS_EXTENSION_LIB_H_ */
