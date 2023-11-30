#include <Library/BaseLib.h>
#include <Library/DeviceConfigurationMapLib.h>

STATIC
CONFIGURATION_DESCRIPTOR_EX
gDeviceConfigurationDescriptorEx[] = {
  /* Terminator */
  {"Terminator", 0xFFFFFFFF}
};

CONFIGURATION_DESCRIPTOR_EX
*GetDeviceConfigurationMap()
{
  return gDeviceConfigurationDescriptorEx;
}