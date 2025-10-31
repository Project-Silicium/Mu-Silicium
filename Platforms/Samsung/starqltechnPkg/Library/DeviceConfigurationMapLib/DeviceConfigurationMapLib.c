#include <Library/DeviceConfigurationMapLib.h>

STATIC
CONFIGURATION_DESCRIPTOR_EX
gDeviceConfigurationDescriptorEx[] = {
  {"NumCpusFuseAddr", 0x5c04c},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x146bf000},
  {"DloadCookieAddr", 0x1fd3000},
  {"DloadCookieValue", 0x10},
  {"NumCpus", 0x8},
  {"NumActiveCores", 0x8},
  {"MaxLogFileSize", 0x400000},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x10001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1e92},
  {"Sdc2GpioConfigOn", 0x1e92},
  {"Sdc1GpioConfigOff", 0xa00},
  {"Sdc2GpioConfigOff", 0xa00},
  {"EnableSDHCSwitch", 0x1},
  {"SecurityFlag", 0xc4},
  {"TzAppsRegnAddr", 0x86d00000},
  {"TzAppsRegnSize", 0x2200000},
  {"ImageFVFlashed", 0x0},
  {"EnableLogFsSyncInRetail", 0x0},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

CONFIGURATION_DESCRIPTOR_EX*
GetDeviceConfigurationMap ()
{
  return gDeviceConfigurationDescriptorEx;
}
