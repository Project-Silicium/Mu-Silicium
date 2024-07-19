/**
  Copyright (C) 2013-2015 NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _TEGRA_GPIO_DXE_H_
#define _TEGRA_GPIO_DXE_H_

//
// The Tegra GPIO Controller has 256 GPIOs in 8 Banks of 4 Ports,
// each with 8 GPIOs.
//
#define TEGRA_GPIO_PORTS   4
#define TEGRA_GPIO_BANKS   8

#define GPIO_BANK(x)      ((x) >> 5)
#define GPIO_PORT(x)      (((x) >> 3) & 0x3)
#define GPIO_FULLPORT(x)  ((x) >> 3)
#define GPIO_BIT(x)       ((x) & 0x7)

// GPIO Controller Registers for a Single Bank
typedef struct gpio_ctlr_bank {
  UINT32 GpioConfig[TEGRA_GPIO_PORTS];
  UINT32 GpioDirection[TEGRA_GPIO_PORTS];
  UINT32 GpioOut[TEGRA_GPIO_PORTS];
  UINT32 GpioIn[TEGRA_GPIO_PORTS];
  UINT32 GpioIntStatus[TEGRA_GPIO_PORTS];
  UINT32 GpioIntEnable[TEGRA_GPIO_PORTS];
  UINT32 GpioIntLevel[TEGRA_GPIO_PORTS];
  UINT32 GpioIntClear[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedConfig[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedDirection[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedOut[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedIn[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedIntStatus[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedIntEnable[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedIntLevel[TEGRA_GPIO_PORTS];
  UINT32 GpioMaskedIntClear[TEGRA_GPIO_PORTS];
} GpioCtrlBank;

typedef struct gpio_ctlr {
  GpioCtrlBank GpioBank[TEGRA_GPIO_BANKS];
} GpioCtrl;

#endif /* _TEGRA_GPIO_DXE_H_ */
