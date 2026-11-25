/**
  Copyright (c) 2018 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Library/UsiLib.h>

STATIC
EFI_USI_DATA
gUsiData[] = {
  {
    .ControllerAddress = 0x138A0000,
    .I2cBus            = 0,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersionNone
  },
  {
    .ControllerAddress = 0x138B0000,
    .I2cBus            = 1,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersionNone
  },
  {
    .ControllerAddress = 0x138C0000,
    .I2cBus            = 2,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersionNone
  },
  {
    .ControllerAddress = 0x138D0000,
    .I2cBus            = 3,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersionNone
  },
  {
    .UsiAddress        = 0x10032000,
    .ControllerAddress = 0x13920000,
    .I2cBus            = 4,
    .SpiBus            = 2,
    .UartBus           = 3,
    .Version           = UsiVersion1,
    .Channel           = UsiChannel0
  },
  {
    .UsiAddress        = 0x10032000,
    .ControllerAddress = 0x13930000,
    .I2cBus            = 5,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersion1,
    .Channel           = UsiChannel1
  },
  {
    .UsiAddress        = 0x10032004,
    .ControllerAddress = 0x13940000,
    .I2cBus            = 6,
    .SpiBus            = 3,
    .UartBus           = 4,
    .Version           = UsiVersion1,
    .Channel           = UsiChannel0
  },
  {
    .UsiAddress        = 0x10032004,
    .ControllerAddress = 0x13950000,
    .I2cBus            = 7,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersion1,
    .Channel           = UsiChannel1
  },
  {
    .UsiAddress        = 0x10032008,
    .ControllerAddress = 0x13980000,
    .I2cBus            = 8,
    .SpiBus            = 4,
    .UartBus           = 5,
    .Version           = UsiVersion1,
    .Channel           = UsiChannel0
  },
  {
    .UsiAddress        = 0x10032008,
    .ControllerAddress = 0x13990000,
    .I2cBus            = 9,
    .SpiBus            = MAX_UINT8,
    .UartBus           = MAX_UINT8,
    .Version           = UsiVersion1,
    .Channel           = UsiChannel1
  }
};

VOID
GetUsiData (
  OUT EFI_USI_DATA **Data,
  OUT UINT8         *Count)
{
  *Data  = gUsiData;
  *Count = ARRAY_SIZE (gUsiData);
}
