/**
  Copyright (c) 2018 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Library/HsI2cLib.h>

#define HSI2C_SOURCE_CLOCK 200000000

#define HSI2C_BUS_DATA(BankId, BankNum, Scl, Sda) \
  {                                               \
    {                                             \
      .Function   = FUNCTION_2,                   \
      .SclBankId  = BankId,                       \
      .SdaBankId  = BankId,                       \
      .SclBankNum = BankNum,                      \
      .SdaBankNum = BankNum,                      \
      .SclPin     = Scl,                          \
      .SdaPin     = Sda                           \
    },                                            \
    {                                             \
      .Freq       = HSI2C_SOURCE_CLOCK,           \
      .SpeedMode  = FAST_SPD                      \
    }                                             \
  }

STATIC
EFI_HSI2C_BUS_DATA
gHsI2cBusData[] = {
  [0] = HSI2C_BUS_DATA (BANK_ID_C, 1, 1, 0),
  [1] = HSI2C_BUS_DATA (BANK_ID_C, 1, 3, 2),
  [2] = HSI2C_BUS_DATA (BANK_ID_C, 1, 5, 4),
  [3] = HSI2C_BUS_DATA (BANK_ID_C, 1, 7, 6),
  [4] = HSI2C_BUS_DATA (BANK_ID_C, 2, 1, 0),
  [5] = HSI2C_BUS_DATA (BANK_ID_C, 2, 3, 2),
  [6] = HSI2C_BUS_DATA (BANK_ID_C, 2, 5, 4),
  [7] = HSI2C_BUS_DATA (BANK_ID_C, 2, 7, 6),
  [8] = HSI2C_BUS_DATA (BANK_ID_P, 7, 1, 0),
  [9] = HSI2C_BUS_DATA (BANK_ID_P, 8, 1, 0)
};

VOID
GetHsI2cBusData (
  OUT EFI_HSI2C_BUS_DATA **Data,
  OUT UINT8               *Count)
{
  *Data  = gHsI2cBusData;
  *Count = ARRAY_SIZE (gHsI2cBusData);
}
