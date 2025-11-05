#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX
gConfigurationEntryDescriptorEx[] = {
  // Configuration Map
  {"NumCpusFuseAddr", 0x5C04C},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x146BF000},
  {"DloadCookieAddr", 0x01FD3000},
  {"DloadCookieValue", 0x10},
  {"MemoryCaptureModeOffset", 0x1C},
  {"AbnormalResetOccurredOffset", 0x24},
  {"DBIDumpDDRBase", 0x9FFD0000},
  {"NumCpus", 4},
  {"NumActiveCores", 8},
  {"MaxLogFileSize", 0x400000},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x00010001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOff", 0xA00},
  {"EnableSDHCSwitch", 0x1},
  {"PSHoldOffset", 0xC000},
  {"PSHoldSHFT", 0x0},
  {"GCCResetValueAddress", 0x146BF028},
  {"SecurityFlag", 0xC4},
  {"TzAppsRegnAddr", 0x86D00000},
  {"TzAppsRegnSize", 0x02200000},
  {"EnableLogFsSyncInRetail", 0x1},
  {"EnableSecurityHoleForSplashPartition", 0x1},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX*
GetConfigurationMap ()
{
  return gConfigurationEntryDescriptorEx;
}
