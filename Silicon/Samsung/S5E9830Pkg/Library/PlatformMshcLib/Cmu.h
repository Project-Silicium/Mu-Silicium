/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CMU_H_
#define _CMU_H_

#define CMU_TOP_BASE                       0x1A330000
#define CMU_HSI1_BASE                      0x13000000

#define CLK_CON_MUX_CLKCMU_HSI1_MMC_CARD   (CMU_TOP_BASE  + 0x108C)
#define CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD   (CMU_TOP_BASE  + 0x1884)
#define CLK_CON_GAT_CLKCMU_HSI1_MMC_CARD   (CMU_TOP_BASE  + 0x2090)
#define PLL_CON0_MUX_HSI1_MMC_CARD_USER    (CMU_HSI1_BASE + 0x0610)
#define CLK_CON_GAT_MMC_CARD_I_ACLK        (CMU_HSI1_BASE + 0x201C)
#define CLK_CON_GAT_MMC_CARD_SDCLKIN       (CMU_HSI1_BASE + 0x2020)
#define QCH_CON_MMC_CARD_QCH               (CMU_HSI1_BASE + 0x3038)

#define CMU_MUX_SEL_MASK                   (0x3)
#define CMU_MUX_SEL_PLL_SHARED2            (0x1)
#define CMU_MUX_BUSY                       (0x1 << 16)

#define CMU_DIV_RATIO_MASK                 (0x1FF)
#define CMU_DIV_BUSY                       (0x1 << 16)

#define CMU_GATE_MANUAL                    (0x1 << 20)
#define CMU_GATE_CG_VAL                    (0x1 << 21)
#define CMU_GATE_ON                        (CMU_GATE_MANUAL | CMU_GATE_CG_VAL)

#define CMU_USER_MUX_SEL                   (0x1 << 4)
#define CMU_USER_MUX_BUSY                  (0x1 << 16)

#define CMU_QCH_ENABLE                     (0x1)
#define CMU_QCH_CLOCK_REQ                  (0x1 << 1)

#define MSHC_CIU_CLOCK_PLL_HZ              (800000000)
#define MSHC_CIU_CLOCK_OSC_HZ              (26000000)

#define MSHC_CCLKIN_MIN                    (25000000)

#define CMU_TIMEOUT_COUNT                  (10000)


#endif /* _CMU_H_ */
