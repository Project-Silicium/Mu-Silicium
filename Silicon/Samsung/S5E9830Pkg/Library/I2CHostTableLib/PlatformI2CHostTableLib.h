#ifndef __PLATFORMI2CHOSTTABLELIB_H__
#define __PLATFORMI2CHOSTTABLELIB_H__

#include <Protocol/EfiI2C.h>

STATIC I2CHost S5E9830_I2CHost_Table[] =
{
  { // Haptics
    .Id = 5,
    .Delay = 0,
    .SclBank = (GpioBank *)0x10430000,
    .SclBankOffset = 0x20,
    .SclPin = 2,
    .SdaBank = (GpioBank *)0x10430000,
    .SdaBankOffset = 0x20,
    .SdaPin = 3
  },
  { // FuelGauge
    .Id = 7,
    .Delay = 0,
    .SclBank = (GpioBank *)0x10430000,
    .SclBankOffset = 0x20,
    .SclPin = 6,
    .SdaBank = (GpioBank *)0x10430000,
    .SdaBankOffset = 0x20,
    .SdaPin = 7
  },
  { // Terminator
    .Id = -1,
    .Delay = 0,
    .SclBank = (GpioBank *)0,
    .SclBankOffset = 0,
    .SclPin = 0,
    .SdaBank = (GpioBank *)0,
    .SdaBankOffset = 0,
    .SdaPin = 0
  }
};

enum I2CHosts
{
  HSI2C_5
};

#endif // __PLATFORMI2CHOSTTABLELIB_H__
