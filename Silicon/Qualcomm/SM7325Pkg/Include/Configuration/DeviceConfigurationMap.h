#ifndef _DEVICE_CONFIGURATION_MAP_H_
#define _DEVICE_CONFIGURATION_MAP_H_

#define CONFIGURATION_NAME_MAX_LENGTH 64

typedef struct {
  CHAR8                        Name[CONFIGURATION_NAME_MAX_LENGTH];
  UINT64                       Value;
} CONFIGURATION_DESCRIPTOR_EX, *PCONFIGURATION_DESCRIPTOR_EX;

static CONFIGURATION_DESCRIPTOR_EX gDeviceConfigurationDescriptorEx[] = {
//    {"AbnormalResetOccurredOffset", 0x24},
//    {"BootConfigRegAddr", 0x00786070},
//    {"BootConfigRegMask", 0x0000003E},
//    {"BootConfigRegOfset", 0x01},
    {"AllowNonPersistentVarsInRetail", 0x1},
//    {"DBIDumpDDRBase", 0x82A00000},
//    {"DDRThroughput", 6000000000},
    {"DDRInfoNotifyFlag", 0x0},
    {"DRAM_CLK_PERIOD_ADDR", 0x240BA050},
    {"DetectRetailUserAttentionHotkey", 0x00},
    {"DetectRetailUserAttentionHotkeyCode", 0x17},
    {"DloadCookieAddr", 0x01FD3000},
    {"DloadCookieValue", 0x10},
    {"EarlyInitCoreCnt", 2},
    {"EnableACPIFallback", 0x0},
    {"EnableDisplayImageFv", 0x0},
    {"EnableLogFsSyncInRetail", 0x0},
    {"EnableDisplayThread", 0},
    {"EnableMultiThreading", 1},
    {"EnableMultiCoreFvDecompression", 1},
    {"EnableOEMSetupAppInRetail", 0x0},
//    {"EnablePXE", 0x0},
    {"EUDEnableAddr", 0x88E2000},
    {"EnableSDHCSwitch", 0x1},
//    {"EnableSecondaryUFSforHLOS", 0},
    {"EnableShell", 0x1},
    {"EnableUfsIOC", 1},
    {"EnableUefiSecAppDebugLogDump", 0x0},
    {"EnableVariablePolicyEngine", 0},
    {"InitialPagePoolCount", 0x700},
    {"LoadAutoImageInPILFlag", 0x1},
//    {"GccCe1ClkCntlReg", 0x00152004},
//    {"GccCe1ClkCntlVal", 0x00000038},
    {"MaxCoreCnt", 8},
    {"MaxLogFileSize", 0x400000},
    {"MinidumpTALoadingCfg", 0},
//    {"MemoryCaptureModeOffset", 0x1C},
    {"NumActiveCores", 8},
    {"NumCpus", 8},
    {"NumCpusFuseAddr", 0x5C04C},
    {"PilSubsysDbgCookieAddr", 0x146AA6DC},
    {"PilSubsysDbgCookieVal", 0x53444247},
//    {"PSHoldOffset", 0xC000},
//    {"PSHoldSHFT", 0x0},
    {"PrimaryLogoIndex", 0x3},
    {"SecondaryLogoIndex", 0x2},
    {"PwrBtnShutdownFlag", 0x0},
    {"Sdc1GpioConfigOff", 0xA00},
    {"Sdc1GpioConfigOn", 0x1E92},
    {"Sdc2GpioConfigOff", 0xA00},
    {"Sdc2GpioConfigOn", 0x1E92},
    {"SecurityFlag", 0xC4},
    {"SecPagePoolCount", 0x800},
//    {"SerialPortBufferSize", 0x8000},
    {"SharedIMEMBaseAddr", 0x146AA000},
    {"ShmBridgememSize", 0xA00000},
//    {"TzAppsRegnAddr", 0x87900000},
//    {"TzAppsRegnSize", 0x02200000},
//    {"TzDiagOffset", 0x720},
//    {"TzDiagSize", 0x2000},
    {"UfsSmmuConfigForOtherBootDev", 1},
    {"UEFIExitUfsSSURequired", 1},
    {"UsbFnIoRevNum", 0x00010001},
    {"UefiMemUseThreshold", 0xE1},
    {"USBHS1_Config", 0x0},

    /* Terminator */
    {"Terminator", 0xFFFFFFFF}};

#endif
