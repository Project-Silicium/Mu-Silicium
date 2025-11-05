#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX
gConfigurationEntryDescriptorEx[] = {
  // Configuration Map
  {"EnableShell", 0x1},
  {"SecPagePoolCount", 0x800},
  {"SharedIMEMBaseAddr", 0x146aa000},
  {"DloadCookieAddr", 0x1fd3000},
  {"DloadCookieValue", 0x10},
  {"PilSubsysDbgCookieAddr", 0x146aa6dc},
  {"PilSubsysDbgCookieVal", 0x53444247},
  {"NumCpus", 0x8},
  {"NumActiveCores", 0x8},
  {"MaxLogFileSize", 0x400000},
  {"UefiMemUseThreshold", 0x1900},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x10001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1e92},
  {"Sdc2GpioConfigOn", 0x1e92},
  {"Sdc1GpioConfigOff", 0xa00},
  {"Sdc2GpioConfigOff", 0xa00},
  {"EnableSDHCSwitch", 0x1},
  {"EnableUfsIOC", 0x1},
  {"UfsSmmuConfigForOtherBootDev", 0x1},
  {"SecurityFlag", 0xc4},
  {"DetectRetailUserAttentionHotkey", 0x0},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"EnableOEMSetupAppInRetail", 0x0},
  {"EnableLogFsSyncInRetail", 0x0},
  {"ShmBridgememSize", 0xa00000},
  {"EnableMultiThreading", 0x1},
  {"MaxCoreCnt", 0x8},
  {"EarlyInitCoreCnt", 0x2},
  {"EnableUefiSecAppDebugLogDump", 0x0},
  {"AllowNonPersistentVarsInRetail", 0x1},
  {"EnableDisplayThread", 0x1},
  {"EnableDisplayImageFv", 0x0},
  {"PrimaryLogoIndex", 0x1},
  {"SecondaryLogoIndex", 0x2},
  {"UARTLogBufferSize", 0x80000},
  {"DDRInfoNotifyFlag", 0x0},
  {"EnableMultiCoreFvDecompression", 0x1},
  {"EnableVariablePolicyEngine", 0x0},
  {"EnableACPIFallback", 0x0},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX*
GetConfigurationMap ()
{
  return gConfigurationEntryDescriptorEx;
}
