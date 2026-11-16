/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MSHC_EXYNOS_H_
#define _MSHC_EXYNOS_H_

#define MSHC_CLKSEL_DIV          (3)

//
// eMMC (dwmmc0) Timings
//
#define MSHC_EMMC_TIMING_SDR     CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 4, 0)
#define MSHC_EMMC_TIMING_DDR     CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 4, 2)

//
// microSD (dwmmc2) Timings
//
#define MSHC_SD_TIMING_SDR       CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 2, 0)
#define MSHC_SD_TIMING_DDR       CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 2, 1)
#define MSHC_SD_TIMING_SDR50     CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 4, 2)
#define MSHC_SD_TIMING_SDR104    CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 3, 0)

//
// Exynos extensions
//
#define MSHCI_FORCE_CLK_STOP     (0xB0)
#define MSHCI_BLOCK_DMA_FOR_CI   (0x1F8)

#define MMC_HWACG_CONTROL        BIT4
#define BLOCK_DMA_NO_EXT_TIMEOUT BIT29

#endif /* _MSHC_EXYNOS_H_ */
