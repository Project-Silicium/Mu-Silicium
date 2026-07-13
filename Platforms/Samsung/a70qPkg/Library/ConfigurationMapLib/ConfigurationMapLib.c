#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR
gConfigurationDescriptor[] = {
    // Configuration Map
    // Configuration Map
    {"NumCpusFuseAddr", 0x5C04C},
    {"EnableShell", 0x1},
    {"SharedIMEMBaseAddr", 0x146AA000},
    {"DloadCookieAddr", 0x1FD3000},
    {"DloadCookieValue", 0x10},
    {"NumCpus", 0x4},
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
    {"SecurityFlag", 0xC4},
    {"TzAppsRegnAddr", 0x86D00000},
    {"TzAppsRegnSize", 0x4B00000},
    {"EnableLogFsSyncInRetail", 0x0},
    {"DetectRetailUserAttentionHotkey", 0x0},
    {"DetectRetailUserAttentionHotkeyCode", 0x17},
    {"EnableOEMSetupAppInRetail", 0x0},
    {"EnableMultiThreading", 0x1},
    {"MaxCoreCnt", 0x8},
    {"EarlyInitCoreCnt", 0x1},
    {"EnableDisplayThread", 0x0},
    {"AllowNonPersistentVarsInRetail", 0x1},
    {"EnableUefiSecAppDebugLogDump", 0x0}
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
