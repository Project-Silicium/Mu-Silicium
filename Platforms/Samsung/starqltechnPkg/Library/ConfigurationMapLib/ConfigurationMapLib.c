#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX
gConfigurationEntryDescriptorEx[] = {
  // Configuration Map
  {"NumCpusFuseAddr", 0x5C04C},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x146BF000},
  {"DloadCookieAddr", 0x1FD3000},
  {"DloadCookieValue", 0x10},
  {"NumCpus", 0x8},
  {"NumActiveCores", 0x8},
  {"MaxLogFileSize", 0x400000},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x10001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOff", 0xA00},
  {"EnableSDHCSwitch", 0x1},
  {"SecurityFlag", 0xc4},
  {"TzAppsRegnAddr", 0x86D00000},
  {"TzAppsRegnSize", 0x2200000},
  {"ImageFVFlashed", 0x0},
  {"EnableLogFsSyncInRetail", 0x0},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX*
GetConfigurationMap ()
{
  return gConfigurationEntryDescriptorEx;
}
