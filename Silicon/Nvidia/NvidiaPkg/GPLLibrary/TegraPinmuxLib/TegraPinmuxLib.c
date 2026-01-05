/**
  TegraPinmuxLib.c - Tegra Pinmux Configuration Library

  Copyright (c) 2018 naehrwert
  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#include <Library/TegraPinmuxLib.h>
#include <Library/IoLib.h>
#include <T210.h>

VOID
EFIAPI
PinmuxSetConfig (
  IN UINT32 PinIndex,
  IN UINT32 Config
  )
{
  MmioWrite32 (PINMUX_AUX_BASE + (PinIndex * 4), Config);
}

UINT32
EFIAPI
PinmuxGetConfig (
  IN UINT32 PinIndex
  )
{
  return MmioRead32 (PINMUX_AUX_BASE + (PinIndex * 4));
}

VOID
EFIAPI
PinmuxConfigI2c (
  IN UINT32 I2cIndex
  )
{
  UINT32 PinSda = 0;
  UINT32 PinScl = 0;

  switch (I2cIndex) {
  case PINMUX_I2C1:
    PinSda = PINMUX_GEN1_I2C_SDA_INDEX;
    PinScl = PINMUX_GEN1_I2C_SCL_INDEX;
    break;
  case PINMUX_I2C2:
    PinSda = PINMUX_GEN2_I2C_SDA_INDEX;
    PinScl = PINMUX_GEN2_I2C_SCL_INDEX;
    break;
  case PINMUX_I2C3:
    PinSda = PINMUX_GEN3_I2C_SDA_INDEX;
    PinScl = PINMUX_GEN3_I2C_SCL_INDEX;
    break;
  case PINMUX_I2C4:
    PinSda = PINMUX_CAM_I2C_SDA_INDEX;
    PinScl = PINMUX_CAM_I2C_SCL_INDEX;
    break;
  case PINMUX_I2C5:
    PinSda = PINMUX_PWR_I2C_SDA_INDEX;
    PinScl = PINMUX_PWR_I2C_SCL_INDEX;
    break;
  default:
    return;
  }

  PinmuxSetConfig (PinSda, PINMUX_INPUT_ENABLE);
  PinmuxSetConfig (PinScl, PINMUX_INPUT_ENABLE);
}

VOID
EFIAPI
PinmuxConfigSdmmc (
  IN UINT32 SdmmcIndex
  )
{
  UINT32 BaseConfig;
  UINT32 DataConfig;

  if (SdmmcIndex != 0) {
    return;
  }

  // SDMMC1: Drive 2X, Input Enable
  BaseConfig = PINMUX_DRIVE_2X | PINMUX_INPUT_ENABLE;

  // CLK: No pull
  PinmuxSetConfig (PINMUX_SDMMC1_CLK_INDEX, PINMUX_FUNC_DEFAULT | BaseConfig | PINMUX_PULL_NONE);

  // CMD/DAT: Pull up
  DataConfig = BaseConfig | PINMUX_PULL_UP;
  PinmuxSetConfig (PINMUX_SDMMC1_CMD_INDEX, PINMUX_FUNC_DEFAULT | DataConfig);
  PinmuxSetConfig (PINMUX_SDMMC1_DAT0_INDEX, PINMUX_FUNC_DEFAULT | DataConfig);
  PinmuxSetConfig (PINMUX_SDMMC1_DAT1_INDEX, PINMUX_FUNC_DEFAULT | DataConfig);
  PinmuxSetConfig (PINMUX_SDMMC1_DAT2_INDEX, PINMUX_FUNC_DEFAULT | DataConfig);
  PinmuxSetConfig (PINMUX_SDMMC1_DAT3_INDEX, PINMUX_FUNC_DEFAULT | DataConfig);

  // Card detect
  PinmuxSetConfig (PINMUX_GPIO_PZ1_INDEX, PINMUX_FUNC_DEFAULT | PINMUX_PULL_UP | PINMUX_INPUT_ENABLE);
}
