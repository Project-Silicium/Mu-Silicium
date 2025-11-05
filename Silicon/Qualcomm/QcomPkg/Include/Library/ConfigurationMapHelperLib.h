#ifndef _CONFIGURATION_MAP_HELPER_LIB_H_
#define _CONFIGURATION_MAP_HELPER_LIB_H_

#include <Library/ConfigurationMapLib.h>

EFI_STATUS
EFIAPI
LocateConfigurationMapUINT32ByName (
  IN  CHAR8  *ConfigurationMapUINT32Name,
  OUT UINT32 *ConfigurationDescriptor
  );

EFI_STATUS
EFIAPI
LocateConfigurationMapUINT64ByName (
  IN  CHAR8  *ConfigurationMapUINT64Name,
  OUT UINT64 *ConfigurationDescriptor
  );

#endif /* _CONFIGURATION_MAP_HELPER_LIB_H_ */
