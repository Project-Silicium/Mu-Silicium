#include <Library/UfsHostBridge.h>
#include <Library/UfsCalAdapterLib.h>

#ifndef __UFS_CAL_H__
#define __UFS_CAL_H__

#define IS_PWR_MODE_HS(m) (((m) == FAST_MODE) || ((m) == FASTAUTO_MODE))
#define IS_PWR_MODE_PWM(m) (((m) == SLOW_MODE) || ((m) == SLOWAUTO_MODE))

#define UNIPRO_MCLK_PERIOD(p) (1000000000UL / (p)->MclkRate)

#define UNIPRO_MCLK_PERIOD_ROUND_OFF(p) (UINT32)(((2000000000ULL / (p)->MclkRate) + 1) / 2)
#define UNIPRO18_MCLK_PERIOD(p) (16ULL * 1000ULL * 1000000ULL / (p)->MclkRate)

#define TX_LINE_RESET_TIME 3200UL
#define RX_LINE_RESET_DETECT_TIME 1000UL
#define PCS_TX_LINE_RESET_PERIOD(p) (UINT32)((TX_LINE_RESET_TIME * ((UINT64)(p)->MclkRate / 1000000ULL)))
#define PCS_RX_LINE_RESET_DETECT_PERIOD(p) (UINT32)((RX_LINE_RESET_DETECT_TIME * ((UINT64)(p)->MclkRate / 1000000ULL)))

#define PHY_PMA_COMN_ADDR(reg) (reg)
#define PHY_PMA_TRSV_ADDR(reg,lane) ((reg) + (0x800 * (lane)))

#define PMD_PWM_G1_L1 (1U << __PMD_PWM_G1_L1)
#define PMD_PWM_G1_L2 (1U << __PMD_PWM_G1_L2)
#define PMD_PWM_G2_L1 (1U << __PMD_PWM_G2_L1)
#define PMD_PWM_G2_L2 (1U << __PMD_PWM_G2_L2)
#define PMD_PWM_G3_L1 (1U << __PMD_PWM_G3_L1)
#define PMD_PWM_G3_L2 (1U << __PMD_PWM_G3_L2)
#define PMD_PWM_G4_L1 (1U << __PMD_PWM_G4_L1)
#define PMD_PWM_G4_L2 (1U << __PMD_PWM_G4_L2)
#define PMD_PWM_G5_L1 (1U << __PMD_PWM_G5_L1)
#define PMD_PWM_G5_L2 (1U << __PMD_PWM_G5_L2)
#define PMD_PWM_MAX (1U << __PMD_PWM_MAX)
#define PMD_HS_G1_L1 (1U << __PMD_HS_G1_L1)
#define PMD_HS_G1_L2 (1U << __PMD_HS_G1_L2)
#define PMD_HS_G2_L1 (1U << __PMD_HS_G2_L1)
#define PMD_HS_G2_L2 (1U << __PMD_HS_G2_L2)
#define PMD_HS_G3_L1 (1U << __PMD_HS_G3_L1)
#define PMD_HS_G3_L2 (1U << __PMD_HS_G3_L2)
#define PMD_HS_G4_L1 (1U << __PMD_HS_G4_L1)
#define PMD_HS_G4_L2 (1U << __PMD_HS_G4_L2)
#define PMD_HS_MAX (1U << __PMD_HS_MAX)
#define PMD_ALL (PMD_HS_MAX - 1)
#define PMD_PWM (PMD_PWM_MAX - 1)
#define PMD_HS (PMD_ALL ^ PMD_PWM)

#define BRD_SMDK (1U << __BRD_SMDK)
#define BRD_ASB	 (1U << __BRD_ASB)
#define BRD_HSIE (1U << __BRD_HSIE)
#define BRD_ZEBU (1U << __BRD_ZEBU)
#define BRD_UNIV (1U << __BRD_UNIV)
#define BRD_MAX (1U << __BRD_MAX)
#define BRD_ALL ((1U << __BRD_MAX) - 1)

#define for_each_phy_cfg(cfg)  for (; (cfg)->Flag != PHY_CFG_NONE; (cfg)++)

#define NUM_OF_UFS_HOST  2

enum {
    PA_HS_MODE_A = 1,
    PA_HS_MODE_B = 2
};

enum {
    FAST_MODE = 1,
    SLOW_MODE = 2,
    FASTAUTO_MODE = 4,
    SLOWAUTO_MODE = 5,
    UNCHANGED = 7
};

enum {
  PHY_CFG_NONE = 0,
  PHY_PCS_COMN,
  PHY_PCS_RXTX,
  PHY_PMA_COMN,
  PHY_PMA_TRSV,
  PHY_PLL_WAIT,
  PHY_CDR_WAIT,
  PHY_CDR_AFC_WAIT,
  UNIPRO_STD_MIB,
  UNIPRO_DBG_MIB,
  UNIPRO_DBG_APB,
  PHY_PCS_RX,
  PHY_PCS_TX,
  PHY_PCS_RX_PRD,
  PHY_PCS_TX_PRD,
  UNIPRO_DBG_PRD,
  PHY_PMA_TRSV_LANE1_SQ_OFF,
  PHY_PMA_TRSV_SQ,
  COMMON_WAIT,
  PHY_PCS_RX_LR_PRD,
  PHY_PCS_TX_LR_PRD,
  PHY_PCS_RX_PRD_ROUND_OFF,
  PHY_PCS_TX_PRD_ROUND_OFF,
  UNIPRO_ADAPT_LENGTH,
  PHY_EMB_CDR_WAIT,
  PHY_EMB_CAL_WAIT,
};

enum {
  __PMD_PWM_G1_L1,
  __PMD_PWM_G1_L2,
  __PMD_PWM_G2_L1,
  __PMD_PWM_G2_L2,
  __PMD_PWM_G3_L1,
  __PMD_PWM_G3_L2,
  __PMD_PWM_G4_L1,
  __PMD_PWM_G4_L2,
  __PMD_PWM_G5_L1,
  __PMD_PWM_G5_L2,
  __PMD_PWM_MAX,

  __PMD_HS_G1_L1,
  __PMD_HS_G1_L2,
  __PMD_HS_G2_L1,
  __PMD_HS_G2_L2,
  __PMD_HS_G3_L1,
  __PMD_HS_G3_L2,
  __PMD_HS_G4_L1,
  __PMD_HS_G4_L2,
  __PMD_HS_MAX,
};

enum {
	__BRD_SMDK,
	__BRD_ASB,
	__BRD_HSIE,
	__BRD_ZEBU,
	__BRD_UNIV,
	__BRD_MAX,
};

struct UfsCalPhyCfg {
  UINT32 Address;
  UINT32 Value;
  UINT32 Flag;
  UINT32 Layer;
  UINT8  Board;
};

UfsCalError UfsCalInit (struct UfsCalParam *p);
UfsCalError UfsCalPreLink (struct UfsCalParam *p);
UfsCalError UfsCalPostLink (struct UfsCalParam *p);
UfsCalError UfsCalPrePmc (struct UfsCalParam *p);
UfsCalError UfsCalPostPmc (struct UfsCalParam *p);
#endif /* __UFS_CAL_H__ */