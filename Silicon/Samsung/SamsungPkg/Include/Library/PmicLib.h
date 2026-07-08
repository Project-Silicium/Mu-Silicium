#ifndef _PMIC_LIB_H_
#define _PMIC_LIB_H_

#include <Device/Pmic.h>

//
// PMIC Limits
//
#define MAX_SPEEDY_PMIC_COUNT 6
#define MAX_HSI2C_PMIC_COUNT  6

//
// SPEEDY PMIC Data
//
typedef struct {
  EFI_PMIC_ID Id;
  UINT8       BusNumber;
} EFI_SPEEDY_PMIC_DATA;

//
// HSI2C PMIC Data
//
typedef struct {
  EFI_PMIC_ID Id;
  UINT8       BusNumber;
  UINT32      Slave;
} EFI_HSI2C_PMIC_DATA;

//
// PMIC Data
//
typedef struct {
  EFI_SPEEDY_PMIC_DATA Speedy[MAX_SPEEDY_PMIC_COUNT];
  EFI_HSI2C_PMIC_DATA  HsI2c[MAX_HSI2C_PMIC_COUNT];
} EFI_PMIC_DATA;

/**
  This Function gets the Platform PMIC Data.

  @return Data                             - The PMIC Data.
  @return NULL                             - There is no PMIC Data.
**/
EFI_PMIC_DATA*
GetPmicData ();

#endif /* _PMIC_LIB_H_ */
