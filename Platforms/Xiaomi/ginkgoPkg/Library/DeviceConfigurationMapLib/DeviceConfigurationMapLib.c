#include <Library/DeviceConfigurationMapLib.h>

STATIC
CONFIGURATION_DESCRIPTOR_EX
gDeviceConfigurationDescriptorEx[] = {
  // Configuration Map
  {"NumCpusFuseAddr", 0x5C04C},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x0C125000},
  {"DloadCookieAddr", 0x003D3000},
  {"DloadCookieValue", 0x10},
  {"NumCpus", 8},
  {"NumActiveCores", 8},
  {"MaxLogFileSize", 0x400000},
  {"UefiMemUseThreshold", 0x77},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x00010001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOff", 0xA00},
  {"EnableSDHCSwitch", 0x1},
  {"EnableUfsIOC", 0},
  {"UfsSmmuConfigForOtherBootDev", 1},
  {"SecurityFlag", 0xC4},
  {"TzAppsRegnAddr", 0x46D00000},
  {"TzAppsRegnSize", 0x02200000},
  {"EnableLogFsSyncInRetail", 0x0},
  {"ShmBridgememSize", 0xA00000},
  {"EnableMultiThreading", 1},
  {"MaxCoreCnt", 8},
  {"EarlyInitCoreCnt", 1},
  {"EnableDisplayThread", 1},
  {"EnableUefiSecAppDebugLogDump", 0x0},
  {"AllowNonPersistentVarsInRetail", 0x1},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

CONFIGURATION_DESCRIPTOR_EX*
GetDeviceConfigurationMap ()
{
  return gDeviceConfigurationDescriptorEx;
}
