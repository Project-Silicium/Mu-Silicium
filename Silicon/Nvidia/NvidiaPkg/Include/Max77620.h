/**
  Max77620.h - MAX77620 PMIC Register Definitions

  Copyright (C) 2016 NVIDIA CORPORATION
  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#ifndef _MAX77620_H_
#define _MAX77620_H_

//
// I2C Address
//
#define MAX77620_I2C_ADDR             0x3C

//
// Global Registers
//
#define MAX77620_REG_CNFGGLBL1        0x00
#define MAX77620_REG_CID4             0x5C

//
// SD Regulator Registers
//
#define MAX77620_REG_SD0              0x16
#define MAX77620_REG_SD1              0x17
#define MAX77620_REG_SD2              0x18
#define MAX77620_REG_SD3              0x19
#define MAX77620_REG_SD0_CFG          0x1D
#define MAX77620_REG_SD1_CFG          0x1E
#define MAX77620_REG_SD2_CFG          0x1F
#define MAX77620_REG_SD3_CFG          0x20
#define MAX77620_REG_SD_CFG2          0x22

//
// LDO Regulator Registers
//
#define MAX77620_REG_LDO0_CFG         0x23
#define MAX77620_REG_LDO0_CFG2        0x24
#define MAX77620_REG_LDO1_CFG         0x25
#define MAX77620_REG_LDO1_CFG2        0x26
#define MAX77620_REG_LDO2_CFG         0x27
#define MAX77620_REG_LDO2_CFG2        0x28
#define MAX77620_REG_LDO3_CFG         0x29
#define MAX77620_REG_LDO3_CFG2        0x2A
#define MAX77620_REG_LDO4_CFG         0x2B
#define MAX77620_REG_LDO4_CFG2        0x2C
#define MAX77620_REG_LDO5_CFG         0x2D
#define MAX77620_REG_LDO5_CFG2        0x2E
#define MAX77620_REG_LDO6_CFG         0x2F
#define MAX77620_REG_LDO6_CFG2        0x30
#define MAX77620_REG_LDO7_CFG         0x31
#define MAX77620_REG_LDO7_CFG2        0x32
#define MAX77620_REG_LDO8_CFG         0x33
#define MAX77620_REG_LDO8_CFG2        0x34

//
// FPS Registers
//
#define MAX77620_REG_FPS_LDO0         0x46
#define MAX77620_REG_FPS_LDO1         0x47
#define MAX77620_REG_FPS_LDO2         0x48
#define MAX77620_REG_FPS_LDO3         0x49
#define MAX77620_REG_FPS_LDO4         0x4A
#define MAX77620_REG_FPS_LDO5         0x4B
#define MAX77620_REG_FPS_LDO6         0x4C
#define MAX77620_REG_FPS_LDO7         0x4D
#define MAX77620_REG_FPS_LDO8         0x4E
#define MAX77620_REG_FPS_SD0          0x4F
#define MAX77620_REG_FPS_SD1          0x50
#define MAX77620_REG_FPS_SD2          0x51
#define MAX77620_REG_FPS_SD3          0x52

//
// Status Register
//
#define MAX77620_REG_STATSD           0x14

//
// Voltage Masks
//
#define MAX77620_SDX_VOLT_MASK        0xFF
#define MAX77620_SD0_VOLT_MASK        0x3F
#define MAX77620_SD1_VOLT_MASK        0x7F
#define MAX77620_LDO_VOLT_MASK        0x3F

//
// Power Mode Bits
//
#define MAX77620_SD_POWER_MODE_MASK   0x30
#define MAX77620_SD_POWER_MODE_SHIFT  4
#define MAX77620_LDO_POWER_MODE_MASK  0xC0
#define MAX77620_LDO_POWER_MODE_SHIFT 6

//
// Global Config 1 Bits
//
#define MAX77620_CNFGGLBL1_LBDAC_EN   (1 << 7)
#define MAX77620_CNFGGLBL1_LBHYST_N   (1 << 4)
#define MAX77620_CNFGGLBL1_LBDAC_N    (1 << 1)

//
// FPS Source
//
typedef enum {
  MAX77620_FPS_SRC_0 = 0,
  MAX77620_FPS_SRC_1,
  MAX77620_FPS_SRC_2,
  MAX77620_FPS_SRC_NONE,
} MAX77620_FPS_SRC;

//
// Regulator IDs
//
typedef enum {
  MAX77620_REGULATOR_SD0 = 0,
  MAX77620_REGULATOR_SD1,
  MAX77620_REGULATOR_SD2,
  MAX77620_REGULATOR_SD3,
  MAX77620_REGULATOR_LDO0,
  MAX77620_REGULATOR_LDO1,
  MAX77620_REGULATOR_LDO2,
  MAX77620_REGULATOR_LDO3,
  MAX77620_REGULATOR_LDO4,
  MAX77620_REGULATOR_LDO5,
  MAX77620_REGULATOR_LDO6,
  MAX77620_REGULATOR_LDO7,
  MAX77620_REGULATOR_LDO8,
  MAX77620_REGULATOR_MAX,
} MAX77620_REGULATOR_ID;

#endif
