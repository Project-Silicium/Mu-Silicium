/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CMU_H_
#define _CMU_H_

#define CMU_TOP_BASE                       0x12060000
#define CMU_FSYS_BASE                      0x13400000

//
// eMMC (dwmmc0 / MMC_EMBD) Clocks
//
#define CLK_CON_MUX_CLKCMU_FSYS_MMC_EMBD   (CMU_TOP_BASE  + 0x1030)
#define CLK_CON_DIV_CLKCMU_FSYS_MMC_EMBD   (CMU_TOP_BASE  + 0x184C)
#define CLK_CON_GAT_CLKCMU_FSYS_MMC_EMBD   (CMU_TOP_BASE  + 0x204C)
#define PLL_CON0_MUX_FSYS_MMC_EMBD_USER    (CMU_FSYS_BASE + 0x0140)
#define CLK_CON_GAT_MMC_EMBD_I_ACLK        (CMU_FSYS_BASE + 0x2038)
#define CLK_CON_GAT_MMC_EMBD_SDCLKIN       (CMU_FSYS_BASE + 0x203C)
#define QCH_CON_MMC_EMBD_QCH               (CMU_FSYS_BASE + 0x301C)

//
// microSD (dwmmc2 / MMC_CARD) Clocks
//
#define CLK_CON_MUX_CLKCMU_FSYS_MMC_CARD   (CMU_TOP_BASE  + 0x102C)
#define CLK_CON_DIV_CLKCMU_FSYS_MMC_CARD   (CMU_TOP_BASE  + 0x1848)
#define CLK_CON_GAT_CLKCMU_FSYS_MMC_CARD   (CMU_TOP_BASE  + 0x2048)
#define PLL_CON0_MUX_FSYS_MMC_CARD_USER    (CMU_FSYS_BASE + 0x0120)
#define CLK_CON_GAT_MMC_CARD_I_ACLK        (CMU_FSYS_BASE + 0x2030)
#define CLK_CON_GAT_MMC_CARD_SDCLKIN       (CMU_FSYS_BASE + 0x2034)
#define QCH_CON_MMC_CARD_QCH               (CMU_FSYS_BASE + 0x3018)

#define CMU_MUX_SEL_MASK                   (0x1)
#define CMU_MUX_SEL_PLL_SHARED0_DIV2       (0x0)
#define CMU_MUX_BUSY                       BIT16

#define CMU_DIV_RATIO_MASK                 (0x1FF)
#define CMU_DIV_BUSY                       BIT16

#define CMU_GATE_MANUAL                    BIT20
#define CMU_GATE_CG_VAL                    BIT21
#define CMU_GATE_ON                        (CMU_GATE_MANUAL | CMU_GATE_CG_VAL)

#define CMU_USER_MUX_SEL                   BIT4
#define CMU_USER_MUX_BUSY                  BIT7

#define CMU_QCH_ENABLE                     BIT0
#define CMU_QCH_CLOCK_REQ                  BIT1

#define MSHC_CIU_CLOCK_PLL_HZ              (799500000)
#define MSHC_CCLKIN_MIN                    (25000000)

#define CMU_TIMEOUT_COUNT                  (10000)

#endif /* _CMU_H_ */
