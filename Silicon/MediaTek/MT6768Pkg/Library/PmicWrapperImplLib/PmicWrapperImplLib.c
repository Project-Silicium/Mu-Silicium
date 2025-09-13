#include <Protocol/MtkPmicWrapper.h>

MTK_PMIC_WRAPPER_PLATFORM_INFO PlatformInfo = {
  .RegMap = {
    [PMIC_WRAPPER_INIT_DONE2]   = 0x094,
    [PMIC_WRAPPER_WACS2_CMD]    = 0xC20,
    [PMIC_WRAPPER_WACS2_RDATA]  = 0xC24,
    [PMIC_WRAPPER_WACS2_VLDCLR] = 0xC28,
  }
};
