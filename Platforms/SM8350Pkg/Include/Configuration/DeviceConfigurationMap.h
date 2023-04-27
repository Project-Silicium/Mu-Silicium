#ifndef _DEVICE_CONFIGURATION_MAP_H_
#define _DEVICE_CONFIGURATION_MAP_H_

#define CONFIGURATION_NAME_MAX_LENGTH 64

typedef struct {
  CHAR8                        Name[CONFIGURATION_NAME_MAX_LENGTH];
  UINT64                       Value;
} CONFIGURATION_DESCRIPTOR_EX, *PCONFIGURATION_DESCRIPTOR_EX;

static CONFIGURATION_DESCRIPTOR_EX gDeviceConfigurationDescriptorEx[] = {
    {"AbnormalResetOccurredOffset", 0x24},
    {"AllowNonPersistentVarsInRetail", 0x1},
    {"DDRInfoNotifyFlag", 0x0},
    {"DetectRetailUserAttentionHotkey", 0x00},
    {"DetectRetailUserAttentionHotkeyCode", 0x17},
    {"DisableSubSysCrshHandler", 1},
    {"DloadCookieAddr", 0x01FD3000},
    {"DloadCookieValue", 0x10},
    {"EarlyInitCoreCnt", 2},
    {"EnableACPIFallback", 0x0},
    {"EnableChargerPdSupport", 0},
    {"EnableDisplayImageFv", 0x0},
    {"EnableDisplayThread", 0x0},
    {"EnableLogFsSyncInRetail", 0x1},
    {"EnableMultiCoreFvDecompression", 0},
    {"EnableMultiThreading", 0},
    {"EnableOEMSetupAppInRetail", 0x0},
    {"EnablePXE", 0x0},
    {"EnableSDHCSwitch", 0x1},
    {"EnableShell", 0x1},
    {"EnableUefiSecAppDebugLogDump", 0x1},
    {"EnableUfsIOC", 1},
    {"EnableVariablePolicyEngine", 7},
    {"EUDEnableAddr", 0x88E2000},
    {"InitialPagePoolCount", 0x900},
    {"MaxCoreCnt", 8},
    {"MaxLogFileSize", 0x400000},
    {"MinidumpTALoadingCfg", 0x0},
    {"MemoryCaptureModeOffset", 0x1C},
    {"NumActiveCores", 8},
    {"NumCpus", 8},
    {"NumCpusFuseAddr", 0x5C04C},
    {"PwrBtnShutdownFlag", 0x0},
    {"Sdc1GpioConfigOff", 0xA00},
    {"Sdc1GpioConfigOn", 0x1E92},
    {"Sdc2GpioConfigOff", 0xA00},
    {"Sdc2GpioConfigOn", 0x1E92},
    {"SecPagePoolCount", 0x9B9},
    {"SecurityFlag", 0x1C77},
    {"SharedIMEMBaseAddr", 0x146BF000},
    {"ShmBridgememSize", 0xA00000},
    {"SmcInvokeConfig", 0x20040100},
    {"TzDiagOffset", 0x720},
    {"TzDiagSize", 0x2000},
    {"UARTLogBufferSize", 0x100000},
    {"UefiMemUseThreshold", 0xD8},
    {"UfsSmmuConfigForOtherBootDev", 1},
    {"UsbFnIoRevNum", 0x00010001},
    {"USBHS1_Config", 0x0},
    /* Terminator */
    {"Terminator", 0xFFFFFFFF}};

#endif
