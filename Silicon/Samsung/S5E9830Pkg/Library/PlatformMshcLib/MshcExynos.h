/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MSHC_EXYNOS_H_
#define _MSHC_EXYNOS_H_

#define MSHC_CLKSEL_DIV          (3)
#define MSHC_TIMING_SDR          CLKSEL_TIMING (3, 0, 2, 0)
#define MSHC_TIMING_DDR          CLKSEL_TIMING (3, 0, 2, 1)
#define MSHC_TIMING_SDR50        CLKSEL_TIMING (3, 0, 4, 2)
#define MSHC_TIMING_SDR104       CLKSEL_TIMING (3, 0, 3, 0)

//
// Exynos extensions
//
#define MSHCI_FORCE_CLK_STOP               (0xB0)
#define MSHCI_AXI_BURST_LENGTH             (0xB4)
#define MSHCI_BLOCK_DMA_FOR_CI             (0x1F8)

#define MMC_HWACG_CONTROL                  (0x1 << 4)

#define BLOCK_DMA_NO_EXT_TIMEOUT           (0x1 << 29)

#endif /* _MSHC_EXYNOS_H_ */