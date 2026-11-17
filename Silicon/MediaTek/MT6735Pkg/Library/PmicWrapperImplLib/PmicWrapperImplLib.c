#include <Library/PmicWrapperImplLib.h>

MTK_PMIC_WRAPPER_PLATFORM_INFO gPlatformInfo = {
  .RegMap = {
    [PmicWrapperInitDone2]   = 0x09C,
    [PmicWrapperWacs2Cmd]    = 0x0A0,
    [PmicWrapperWacs2RData]  = 0x0A4,
    [PmicWrapperWacs2VldClr] = 0x0A8,
  },
  .ArbCapabilities = FALSE,
};
