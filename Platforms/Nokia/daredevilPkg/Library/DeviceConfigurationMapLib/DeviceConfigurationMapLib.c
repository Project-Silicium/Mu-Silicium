#include <Library/DeviceConfigurationMapLib.h>

STATIC
CONFIGURATION_DESCRIPTOR_EX
gDeviceConfigurationDescriptorEx[] = {
  // configurations
{"NumCpusFuseAddr", 0x5C04C},
// Force booting to shell whilst in pre-silicon phase
{"EnableShell", 0x1},

// Shared IMEM (Cookies, Offsets)
{"SharedIMEMBaseAddr", 0x146BF000},
{"DloadCookieAddr", 0x01FD3000},
{"DloadCookieValue", 0x10},
{"MemoryCaptureModeOffset", 0x1C},
{"AbnormalResetOccurredOffset", 0x24},
{"DBIDumpDDRBase", 0x9FFD0000},

{"NumCpus", 4},
{"NumActiveCores", 8},

{"MaxLogFileSize", 0x400000},

// USB 
{"USBHS1_Config", 0x0},
{"UsbFnIoRevNum", 0x00010001},

// Buttons / KeyPad
{"PwrBtnShutdownFlag", 0x0},

// SDCC
{"Sdc1GpioConfigOn", 0x1E92},
{"Sdc2GpioConfigOn", 0x1E92},
{"Sdc1GpioConfigOff", 0xA00},
{"Sdc2GpioConfigOff", 0xA00},

// SDHC Mode 0:Legacy Mode, Non-zero: SDHC Mode
{"EnableSDHCSwitch", 0x1},

// Reset
{"PSHoldOffset", 0xC000},
{"PSHoldSHFT", 0x0},

// Reset Reason -  SharedIMEMBaseAddr +  0x28
{"GCCResetValueAddress", 0x146BF028},

// Security flag
{"SecurityFlag", 0xC4},

// TZ ApPs Notification parameters
{"TzAppsRegnAddr", 0x86D00000},
{"TzAppsRegnSize", 0x02200000},
  
// LogFs partition 
{"EnableLogFsSyncInRetail", 0x0},
  // Terminator
  {"Terminator", 0xFFFFFFFF}
};

CONFIGURATION_DESCRIPTOR_EX*
GetDeviceConfigurationMap ()
{
  return gDeviceConfigurationDescriptorEx;
}
