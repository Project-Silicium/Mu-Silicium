/**
  Copyright (c) 2017 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#ifndef _USI_H_
#define _USI_H_

//
// USIv1 Modes
//
#define USI_V1_NONE_SW_CONF       0x0
#define USI_V1_I2C0_SW_CONF       0x1
#define USI_V1_I2C1_SW_CONF       0x2
#define USI_V1_I2C0_I2C1_SW_CONF 0x3
#define USI_V1_SPI_SW_CONF        0x4
#define USI_V1_UART_SW_CONF       0x8
#define USI_V1_UART_I2C1_SW_CONF  0xA
#define USI_V1_SW_CONF_MASK       0xF

//
// USIv2 Modes
//
#define USI_V2_UART_SW_CONF BIT0
#define USI_V2_SPI_SW_CONF  BIT1
#define USI_V2_I2C_SW_CONF  BIT2
#define USI_V2_SW_CONF_MASK (USI_V2_UART_SW_CONF | USI_V2_SPI_SW_CONF | USI_V2_I2C_SW_CONF)

//
// USIv2 Register Offsets
//
#define USI_V2_CON_OFFSET 0xC4

#endif /* _USI_H_ */
