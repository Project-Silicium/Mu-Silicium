#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX
gConfigurationEntryDescriptorEx[] = {
  // Configuration Map
  {"NumCpusFuseAddr", 0x5c04c},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0xc125000},
  {"DloadCookieAddr", 0x3d3000},
  {"DloadCookieValue", 0x10},
  {"NumCpus", 0x8},
  {"NumActiveCores", 0x8},
  {"MaxLogFileSize", 0x400000},
  {"UefiMemUseThreshold", 0x7a},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x10001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1e92},
  {"Sdc2GpioConfigOn", 0x1e92},
  {"Sdc1GpioConfigOff", 0xa00},
  {"Sdc2GpioConfigOff", 0xa00},
  {"EnableSDHCSwitch", 0x1},
  {"EnableUfsIOC", 0x0},
  {"UfsSmmuConfigForOtherBootDev", 0x1},
  {"SecurityFlag", 0xc4},
  {"TzAppsRegnAddr", 0xc1800000},
  {"TzAppsRegnSize", 0x8400000},
  {"EnableLogFsSyncInRetail", 0x0},
  {"ShmBridgememSize", 0xa00000},
  {"EnableMultiThreading", 0x1},
  {"MaxCoreCnt", 0x8},
  {"EarlyInitCoreCnt", 0x1},
  {"EnableDisplayThread", 0x1},
  {"EnableUefiSecAppDebugLogDump", 0x1},
  {"AllowNonPersistentVarsInRetail", 0x1},
  {"MinidumpTALoadingCfg", 0x0},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX*
GetConfigurationMap ()
{
  return gConfigurationEntryDescriptorEx;
}
