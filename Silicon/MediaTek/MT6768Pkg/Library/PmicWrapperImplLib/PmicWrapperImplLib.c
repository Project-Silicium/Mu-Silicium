#include <Library/PmicWrapperImplLib.h>

MTK_PMIC_WRAPPER_PLATFORM_INFO gPlatformInfo = {
  .RegMap = {
    [PmicWrapperInitDone2]   = 0x094,
    [PmicWrapperWacs2Cmd]    = 0xC20,
    [PmicWrapperWacs2RData]  = 0xC24,
    [PmicWrapperWacs2VldClr] = 0xC28,
  }
};
