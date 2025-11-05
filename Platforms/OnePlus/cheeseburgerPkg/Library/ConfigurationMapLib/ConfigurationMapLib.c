#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX
gConfigurationEntryDescriptorEx[] = {
  // Configuration Map
  {"NumCpusFuseAddr", 0x5c04c},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x146bf000},
  {"DloadCookieAddr", 0x1fd3000},
  {"DloadCookieValue", 0x10},
  {"MemoryCaptureModeOffset", 0x1c},
  {"AbnormalResetOccurredOffset", 0x24},
  {"DBIDumpDDRBase", 0x9ffd0000},
  {"NumCpus", 0x4},
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
  {"PSHoldOffset", 0xc000},
  {"PSHoldSHFT", 0x0},
  {"GCCResetValueAddress", 0x146bf028},
  {"SecurityFlag", 0xc4},
  {"TzAppsRegnAddr", 0x86d00000},
  {"TzAppsRegnSize", 0x2200000},
  {"EnableLogFsSyncInRetail", 0x0},
  {"EnableSecurityHoleForSplashPartition", 0x0},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX*
GetConfigurationMap ()
{
  return gConfigurationEntryDescriptorEx;
}
