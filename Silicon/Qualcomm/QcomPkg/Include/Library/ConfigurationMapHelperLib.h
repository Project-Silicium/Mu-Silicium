#ifndef _CONFIGURATION_MAP_HELPER_LIB_H_
#define _CONFIGURATION_MAP_HELPER_LIB_H_

#include <Library/DeviceConfigurationMapLib.h>

EFI_STATUS
EFIAPI
LocateConfigurationMapUINT32ByName (
  CHAR8  *ConfigurationMapUINT32Name,
  UINT32 *ConfigurationDescriptor
  );

EFI_STATUS
EFIAPI
LocateConfigurationMapUINT64ByName (
  CHAR8  *ConfigurationMapUINT64Name,
  UINT64 *ConfigurationDescriptor
  );

#endif /* _CONFIGURATION_MAP_HELPER_LIB_H_ */
