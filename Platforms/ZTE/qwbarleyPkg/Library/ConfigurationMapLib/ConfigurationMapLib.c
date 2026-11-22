#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR
gConfigurationDescriptor[] = {
  // Generated from ZTE xbl_config.img qcom,uefi int_param
  {"EnableShell", 0x01},
  {"SecPagePoolCount", 0xa00},
  {"SharedIMEMBaseAddr", 0x14680000},
  {"DloadCookieAddr", 0x1fd9000},
  {"DloadCookieValue", 0x10},
  {"SkipDBISetup", 0x00},
  {"PilSubsysDbgCookieAddr", 0x146806dc},
  {"PilSubsysDbgCookieVal", 0x53444247},
  {"NumCpus", 0x08},
  {"NumActiveCores", 0x08},
  {"MaxLogFileSize", 0x400000},
  {"UefiMemUseThreshold", 0xe1},
  {"USBHS1_Config", 0x00},
  {"UsbFnIoRevNum", 0x10001},
  {"PwrBtnShutdownFlag", 0x00},
  {"Sdc1GpioConfigOn", 0x1e92},
  {"Sdc2GpioConfigOn", 0x1e92},
  {"Sdc1GpioConfigOff", 0xa00},
  {"Sdc2GpioConfigOff", 0xa00},
  {"EnableSDHCSwitch", 0x01},
  {"EnableUfsIOC", 0x01},
  {"UfsSmmuConfigForOtherBootDev", 0x01},
  {"SecurityFlag", 0xc4},
  {"EnableLogFsSyncInRetail", 0x00},
  {"DetectRetailUserAttentionHotkey", 0x00},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"ShmBridgememSize", 0xa00000},
  {"MaxCoreCount", 0x08},
  {"EnableMultiThreading", 0x01},
  {"EarlyInitCoreCnt", 0x02},
  {"IsFunctionalCoreCountToOverrideFlag", 0x00},
  {"EnableUefiSecAppDebugLogDump", 0x00},
  {"AllowNonPersistentVarsInRetail", 0x01},
  {"EnableDisplayThread", 0x01},
  {"EnableDisplayImageFv", 0x00},
  {"DDRInfoNotifyFlag", 0x00},
  {"EnableMultiCoreFvDecompression", 0x01},
  {"EnableVariablePolicyEngine", 0x00},
  {"EnableACPIFallback", 0x00},
  {"DRAM_CLK_PERIOD_ADDR", 0x240ba050},
};

VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount)
{
  // Pass Data
  *ConfigurationDescriptor      = gConfigurationDescriptor;
  *ConfigurationDescriptorCount = ARRAY_SIZE (gConfigurationDescriptor);
}
