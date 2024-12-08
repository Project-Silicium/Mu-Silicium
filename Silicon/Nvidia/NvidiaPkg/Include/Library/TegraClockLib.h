/**
  Copyright (c) 2011 The Chromium OS Authors.
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _TEGRA_CLOCK_LIB_H_
#define _TEGRA_CLOCK_LIB_H_

#include <Device/TegraClock.h>

#define CLOCK_ID_PLL_COUNT (CLOCK_ID_COUNT - 3)

typedef struct clk_pll_info {
  UINT32 m_shift;
  UINT32 n_shift;
  UINT32 p_shift;
  UINT32 kcp_shift;
  UINT32 kvco_shift;
  UINT32 lock_ena;
  UINT32 rsvd;
  UINT32 m_mask;
  UINT32 n_mask;
  UINT32 p_mask;
  UINT32 kcp_mask;
  UINT32 kvco_mask;
  UINT32 lock_det;
  UINT32 rsvd2;
} PllClockInfo;

typedef enum clock_osc_freq {
  CLOCK_OSC_FREQ_13_0 = 0,
  CLOCK_OSC_FREQ_16_8,
  CLOCK_OSC_FREQ_19_2 = 4,
  CLOCK_OSC_FREQ_38_4,
  CLOCK_OSC_FREQ_12_0 = 8,
  CLOCK_OSC_FREQ_48_0,
  CLOCK_OSC_FREQ_26_0 = 12,

  CLOCK_OSC_FREQ_COUNT,
} OscClockFreqs;

PllClockInfo*
GetPllInfoTable (IN UINT8 ClockID);

SimplePllClock*
GetSimplePll (
  IN UINT8                ClockID,
  IN EFI_PHYSICAL_ADDRESS ClockControllerBase
  );

OscClockFreqs
GetOscClockFreq (IN EFI_PHYSICAL_ADDRESS ClockControllerBase);

UINT32
GetClkmRate (
  IN UINT32               ParentRate,
  IN EFI_PHYSICAL_ADDRESS ClockControllerBase
  );

#endif /* _TEGRA_CLOCK_LIB_H_ */