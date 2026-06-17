/**
  Copyright (C) 2015 Samsung Electrnoics

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
**/

#ifndef _S2MPS_18_H_
#define _S2MPS_18_H_

#include <Device/Pmic.h>

//
// Slave Addresses
//
#define S2MPS18_PM_ADDR            0x01
#define S2MPS18_RTC_ADDR           0x02

//
// PM Register Addresses
//
#define S2MPS18_PM_INT1            0x00
#define S2MPS18_PM_INT2            0x01
#define S2MPS18_PM_INT3            0x02
#define S2MPS18_PM_INT4            0x03
#define S2MPS18_PM_INT5            0x04
#define S2MPS18_PM_INT6            0x05
#define S2MPS18_PM_INT7            0x06
#define S2MPS18_PM_INT1M           0x07
#define S2MPS18_PM_INT2M           0x08
#define S2MPS18_PM_INT3M           0x09
#define S2MPS18_PM_INT4M           0x0A
#define S2MPS18_PM_INT5M           0x0B
#define S2MPS18_PM_INT6M           0x0C
#define S2MPS18_PM_INT7M           0x0D
#define S2MPS18_PM_STATUS1         0x0E
#define S2MPS18_PM_STATUS2         0x0F
#define S2MPS18_PM_PWRONSRC        0x10
#define S2MPS18_PM_OFFSRC          0x11
#define S2MPS18_PM_LDO_CTRL(x)    ((x) < 2 ? (0x48 + (x)) : (0x48 + (x) + 1))

//
// RTC Register Addresses
//
#define S2MPS18_RTC_WTSR_SMPL      0x01
#define S2MPS18_RTC_UPDATE         0x02
#define S2MPS18_RTC_CAP_SEL        0x03
#define S2MPS18_RTC_MSEC           0x04
#define S2MPS18_RTC_SEC            0x05
#define S2MPS18_RTC_MIN            0x06
#define S2MPS18_RTC_HOUR           0x07
#define S2MPS18_RTC_WEEK           0x08
#define S2MPS18_RTC_DAY            0x09
#define S2MPS18_RTC_MON            0x0A
#define S2MPS18_RTC_YEAR           0x0B

//
// WTSR Register Bits
//
#define S2MPS18_RTC_WTSR_SET       BIT6

//
// Regulator Limits
//
#define MAX_S2MPS18_LDO_COUNT      45

//
// LDO Regulator Modes
//
#define S2MPS18_OUTPUT_ON_TCXO     BIT6
#define S2MPS18_OUTPUT_ON_NORMAL  (BIT6 | BIT7)

#endif /* _S2MPS_18_H_ */
