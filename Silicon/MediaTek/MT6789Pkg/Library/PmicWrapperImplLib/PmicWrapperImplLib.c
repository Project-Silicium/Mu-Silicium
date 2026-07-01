#include <Library/PmicWrapperImplLib.h>

MTK_PMIC_WRAPPER_PLATFORM_INFO gPlatformInfo = {
  .RegMap = {
    [PmicWrapperInitDone2]     = 0x0,
    [PmicWrapperWacs2Cmd]      = 0x880,
    [PmicWrapperWacs2VldClr]   = 0x8A4,
    [PmicWrapperWacs2RData]    = 0x8A8,
    [PmicWrapperSwinf2WData31] = 0x884,
    [PmicWrapperSwinf2RData31] = 0x894,
  },
  .ArbCapabilities = TRUE,
};
