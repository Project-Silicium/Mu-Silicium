#ifndef _CONFIGURATION_MAP_LIB_H_
#define _CONFIGURATION_MAP_LIB_H_

//
// Name Length Limit
//
#define CONFIGURATION_NAME_MAX_LENGTH 64

//
// Configuration Entry Descriptor
//
typedef struct {
  CHAR8  Name[CONFIGURATION_NAME_MAX_LENGTH];
  UINT64 Value;
} EFI_CONFIGURATION_ENTRY_DESCRIPTOR;

/**
  This Function returns the Configuration Map Data.

  @param[out] ConfigurationDescriptor      - The Configuration Map.
  @param[out] ConfigurationDescriptorCount - The Number of Entries in the Configuration Map.
**/
VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount
  );

#endif /* _CONFIGURATION_MAP_LIB_H_ */
