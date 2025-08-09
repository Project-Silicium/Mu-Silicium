#ifndef __I2CHOSTTABLELIB_H__
#define __I2CHOSTTABLELIB_H__

#include <Protocol/EfiI2C.h>

I2CHost *
GetI2CHosts (VOID);

EFI_STATUS
GetI2CHostById (
  INTN TargetId,
  OUT I2CHost Host);

#endif // __I2CHOSTTABLELIB_H__
