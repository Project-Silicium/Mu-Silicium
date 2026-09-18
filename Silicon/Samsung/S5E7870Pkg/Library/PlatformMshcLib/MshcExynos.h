/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MSHC_EXYNOS_H_
#define _MSHC_EXYNOS_H_

#define MSHC_CLKSEL_DIV          (3)
#define MSHC_TIMING_SDR          CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 0, 4)
#define MSHC_TIMING_DDR          CLKSEL_TIMING (MSHC_CLKSEL_DIV, 0, 2, 4)

#endif /* _MSHC_EXYNOS_H_ */
