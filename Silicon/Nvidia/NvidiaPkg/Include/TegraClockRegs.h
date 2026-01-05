/**
  TegraClockRegs.h - Tegra Clock Reset Controller Register Definitions

  Copyright (c) 2018 naehrwert
  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#ifndef _TEGRA_CLOCK_REGS_H_
#define _TEGRA_CLOCK_REGS_H_

//
// Clock Source Registers
//
#define CLK_RST_CONTROLLER_CLK_SOURCE_I2C1    0x124
#define CLK_RST_CONTROLLER_CLK_SOURCE_I2C2    0x198
#define CLK_RST_CONTROLLER_CLK_SOURCE_I2C3    0x1B8
#define CLK_RST_CONTROLLER_CLK_SOURCE_I2C4    0x3C4
#define CLK_RST_CONTROLLER_CLK_SOURCE_I2C5    0x128
#define CLK_RST_CONTROLLER_CLK_SOURCE_I2C6    0x65C
#define CLK_RST_CONTROLLER_CLK_SOURCE_SDMMC1  0x150
#define CLK_RST_CONTROLLER_CLK_SOURCE_SDMMC4  0x164

//
// Set/Clear Registers
//
#define CLK_RST_CONTROLLER_RST_DEV_L_SET      0x300
#define CLK_RST_CONTROLLER_RST_DEV_L_CLR      0x304
#define CLK_RST_CONTROLLER_RST_DEV_H_SET      0x308
#define CLK_RST_CONTROLLER_RST_DEV_H_CLR      0x30C
#define CLK_RST_CONTROLLER_RST_DEV_U_SET      0x310
#define CLK_RST_CONTROLLER_RST_DEV_U_CLR      0x314
#define CLK_RST_CONTROLLER_CLK_ENB_L_SET      0x320
#define CLK_RST_CONTROLLER_CLK_ENB_L_CLR      0x324
#define CLK_RST_CONTROLLER_CLK_ENB_H_SET      0x328
#define CLK_RST_CONTROLLER_CLK_ENB_H_CLR      0x32C
#define CLK_RST_CONTROLLER_CLK_ENB_U_SET      0x330
#define CLK_RST_CONTROLLER_CLK_ENB_U_CLR      0x334
#define CLK_RST_CONTROLLER_RST_DEV_V_CLR      0x434
#define CLK_RST_CONTROLLER_CLK_ENB_V_SET      0x440
#define CLK_RST_CONTROLLER_CLK_ENB_X_SET      0x284
#define CLK_RST_CONTROLLER_RST_DEV_X_CLR      0x294

//
// Clock L Bank Bits
//
#define CLK_L_I2C1              (1 << 12)
#define CLK_L_SDMMC1            (1 << 14)
#define CLK_L_SDMMC4            (1 << 15)

//
// Clock H Bank Bits
//
#define CLK_H_I2C5              (1 << 15)
#define CLK_H_I2C2              (1 << 22)

//
// Clock U Bank Bits
//
#define CLK_U_I2C3              (1 << 3)

//
// Clock V Bank Bits
//
#define CLK_V_I2C4              (1 << 7)

//
// Clock X Bank Bits
//
#define CLK_X_I2C6              (1 << 6)

#endif
