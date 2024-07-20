/**
  Copyright (C) 2022 Svyatoslav Ryhel <clamor95@gmail.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _TEGRA_TIMER_H_
#define _TEGRA_TIMER_H_

// Tegra Timer Base Address
#define TIMER_TMR1_TMR_PTV_0       0x60005000
#define TIMER_TMR1_TMR_PCR_0      (TIMER_TMR1_TMR_PTV_0 + 0x4)

// Tegra Timer Registers
#define TEGRA_TIMER_USEC_CNTR      0x60005010
#define TEGRA_TIMER_USEC_CFG      (0x60005010 + 0x4)

#endif /* _TEGRA_TIMER_H_ */
