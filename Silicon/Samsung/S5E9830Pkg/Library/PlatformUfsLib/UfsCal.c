#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/UfsCalAdapterLib.h>

#include "UfsCal.h"

static struct UfsCalParam  *ufs_cal[NUM_OF_UFS_HOST];
static unsigned long ufs_cal_lock_timeout = 0xFFFFFFFF;

static const struct UfsCalPhyCfg init_cfg_evt0[] = {
  {0x44,   0x00,        PMD_ALL, UNIPRO_DBG_PRD,        BRD_ALL},
  {0x200,  0x40,        PMD_ALL, PHY_PCS_COMN,           BRD_ALL},
  {0x12,   0x00,        PMD_ALL, PHY_PCS_RX_PRD_ROUND_OFF, BRD_ALL},
  {0xAA,   0x00,        PMD_ALL, PHY_PCS_TX_PRD_ROUND_OFF, BRD_ALL},
  {0xA9,   0x02,        PMD_ALL, PHY_PCS_TX,             BRD_ALL},
  {0xAB,   0x00,        PMD_ALL, PHY_PCS_TX_LR_PRD,      BRD_ALL},
  {0x11,   0x00,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x1B,   0x00,        PMD_ALL, PHY_PCS_RX_LR_PRD,      BRD_ALL},
  {0x2F,   0x79,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x76,   0x03,        PMD_ALL, PHY_PCS_RX,             BRD_ZEBU},
  {0x84,   0x01,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x04,   0x01,        PMD_ALL, PHY_PCS_TX,             BRD_ALL},
  {0x25,   0xF6,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x7F,   0x00,        PMD_ALL, PHY_PCS_TX,             BRD_ALL},
  {0x200,  0x00,        PMD_ALL, PHY_PCS_COMN,           BRD_ALL},
  {0x155E, 0x00,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x3000, 0x00,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x3001, 0x01,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x4021, 0x01,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x4020, 0x01,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0xA006, 0x80000000,  PMD_ALL, UNIPRO_DBG_MIB,         BRD_ALL},
  {0x00,   0x3E8,       PMD_ALL, COMMON_WAIT,             BRD_ALL},
  {0x10C,  0x10,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x118,  0x48,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x81C,  0x0C,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xB84,  0x40,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x974,  0x00,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x978,  0x3F,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x97C,  0xFF,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x990,  0x4E,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x9B8,  0x5E,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x9BC,  0x70,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xBB4,  0x25,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAB0,  0x13,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xACC,  0x05,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAD8,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xADC,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAE0,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAE4,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAE8,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAEC,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAF0,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAF4,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAF8,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xBCC,  0x80,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x28,   0x33,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x34,   0xB9,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x38,   0x0F,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x44,   0x01,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0xB0,   0x30,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x104,  0x20,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0xB90,  0x18,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x10C,  0x18,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x10C,  0x00,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0xCE0,  0x08,        PMD_ALL, PHY_EMB_CAL_WAIT,       BRD_ALL},
  {0xA006, 0x00,        PMD_ALL, UNIPRO_DBG_MIB,         BRD_ALL},
  {0, 0, 0, 0, 0},
};

static const struct UfsCalPhyCfg init_cfg_evt1[] = {
  {0x44,   0x00,        PMD_ALL, UNIPRO_DBG_PRD,        BRD_ALL},
  {0x200,  0x40,        PMD_ALL, PHY_PCS_COMN,           BRD_ALL},
  {0x12,   0x00,        PMD_ALL, PHY_PCS_RX_PRD_ROUND_OFF, BRD_ALL},
  {0xAA,   0x00,        PMD_ALL, PHY_PCS_TX_PRD_ROUND_OFF, BRD_ALL},
  {0xA9,   0x02,        PMD_ALL, PHY_PCS_TX,             BRD_ALL},
  {0xAB,   0x00,        PMD_ALL, PHY_PCS_TX_LR_PRD,      BRD_ALL},
  {0x11,   0x00,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x1B,   0x00,        PMD_ALL, PHY_PCS_RX_LR_PRD,      BRD_ALL},
  {0x2F,   0x79,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x76,   0x03,        PMD_ALL, PHY_PCS_RX,             BRD_ZEBU},
  {0x84,   0x01,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x04,   0x01,        PMD_ALL, PHY_PCS_TX,             BRD_ALL},
  {0x25,   0xF6,        PMD_ALL, PHY_PCS_RX,             BRD_ALL},
  {0x7F,   0x00,        PMD_ALL, PHY_PCS_TX,             BRD_ALL},
  {0x200,  0x00,        PMD_ALL, PHY_PCS_COMN,           BRD_ALL},
  {0x155E, 0x00,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x3000, 0x00,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x3001, 0x01,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x4021, 0x01,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x4020, 0x01,        PMD_ALL, UNIPRO_STD_MIB,         BRD_ALL},
  {0x10C,  0x10,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x118,  0x48,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x81C,  0x0C,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xB84,  0x40,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x974,  0x00,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x978,  0x3F,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x97C,  0xFF,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x990,  0x4E,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x9B8,  0x5E,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x9BC,  0x70,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xBB4,  0x25,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAB0,  0x13,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xACC,  0x05,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAD8,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xADC,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAE0,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAE4,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAE8,  0x10,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAEC,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAF0,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAF4,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xAF8,  0x08,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0xBCC,  0x80,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x28,   0x33,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x34,   0xB9,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x38,   0x0F,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x44,   0x01,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0xB0,   0x30,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x104,  0x20,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0xB90,  0x18,        PMD_ALL, PHY_PMA_TRSV,           BRD_ALL},
  {0x10C,  0x18,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0x10C,  0x00,        PMD_ALL, PHY_PMA_COMN,           BRD_ALL},
  {0xCE0,  0x08,        PMD_ALL, PHY_EMB_CAL_WAIT,       BRD_ALL},
  {0xA006, 0x00,        PMD_ALL, UNIPRO_DBG_MIB,         BRD_ALL},
  {0, 0, 0, 0, 0},
};

static const struct UfsCalPhyCfg init_cfg_card[] = {
  {0, 0, 0, 0, 0},
};

static const struct UfsCalPhyCfg post_init_cfg_evt0_g3[] = {
  {0x00, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0x0B, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_init_cfg_evt1_g3[] = {
  {0x00, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0x0B, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0, 0, 0, 0, 0},
};

static const struct UfsCalPhyCfg post_init_cfg_evt0_g4[] = {
  {0x00, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0x0B, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_init_cfg_evt1_g4[] = {
  {0x00, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0x0B, 0x00, PMD_ALL, UNIPRO_ADAPT_LENGTH, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_init_cfg_card[] = {
  {0, 0, 0, 0, 0},
};

static const struct UfsCalPhyCfg calib_of_pwm[] = {
  {0x00, 0x9E, PMD_PWM, UNIPRO_STD_MIB, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg calib_of_pwm_card[]  = { {0,0,0,0,0} };
static const struct UfsCalPhyCfg post_calib_of_pwm[] = {
  {0x00, 0x9E, PMD_PWM, UNIPRO_STD_MIB, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_calib_of_pwm_card[] = { {0,0,0,0,0} };

static const struct UfsCalPhyCfg calib_of_hs_rate_a[] = {
  {0x9D, 0x00, PMD_HS,  PHY_PCS_RX,     BRD_ALL},
  {0x9E, 0x00, PMD_HS,  PHY_PCS_RX,     BRD_ALL},
  {0x0B, 0x00, PMD_HS,  UNIPRO_STD_MIB, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg calib_of_hs_rate_a_card[] = { {0,0,0,0,0} };
static const struct UfsCalPhyCfg post_calib_of_hs_rate_a[] = {
  {0x00, 0x76, PMD_HS,  UNIPRO_STD_MIB, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_calib_of_hs_rate_a_card[] = { {0,0,0,0,0} };

static const struct UfsCalPhyCfg calib_of_hs_rate_b[] = {
  {0x9D, 0x00, PMD_HS,  PHY_PCS_RX,     BRD_ALL},
  {0x9E, 0x00, PMD_HS,  PHY_PCS_RX,     BRD_ALL},
  {0x0B, 0x00, PMD_HS,  UNIPRO_STD_MIB, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg calib_of_hs_rate_b_card[] = { {0,0,0,0,0} };
static const struct UfsCalPhyCfg post_calib_of_hs_rate_b[] = {
  {0x00, 0x76, PMD_HS,  UNIPRO_STD_MIB, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_calib_of_hs_rate_b_card[] = { {0,0,0,0,0} };

static const struct UfsCalPhyCfg post_h8_enter[] = {
  {0xCE0, 0x00, PMD_HS, PHY_PMA_TRSV, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg post_h8_enter_card[] = { {0,0,0,0,0} };
static const struct UfsCalPhyCfg pre_h8_exit[] = {
  {0xCE0, 0x08, PMD_HS, PHY_EMB_CDR_WAIT, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg pre_h8_exit_card[] = { {0,0,0,0,0} };

static const struct UfsCalPhyCfg lane1_sq_off[] = {
  {0x750, 0x02, PMD_ALL, PHY_PMA_TRSV_LANE1_SQ_OFF, BRD_ALL},
  {0, 0, 0, 0, 0},
};
static const struct UfsCalPhyCfg lane1_sq_off_card[] = { {0,0,0,0,0} };

static UfsCalError ufs_cal_wait_pll_lock (void *hba, UINT32 addr, UINT32 mask)
{
  UINT32 i;
  for (i = 0; i < 100; i++) {
    if ((ufs_lld_pma_read (hba, PHY_PMA_COMN_ADDR (addr)) & mask) == mask)
      return UFS_CAL_NO_ERROR;
    MicroSecondDelay(1);
  }
  DEBUG ((EFI_D_ERROR, "UFS CAL: PLL lock timeout\n"));
  return UFS_CAL_ERROR;
}

static UfsCalError ufs_cal_wait_cdr_lock (void *hba, UINT32 addr, UINT32 mask, int lane)
{
  UINT32 i;
  for (i = 0; i < 1000; i++) {
    if ((ufs_lld_pma_read (hba, PHY_PMA_TRSV_ADDR (addr, lane)) & mask) == mask)
      return UFS_CAL_NO_ERROR;
    MicroSecondDelay(1);
  }
  DEBUG ((EFI_D_ERROR, "UFS CAL: CDR lock timeout lane %d\n", lane));
  return UFS_CAL_ERROR;
}

static UfsCalError ufs_cal_wait_cdr_afc_check (void *hba, UINT32 addr, UINT32 mask, int lane)
{
  UINT32 i;
  for (i = 0; i < 1000; i++) {
    if ((ufs_lld_pma_read (hba, PHY_PMA_TRSV_ADDR (addr, lane)) & mask) == mask)
      return UFS_CAL_NO_ERROR;
    MicroSecondDelay(1);
  }
  return UFS_CAL_ERROR;
}

static UfsCalError ufs30_cal_wait_cdr_lock (void *hba, UINT32 addr, UINT32 mask, int lane)
{
  return ufs_cal_wait_cdr_lock (hba, addr, mask, lane);
}

static UfsCalError ufs30_cal_done_wait (void *hba, UINT32 addr, UINT32 mask, int lane)
{
  UINT32 i;
  for (i = 0; i < 500; i++) {
    if ((ufs_lld_pma_read (hba, PHY_PMA_TRSV_ADDR (addr, lane)) & mask) == 0)
      return UFS_CAL_NO_ERROR;
    MicroSecondDelay(1);
  }
  DEBUG ((EFI_D_ERROR, "UFS CAL: EMB CAL timeout lane %d\n", lane));
  return UFS_CAL_ERROR;
}

static UfsCalError ufs_cal_config_uic (
  struct UfsCalParam *p,
  const struct UfsCalPhyCfg *cfg,
  struct UicPwrMode *Pmd)
{
  void *hba = p->Host;
  UINT8 Lane;
  UINT8 NumLanes = p->AvailableLane;
  UINT32 PmdMask  = PMD_ALL;

  if (Pmd) {
    if (IS_PWR_MODE_HS (Pmd->Mode))
    {
      if (Pmd->HsSeries == PA_HS_MODE_A) {
        PmdMask = (1U << (__PMD_HS_G1_L1 + ((Pmd->Gear - 1) * 2) + (Pmd->Lane - 1)));
      } else {
        PmdMask = (1U << (__PMD_HS_G1_L1 + ((Pmd->Gear - 1) * 2) + (Pmd->Lane - 1)));
      }
      PmdMask = PMD_HS;
    }
    else {
      PmdMask = PMD_PWM;
    }
  }

  for_each_phy_cfg (cfg) {
    if (!(cfg->Board & p->Board))
      continue;

    for (Lane = 0; Lane < NumLanes; Lane++)
    {
      if (!(cfg->Flag & PmdMask) && Pmd)
        continue;

      switch (cfg->Layer)
      {
        case UNIPRO_STD_MIB:
        case UNIPRO_DBG_MIB:
            if (Lane == 0)
            ufs_lld_dme_set (hba, UIC_ARG_MIB (cfg->Address), cfg->Value);
            break;
        case UNIPRO_DBG_PRD:
            if (Lane == 0)
            ufs_lld_dme_set (hba, UIC_ARG_MIB (cfg->Address), UNIPRO_MCLK_PERIOD (p));
            break;
        case UNIPRO_ADAPT_LENGTH:
            if (Lane == 0)
            ufs_lld_dme_set (hba, UIC_ARG_MIB (cfg->Address), cfg->Value);
            break;
        case PHY_PCS_COMN:
            if (Lane == 0)
            ufs_lld_dme_set (hba, UIC_ARG_MIB (cfg->Address), cfg->Value);
            break;
        case PHY_PCS_RXTX:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, TX_LANE_0 + Lane), cfg->Value);
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, RX_LANE_0 + Lane), cfg->Value);
            break;
        case PHY_PCS_RX:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, RX_LANE_0 + Lane), cfg->Value);
            break;
        case PHY_PCS_TX:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, TX_LANE_0 + Lane), cfg->Value);
            break;
        case PHY_PCS_RX_PRD:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, RX_LANE_0 + Lane),
                            UNIPRO_MCLK_PERIOD (p));
            break;
        case PHY_PCS_TX_PRD:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, TX_LANE_0 + Lane),
                            UNIPRO_MCLK_PERIOD (p));
            break;
        case PHY_PCS_RX_PRD_ROUND_OFF:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, RX_LANE_0 + Lane),
                            UNIPRO_MCLK_PERIOD_ROUND_OFF (p));
            break;
        case PHY_PCS_TX_PRD_ROUND_OFF:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address, TX_LANE_0 + Lane),
                            UNIPRO_MCLK_PERIOD_ROUND_OFF (p));
            break;
        case PHY_PCS_RX_LR_PRD:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address,   RX_LANE_0+Lane), (PCS_RX_LINE_RESET_DETECT_PERIOD(p)>>16)&0xFF);
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address+1, RX_LANE_0+Lane), (PCS_RX_LINE_RESET_DETECT_PERIOD(p)>>8)&0xFF);
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address+2, RX_LANE_0+Lane), (PCS_RX_LINE_RESET_DETECT_PERIOD(p)>>0)&0xFF);
            break;
        case PHY_PCS_TX_LR_PRD:
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address,   TX_LANE_0+Lane), (PCS_TX_LINE_RESET_PERIOD(p)>>16)&0xFF);
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address+1, TX_LANE_0+Lane), (PCS_TX_LINE_RESET_PERIOD(p)>>8)&0xFF);
            ufs_lld_dme_set (hba, UIC_ARG_MIB_SEL (cfg->Address+2, TX_LANE_0+Lane), (PCS_TX_LINE_RESET_PERIOD(p)>>0)&0xFF);
            break;
        case PHY_PMA_COMN:
            if (Lane == 0)
            ufs_lld_pma_write (hba, cfg->Value, PHY_PMA_COMN_ADDR (cfg->Address));
            break;
        case PHY_PMA_TRSV:
            ufs_lld_pma_write (hba, cfg->Value, PHY_PMA_TRSV_ADDR (cfg->Address, Lane));
            break;
        case PHY_PMA_TRSV_LANE1_SQ_OFF:
            if (Lane == 1) {
            if (p->ConnectedRxLane < p->AvailableLane)
                ufs_lld_pma_write (hba, cfg->Value, PHY_PMA_TRSV_ADDR (cfg->Address, Lane));
            }
            break;
        case PHY_PMA_TRSV_SQ:
            if (Lane < p->ConnectedRxLane)
            ufs_lld_pma_write (hba, cfg->Value, PHY_PMA_TRSV_ADDR (cfg->Address, Lane));
            break;
        case UNIPRO_DBG_APB:
            if (Lane == 0)
            ufs_lld_unipro_write (hba, cfg->Value, cfg->Address);
            break;
        case PHY_PLL_WAIT:
            if (Lane == 0) {
            if (ufs_cal_wait_pll_lock (hba, cfg->Address, cfg->Value) == UFS_CAL_ERROR)
                return UFS_CAL_TIMEOUT;
            }
            break;
        case PHY_CDR_WAIT:
            if (Lane < p->ActiveRxLane) {
            if (ufs_cal_wait_cdr_lock (hba, cfg->Address, cfg->Value, Lane) == UFS_CAL_ERROR)
                return UFS_CAL_TIMEOUT;
            }
            break;
        case PHY_EMB_CDR_WAIT:
            if (Lane < p->ActiveRxLane) {
            if (ufs30_cal_wait_cdr_lock (hba, cfg->Address, cfg->Value, Lane) == UFS_CAL_ERROR)
                return UFS_CAL_TIMEOUT;
            }
            break;
        case PHY_CDR_AFC_WAIT:
            if (Lane < p->ActiveRxLane) {
            if (p->Table == HOST_CARD) {
                if (ufs_cal_wait_cdr_afc_check (hba, cfg->Address, cfg->Value, Lane) == UFS_CAL_ERROR)
                return UFS_CAL_TIMEOUT;
            }
            }
            break;
        case COMMON_WAIT:
            if (Lane == 0)
            MicroSecondDelay(cfg->Value);
            break;
        case PHY_EMB_CAL_WAIT:
            if (ufs30_cal_done_wait (hba, cfg->Address, cfg->Value, Lane) == UFS_CAL_ERROR)
            return UFS_CAL_TIMEOUT;
            break;
        default:
            break;
      }
    }
  }
  return UFS_CAL_NO_ERROR;
}

/*
 * This is a recommendation from Samsung UFS device vendor.
 *
 * Activate time: host < device
 * Hibern time: host > device
 */
static void ufs_cal_calib_hibern8_values (void *hba)
{
  UINT32 hw_cap_min_tactivate;
  UINT32 peer_rx_min_actv_time_cap;
  UINT32 max_rx_hibern8_time_cap;

  ufs_lld_dme_get (hba, UIC_ARG_MIB_SEL (0x8F, RX_LANE_0), &hw_cap_min_tactivate);
  ufs_lld_dme_get (hba, UIC_ARG_MIB (0x15A8), &peer_rx_min_actv_time_cap);
  ufs_lld_dme_get (hba, UIC_ARG_MIB (0x15A7), &max_rx_hibern8_time_cap);

  if (peer_rx_min_actv_time_cap >= hw_cap_min_tactivate)
    ufs_lld_dme_peer_set (hba, UIC_ARG_MIB (0x15A8), peer_rx_min_actv_time_cap + 1);
  ufs_lld_dme_set (hba, UIC_ARG_MIB (0x15A7), max_rx_hibern8_time_cap + 1);
}

UfsCalError ufs_cal_post_h8_enter (struct UfsCalParam *p)
{
  const struct UfsCalPhyCfg *cfg;
  cfg = (p->Table == HOST_CARD) ? post_h8_enter_card : post_h8_enter;
  return ufs_cal_config_uic (p, cfg, p->Pmd);
}

UfsCalError ufs_cal_pre_h8_exit (struct UfsCalParam *p)
{
  const struct UfsCalPhyCfg *cfg;
  cfg = (p->Table == HOST_CARD) ? pre_h8_exit_card : pre_h8_exit;
  return ufs_cal_config_uic (p, cfg, p->Pmd);
}

UfsCalError UfsCalPrePmc (struct UfsCalParam *p)
{
  const struct UfsCalPhyCfg *cfg;

  if ((p->Pmd->Mode == SLOW_MODE) || (p->Pmd->Mode == SLOWAUTO_MODE))
    cfg = (p->Table == HOST_CARD) ? calib_of_pwm_card : calib_of_pwm;
  else if (p->Pmd->HsSeries == PA_HS_MODE_B)
    cfg = (p->Table == HOST_CARD) ? calib_of_hs_rate_b_card : calib_of_hs_rate_b;
  else if (p->Pmd->HsSeries == PA_HS_MODE_A)
    cfg = (p->Table == HOST_CARD) ? calib_of_hs_rate_a_card : calib_of_hs_rate_a;
  else
    return UFS_CAL_INV_ARG;

  return ufs_cal_config_uic (p, cfg, p->Pmd);
}

UfsCalError UfsCalPostPmc (struct UfsCalParam *p)
{
  const struct UfsCalPhyCfg *cfg;

  if ((p->Pmd->Mode == SLOWAUTO_MODE) || (p->Pmd->Mode == SLOW_MODE))
    cfg = (p->Table == HOST_CARD) ? post_calib_of_pwm_card : post_calib_of_pwm;
  else if (p->Pmd->HsSeries == PA_HS_MODE_B)
    cfg = (p->Table == HOST_CARD) ? post_calib_of_hs_rate_b_card : post_calib_of_hs_rate_b;
  else if (p->Pmd->HsSeries == PA_HS_MODE_A)
    cfg = (p->Table == HOST_CARD) ? post_calib_of_hs_rate_a_card : post_calib_of_hs_rate_a;
  else
    return UFS_CAL_INV_ARG;

  return ufs_cal_config_uic (p, cfg, p->Pmd);
}

UfsCalError UfsCalPostLink (struct UfsCalParam *p)
{
  UfsCalError                 ret = UFS_CAL_NO_ERROR;
  const struct UfsCalPhyCfg *cfg;

  ufs_cal_calib_hibern8_values (p->Host);

  switch (p->MaxGear) {
  case GEAR_1:
  case GEAR_2:
  case GEAR_3:
    cfg = (p->EvtVer == 0) ? post_init_cfg_evt0_g3 : post_init_cfg_evt1_g3;
    if (p->Table == HOST_CARD) cfg = post_init_cfg_card;
    break;
  case GEAR_4:
    cfg = (p->EvtVer == 0) ? post_init_cfg_evt0_g4 : post_init_cfg_evt1_g4;
    if (p->Table == HOST_CARD) cfg = post_init_cfg_card;
    break;
  default:
    return UFS_CAL_INV_ARG;
  }

  ret = ufs_cal_config_uic (p, cfg, NULL);

  if (ret == UFS_CAL_NO_ERROR) {
    if ((p->AvailableLane == 2) && (p->ConnectedRxLane == 1)) {
      cfg = (p->Table == HOST_CARD) ? lane1_sq_off_card : lane1_sq_off;
      ret = ufs_cal_config_uic (p, cfg, NULL);
    }
  }

  return ret;
}

UfsCalError UfsCalPreLink (struct UfsCalParam *p)
{
  const struct UfsCalPhyCfg *cfg;

  if (p->Table == HOST_CARD) {
    cfg = init_cfg_card;
  } else {
    cfg = (p->EvtVer == 0) ? init_cfg_evt0 : init_cfg_evt1;
  }

  return ufs_cal_config_uic (p, cfg, NULL);
}

UINT8 UfsCalGetTargetBoard (VOID)
{
  return BRD_UNIV;
}

UfsCalError UfsCalInit (struct UfsCalParam *p)
{
  ufs_cal[0] = p;
  ufs_cal_lock_timeout = 1000;

  return UFS_CAL_NO_ERROR;
}