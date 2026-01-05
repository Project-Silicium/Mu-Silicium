/**
  T210.h - Tegra X1 (T210) Base Address Definitions

  Copyright (c) 2018 naehrwert
  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#ifndef _T210_H_
#define _T210_H_

//
// System Registers
//
#define PG_UP_BASE              0x60000000
#define TMR_BASE                0x60005000
#define CLOCK_BASE              0x60006000
#define FLOW_CTLR_BASE          0x60007000
#define SYSREG_BASE             0x6000C000
#define SB_BASE                 (SYSREG_BASE + 0x200)

//
// GPIO
//
#define GPIO_BASE               0x6000D000

//
// Exception Vectors
//
#define EXCP_VEC_BASE           0x6000F000

//
// APB Peripherals
//
#define APB_MISC_BASE           0x70000000
#define PINMUX_AUX_BASE         0x70003000
#define UART_BASE               0x70006000

//
// I2C Controllers
//
#define I2C1_BASE               0x7000C000
#define I2C2_BASE               0x7000C400
#define I2C3_BASE               0x7000C500
#define I2C4_BASE               0x7000C700
#define I2C5_BASE               0x7000D000
#define I2C6_BASE               0x7000D100

//
// Power Management
//
#define RTC_BASE                0x7000E000
#define PMC_BASE                0x7000E400
#define SYSCTR0_BASE            0x7000F000
#define FUSE_BASE               0x7000F800
#define KFUSE_BASE              0x7000FC00

//
// Security Engine
//
#define SE_BASE                 0x70012000

//
// Memory Controllers
//
#define MC_BASE                 0x70019000
#define EMC_BASE                0x7001B000

//
// SDMMC Controllers
//
#define SDMMC1_BASE             0x700B0000
#define SDMMC2_BASE             0x700B0200
#define SDMMC3_BASE             0x700B0400
#define SDMMC4_BASE             0x700B0600

//
// Controller IDs
//
#define SDMMC_1                 0
#define SDMMC_2                 1
#define SDMMC_3                 2
#define SDMMC_4                 3

#define I2C_1                   0
#define I2C_2                   1
#define I2C_3                   2
#define I2C_4                   3
#define I2C_5                   4
#define I2C_6                   5

//
// APB MISC Registers
//
#define APB_MISC_GP_SDMMC1_CLK_LPBK_CONTROL 0x8D4
#define APB_MISC_GP_SDMMC1_PAD_CFGPADCTRL   0xA98

#endif
