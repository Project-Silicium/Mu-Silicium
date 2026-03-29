#ifndef _CONFIGURATION_MAP_LIB_H_
#define _CONFIGURATION_MAP_LIB_H_

#include <PiPei.h>

//
// Limits
//
#define CONFIGURATION_NAME_MAX_LENGTH 64

//
// Configuration Entry
//
typedef struct {
  CHAR8  Name[CONFIGURATION_NAME_MAX_LENGTH];
  UINT64 Value;
} EFI_CONFIGURATION_ENTRY_DESCRIPTOR;

//
// Functions
//
VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount
  );

#endif /* _CONFIGURATION_MAP_LIB_H_ */
