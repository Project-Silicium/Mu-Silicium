/**
  Copyright (c) 2018 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Library/I2cLib.h>

#define I2C_SOURCE_CLOCK  200000000
#define I2C_BUS_CLOCK     100000

#define I2C_BUS_DATA(BaseAddress, BankId, BankNum, Scl, Sda) \
  {                                                       \
    .Address = BaseAddress,                               \
    .Gpio = {                                             \
      .Function   = FUNCTION_2,                           \
      .Pull       = PULL_UP,                              \
      .SclBankId  = BankId,                               \
      .SdaBankId  = BankId,                               \
      .SclBankNum = BankNum,                              \
      .SdaBankNum = BankNum,                              \
      .SclPin     = Scl,                                  \
      .SdaPin     = Sda                                   \
    },                                                    \
    .Clock = {                                            \
      .SourceFrequency = I2C_SOURCE_CLOCK,                \
      .BusFrequency    = I2C_BUS_CLOCK,                   \
      .SdaDelay        = 0                                \
    }                                                     \
  }

STATIC
EFI_I2C_BUS_DATA
gI2cBusData[] = {
  [0] = I2C_BUS_DATA (0x13830000, BANK_ID_P, 1, 1, 0),
  [1] = I2C_BUS_DATA (0x13840000, BANK_ID_P, 1, 3, 2),
  [2] = I2C_BUS_DATA (0x13850000, BANK_ID_P, 2, 1, 0),
  [3] = I2C_BUS_DATA (0x13860000, BANK_ID_P, 3, 1, 0),
  [4] = I2C_BUS_DATA (0x13870000, BANK_ID_P, 4, 1, 0),
  [5] = I2C_BUS_DATA (0x13880000, BANK_ID_P, 4, 3, 2),
  [6] = I2C_BUS_DATA (0x13890000, BANK_ID_P, 4, 5, 4),
  [7] = I2C_BUS_DATA (0x11CD0000, BANK_ID_G, 1, 5, 4)
};

VOID
GetI2cBusData (
  OUT EFI_I2C_BUS_DATA **Data,
  OUT UINT8             *Count)
{
  *Data  = gI2cBusData;
  *Count = ARRAY_SIZE (gI2cBusData);
}
