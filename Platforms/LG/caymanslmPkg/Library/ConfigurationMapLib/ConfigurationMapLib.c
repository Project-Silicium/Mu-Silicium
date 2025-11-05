#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR_EX
gConfigurationEntryDescriptorEx[] = {
  // Configuration Map
  {"AbnormalResetOccurredOffset", 0x24},
  {"DetectRetailUserAttentionHotkey", 0x00},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"DloadCookieAddr", 0x01FD3000},
  {"DloadCookieValue", 0x10},
  {"EarlyInitCoreCnt", 1},
  {"EnableACPIFallback", 0x0},
  {"EnableOEMSetupAppInRetail", 0x0},
  {"EnablePXE", 0x0},
  {"EnableSDHCSwitch", 0x1},
  {"EnableShell", 0x1},
  {"EnableUfsIOC", 1},
  {"EnableVariablePolicyEngine", 7},
  {"FastbootUsbInstance", 1},
  {"GccCe1ClkCntlReg", 0x00152004},
  {"GccCe1ClkCntlVal", 0x00000038},
  {"MaxCoreCnt", 8},
  {"MaxLogFileSize", 0x800000},
  {"MemoryCaptureModeOffset", 0x1C},
  {"NumActiveCores", 8},
  {"NumCpus", 8},
  {"NumCpusFuseAddr", 0x5C04C},
  {"PSHoldOffset", 0xC000},
  {"PSHoldSHFT", 0x0},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"SecurityFlag", 0x1C77},
  {"SerialPortBufferSize", 0x8000},
  {"SharedIMEMBaseAddr", 0x146BF000},
  {"ShmBridgememSize", 0xA00000},
  {"TzAppsRegnAddr", 0x86D00000},
  {"TzAppsRegnSize", 0x02200000},
  {"TzDiagOffset", 0x720},
  {"TzDiagSize", 0x2000},
  {"UfsSmmuConfigForOtherBootDev", 1},
  {"UsbFnIoRevNum", 0x00010001},
  {"USBHS1_Config", 0x0},
  {"UefiMemUseThreshold", 0x94},
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
