#ifndef _DEVICE_CONFIGURATION_MAP_H_
#define _DEVICE_CONFIGURATION_MAP_H_

#define CONFIGURATION_NAME_MAX_LENGTH 64

typedef struct {
  CHAR8                        Name[CONFIGURATION_NAME_MAX_LENGTH];
  UINT64                       Value;
} CONFIGURATION_DESCRIPTOR_EX, *PCONFIGURATION_DESCRIPTOR_EX;

static CONFIGURATION_DESCRIPTOR_EX gDeviceConfigurationDescriptorEx[] = {
    /* Terminator */
    {"Terminator", 0xFFFFFFFF}};

#endif
