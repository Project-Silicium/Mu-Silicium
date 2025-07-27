#include <Library/DeviceConfigurationMapLib.h>

STATIC
CONFIGURATION_DESCRIPTOR_EX
gDeviceConfigurationDescriptorEx[] = {
  // Configuration Map - Adapted for SM7475 (Poco F5)
  {"EnableShell", 0x1},
  {"SecPagePoolCount", 0x800},

  // Memory addresses
  {"SharedIMEMBaseAddr", 0x146AA000},
  {"DloadCookieAddr", 0x146AA01C},
  {"DloadCookieValue", 0x10},
  {"PilSubsysDbgCookieAddr", 0x146AA6DC},
  {"PilSubsysDbgCookieVal", 0x53444247},

  // CPU Configuration
  {"NumCpus", 8},
  {"NumActiveCores", 8},
  {"UefiMemUseThreshold", 0x1900},

  // USB Configuration
  {"USBHS1_Config", 0x0},
  {"UfnIoRevNum", 0x00010001},
  {"PwrBtnShutdownFlag", 0x0},

  // SD Card Configuration
  {"Sdc1GpioConfigOn", 0x0},
  {"Sdc2GpioConfigOn", 0x0},
  {"Sdc1GpioConfigOff", 0x0},
  {"Sdc2GpioConfigOff", 0x0},
  {"EnableSDHCSwitch", 0x0},

  // UFS Storage Configuration
  {"EnableUfsIOC", 1},                 // UFS I/O coherency
  {"UfsSmmuConfigForOtherBootDev", 1}, // SMMU config for UFS
  // UFS addresses from DTS:
  // ufs_mem: 0x1d84000-0x1d86fff
  // ufs_ice: 0x1d88000-0x1d8ffff
  // ufs_ice_hwkm: 0x1d90000-0x1d98fff

  // Security and Boot Configuration
  {"SecurityFlag", 0xC4},
  {"DetectRetailUserAttentionHotkey", 0x00},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"EnableOEMSetupAppInRetail", 0x0},
  {"EnableLogFsSyncInRetail", 0x1},

  // Memory and Threading Configuration
  {"ShmBridgememSize", 0xA00000},      // Shared memory bridge size
  {"EnableMultiThreading", 1},
  {"MaxCoreCnt", 8},                   // Maximum core count
  {"EarlyInitCoreCnt", 2},             // Cores to initialize early

  // Debug and Logging
  {"EnableUefiSecAppDebugLogDump", 0x0},
  {"AllowNonPersistentVarsInRetail", 0x1},

  // Display Configuration
  {"EnableDisplayThread", 0x1},
  {"EnableDisplayImageFv", 0x1},
  {"PrimaryLogoIndex", 1},
  {"SecondaryLogoIndex", 2},

  // Additional Configuration
  {"DDRInfoNotifyFlag", 0x0},
  {"EnableMultiCoreFvDecompression", 1},
  {"EnableVariablePolicyEngine", 0},
  {"EnableACPIFallback", 0x0},

  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

CONFIGURATION_DESCRIPTOR_EX*
GetDeviceConfigurationMap ()
{
  return gDeviceConfigurationDescriptorEx;
}
