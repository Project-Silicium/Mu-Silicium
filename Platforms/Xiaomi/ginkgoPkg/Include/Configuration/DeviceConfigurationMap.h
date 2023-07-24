#ifndef _DEVICE_CONFIGURATION_MAP_H_
#define _DEVICE_CONFIGURATION_MAP_H_

#define CONFIGURATION_NAME_MAX_LENGTH 64

typedef struct {
  CHAR8                        Name[CONFIGURATION_NAME_MAX_LENGTH];
  UINT64                       Value;
} CONFIGURATION_DESCRIPTOR_EX, *PCONFIGURATION_DESCRIPTOR_EX;

static CONFIGURATION_DESCRIPTOR_EX gDeviceConfigurationDescriptorEx[] = {
    {"AllowNonPersistentVarsInRetail", 0x1},
    {"DloadCookieAddr", 0x003D3000},
    {"DloadCookieValue", 0x10},
    {"EarlyInitCoreCnt", 1},
    {"EnableDisplayThread", 1},
	{"EnableLogFsSyncInRetail", 0x0},
    {"EnableMultiThreading", 1},
    {"EnableSDHCSwitch", 0x1},
    {"EnableShell", 0x1},
    {"EnableUfsIOC", 0},
    {"MaxCoreCnt", 8},
    {"MaxLogFileSize", 0x400000},
    {"NumActiveCores", 8},
    {"NumCpus", 8},
	{"NumCpusFuseAddr", 0x5C04C},
    {"PwrBtnShutdownFlag", 0x0},
    {"Sdc1GpioConfigOff", 0xA00},
    {"Sdc1GpioConfigOn", 0x1E92},
    {"Sdc2GpioConfigOff", 0xA00},
    {"Sdc2GpioConfigOn", 0x1E92},
    {"SecurityFlag", 0x1C77},
    {"SharedIMEMBaseAddr", 0x0C125000},
    {"ShmBridgememSize", 0xA00000},
    {"TzAppsRegnAddr", 0x46D00000},
    {"TzAppsRegnSize", 0x02200000},
	{"UefiMemUseThreshold", 0x77},
    {"UfsSmmuConfigForOtherBootDev", 1},
    {"UsbFnIoRevNum", 0x00010001},
    {"USBHS1_Config", 0x0},
    /* Terminator */
    {"Terminator", 0xFFFFFFFF}};

#endif
