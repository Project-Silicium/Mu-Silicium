/**
  TegraPinmuxLib.h - Tegra Pinmux Library Header

  Copyright (c) 2018 naehrwert
  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#ifndef _TEGRA_PINMUX_LIB_H_
#define _TEGRA_PINMUX_LIB_H_

#include <Uefi.h>

//
// Pinmux Config Bits
//
#define PINMUX_FUNC_DEFAULT     0
#define PINMUX_PULL_NONE        (0 << 2)
#define PINMUX_PULL_DOWN        (1 << 2)
#define PINMUX_PULL_UP          (2 << 2)
#define PINMUX_INPUT_ENABLE     (1 << 6)
#define PINMUX_DRIVE_2X         (1 << 13)

//
// SDMMC1 Pin Indices
//
#define PINMUX_SDMMC1_CLK_INDEX   0
#define PINMUX_SDMMC1_CMD_INDEX   1
#define PINMUX_SDMMC1_DAT3_INDEX  2
#define PINMUX_SDMMC1_DAT2_INDEX  3
#define PINMUX_SDMMC1_DAT1_INDEX  4
#define PINMUX_SDMMC1_DAT0_INDEX  5

//
// I2C Pin Indices
//
#define PINMUX_GEN1_I2C_SDA_INDEX 47
#define PINMUX_GEN1_I2C_SCL_INDEX 48
#define PINMUX_GEN2_I2C_SCL_INDEX 49
#define PINMUX_GEN2_I2C_SDA_INDEX 50
#define PINMUX_GEN3_I2C_SCL_INDEX 51
#define PINMUX_GEN3_I2C_SDA_INDEX 52
#define PINMUX_CAM_I2C_SCL_INDEX  53
#define PINMUX_CAM_I2C_SDA_INDEX  54
#define PINMUX_PWR_I2C_SCL_INDEX  55
#define PINMUX_PWR_I2C_SDA_INDEX  56

//
// GPIO Pin Index
//
#define PINMUX_GPIO_PZ1_INDEX     160

//
// I2C Controller IDs
//
#define PINMUX_I2C1 0
#define PINMUX_I2C2 1
#define PINMUX_I2C3 2
#define PINMUX_I2C4 3
#define PINMUX_I2C5 4

/**
  Set pinmux configuration.

  @param[in] PinIndex  Pin index.
  @param[in] Config    Configuration value.
**/
VOID
EFIAPI
PinmuxSetConfig (
  IN UINT32 PinIndex,
  IN UINT32 Config
  );

/**
  Get pinmux configuration.

  @param[in] PinIndex  Pin index.

  @return Configuration value.
**/
UINT32
EFIAPI
PinmuxGetConfig (
  IN UINT32 PinIndex
  );

/**
  Configure pins for I2C controller.

  @param[in] I2cIndex  I2C controller index (0-4).
**/
VOID
EFIAPI
PinmuxConfigI2c (
  IN UINT32 I2cIndex
  );

/**
  Configure pins for SDMMC controller.

  @param[in] SdmmcIndex  SDMMC controller index (0=SDMMC1).
**/
VOID
EFIAPI
PinmuxConfigSdmmc (
  IN UINT32 SdmmcIndex
  );

#endif
