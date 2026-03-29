#ifndef _CONFIGURATION_MAP_HELPER_LIB_H_
#define _CONFIGURATION_MAP_HELPER_LIB_H_

#include <Library/ConfigurationMapLib.h>

EFI_STATUS
EFIAPI
LocateConfigurationEntry32 (
  IN  CHAR8  *EntryName,
  OUT UINT32 *EntryValue
  );

EFI_STATUS
EFIAPI
LocateConfigurationEntry64 (
  IN  CHAR8  *EntryName,
  OUT UINT64 *EntryValue
  );

#endif /* _CONFIGURATION_MAP_HELPER_LIB_H_ */
