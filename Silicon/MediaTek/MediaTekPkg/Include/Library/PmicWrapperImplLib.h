#ifndef _PMIC_WRAPPER_IMPL_LIB_H_
#define _PMIC_WRAPPER_IMPL_LIB_H_

typedef enum {
  PmicWrapperInitDone2,
  PmicWrapperWacs2Cmd,
  PmicWrapperWacs2VldClr,
  PmicWrapperWacs2RData,
  PmicWrapperSwinf2WData31,
  PmicWrapperSwinf2RData31,
  PmicWrapperMaxReg
} MTK_PMIC_WRAPPER_REG;

typedef struct {
  MTK_PMIC_WRAPPER_REG RegMap[PmicWrapperMaxReg];
  BOOLEAN ArbCapabilities;
} MTK_PMIC_WRAPPER_PLATFORM_INFO;

extern MTK_PMIC_WRAPPER_PLATFORM_INFO gPlatformInfo;

#endif /* _PMIC_WRAPPER_IMPL_LIB_H_ */