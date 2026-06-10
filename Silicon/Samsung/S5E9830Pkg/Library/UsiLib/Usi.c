/**
  Copyright (c) 2019 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Library/UsiLib.h>

STATIC
EFI_USI_DATA
gUsiData[] = {
  {
    .UsiAddress        = 0x10421000,
    .ControllerAddress = 0x10540000,
    .I2cBus            = MAX_UINT8,
    .SpiBus            = MAX_UINT8,
    .UartBus           = 0
  },
  {
    .UsiAddress        = 0x10421004,
    .ControllerAddress = 0x10550000,
    .I2cBus            = 0,
    .SpiBus            = 0,
    .UartBus           = 2
  },
  {
    .UsiAddress        = 0x10421008,
    .ControllerAddress = 0x10560000,
    .I2cBus            = 1,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x1042100C,
    .ControllerAddress = 0x10570000,
    .I2cBus            = 2,
    .SpiBus            = 1,
    .UartBus           = 3
  },
  {
    .UsiAddress        = 0x10421010,
    .ControllerAddress = 0x10580000,
    .I2cBus            = 3,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10421014,
    .ControllerAddress = 0x10590000,
    .I2cBus            = 4,
    .SpiBus            = 2,
    .UartBus           = 4
  },
  {
    .UsiAddress        = 0x10421018,
    .ControllerAddress = 0x105A0000,
    .I2cBus            = 5,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x1042101C,
    .ControllerAddress = 0x105B0000,
    .I2cBus            = 6,
    .SpiBus            = 3,
    .UartBus           = 5
  },
  {
    .UsiAddress        = 0x10421020,
    .ControllerAddress = 0x105C0000,
    .I2cBus            = 7,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10421024,
    .ControllerAddress = 0x105D0000,
    .I2cBus            = 8,
    .SpiBus            = 4,
    .UartBus           = 6
  },
  {
    .UsiAddress        = 0x10421028,
    .ControllerAddress = 0x105E0000,
    .I2cBus            = 9,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x1042102C,
    .ControllerAddress = 0x105F0000,
    .I2cBus            = 10,
    .SpiBus            = 5,
    .UartBus           = 7
  },
  {
    .UsiAddress        = 0x10421030,
    .ControllerAddress = 0x10600000,
    .I2cBus            = 11,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x1042103C,
    .ControllerAddress = 0x10630000,
    .I2cBus            = 26,
    .SpiBus            = 13,
    .UartBus           = 15
  },
  {
    .UsiAddress        = 0x10421040,
    .ControllerAddress = 0x10640000,
    .I2cBus            = 27,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10421044,
    .ControllerAddress = 0x10650000,
    .I2cBus            = 28,
    .SpiBus            = 14,
    .UartBus           = 16
  },
  {
    .UsiAddress        = 0x10421048,
    .ControllerAddress = 0x10660000,
    .I2cBus            = 29,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x1042104C,
    .ControllerAddress = 0x10670000,
    .I2cBus            = 30,
    .SpiBus            = 15,
    .UartBus           = 17
  },
  {
    .UsiAddress        = 0x10421050,
    .ControllerAddress = 0x10680000,
    .I2cBus            = 31,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721000,
    .ControllerAddress = 0x10840000,
    .I2cBus            = MAX_UINT8,
    .SpiBus            = MAX_UINT8,
    .UartBus           = 1
  },
  {
    .UsiAddress        = 0x10721018,
    .ControllerAddress = 0x108A0000,
    .I2cBus            = 12,
    .SpiBus            = 6,
    .UartBus           = 8
  },
  {
    .UsiAddress        = 0x1072101C,
    .ControllerAddress = 0x108B0000,
    .I2cBus            = 13,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721020,
    .ControllerAddress = 0x108C0000,
    .I2cBus            = 14,
    .SpiBus            = 7,
    .UartBus           = 9
  },
  {
    .UsiAddress        = 0x10721024,
    .ControllerAddress = 0x108D0000,
    .I2cBus            = 15,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721028,
    .ControllerAddress = 0x108E0000,
    .I2cBus            = 16,
    .SpiBus            = 8,
    .UartBus           = 10
  },
  {
    .UsiAddress        = 0x1072102C,
    .ControllerAddress = 0x108F0000,
    .I2cBus            = 17,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721030,
    .ControllerAddress = 0x10900000,
    .I2cBus            = 18,
    .SpiBus            = 9,
    .UartBus           = 11
  },
  {
    .UsiAddress        = 0x10721034,
    .ControllerAddress = 0x10910000,
    .I2cBus            = 19,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721038,
    .ControllerAddress = 0x10920000,
    .I2cBus            = 20,
    .SpiBus            = 10,
    .UartBus           = 12
  },
  {
    .UsiAddress        = 0x1072103C,
    .ControllerAddress = 0x10930000,
    .I2cBus            = 21,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721040,
    .ControllerAddress = 0x10940000,
    .I2cBus            = 22,
    .SpiBus            = 11,
    .UartBus           = 13
  },
  {
    .UsiAddress        = 0x10721044,
    .ControllerAddress = 0x10950000,
    .I2cBus            = 23,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721048,
    .ControllerAddress = 0x10960000,
    .I2cBus            = 32,
    .SpiBus            = 16,
    .UartBus           = 18
  },
  {
    .UsiAddress        = 0x1072104C,
    .ControllerAddress = 0x10970000,
    .I2cBus            = 33,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10721050,
    .ControllerAddress = 0x10990000,
    .I2cBus            = 34,
    .SpiBus            = 17,
    .UartBus           = 19
  },
  {
    .UsiAddress        = 0x10721054,
    .ControllerAddress = 0x109A0000,
    .I2cBus            = 35,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10722000,
    .ControllerAddress = 0x109C0000,
    .I2cBus            = 24,
    .SpiBus            = 12,
    .UartBus           = 14
  },
  {
    .UsiAddress        = 0x10722004,
    .ControllerAddress = 0x109D0000,
    .I2cBus            = 25,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x10722008,
    .ControllerAddress = 0x109E0000,
    .I2cBus            = 36,
    .SpiBus            = 18,
    .UartBus           = 20
  },
  {
    .UsiAddress        = 0x1072200C,
    .ControllerAddress = 0x109F0000,
    .I2cBus            = 37,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x15C22000,
    .ControllerAddress = 0x15D00000,
    .I2cBus            = 38,
    .SpiBus            = 19,
    .UartBus           = 21
  },
  {
    .UsiAddress        = 0x15C22004,
    .ControllerAddress = 0x15D10000,
    .I2cBus            = 39,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x15C22010,
    .ControllerAddress = 0x15D20000,
    .I2cBus            = 40,
    .SpiBus            = 20,
    .UartBus           = 22
  },
  {
    .UsiAddress        = 0x15C22014,
    .ControllerAddress = 0x15D30000,
    .I2cBus            = 41,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x15C22020,
    .ControllerAddress = 0x15D40000,
    .I2cBus            = 42,
    .SpiBus            = 21,
    .UartBus           = 23
  },
  {
    .UsiAddress        = 0x15C22024,
    .ControllerAddress = 0x15D50000,
    .I2cBus            = 43,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  },
  {
    .UsiAddress        = 0x15C22030,
    .ControllerAddress = 0x15D60000,
    .I2cBus            = 44,
    .SpiBus            = 22,
    .UartBus           = 24
  },
  {
    .UsiAddress        = 0x15C22034,
    .ControllerAddress = 0x15D70000,
    .I2cBus            = 45,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8
  }
};

VOID
GetUsiData (
  OUT EFI_USI_DATA **Data,
  OUT UINT8         *Count)
{
  // Pass Data
  *Data  = gUsiData;
  *Count = ARRAY_SIZE (gUsiData);
}
