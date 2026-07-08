#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/UfsCalAdapterLib.h>

#include "UfsCal.h"

static struct UfsCalParam *ufs_cal[NUM_OF_UFS_HOST];
static unsigned long ufs_cal_lock_timeout = 0xFFFFFFFF;

static const struct UfsCalPhyCfg init_cfg[] = {
	{0x9514, 0x00, PMD_ALL, UNIPRO_DBG_PRD, 0},
	{0x200, 0x40, PMD_ALL, PHY_PCS_COMN, 0},
	{0x12, 0x00, PMD_ALL, PHY_PCS_RX_PRD, 0},
	{0xAA, 0x00, PMD_ALL, PHY_PCS_TX_PRD, 0},
	{0x5C, 0x38, PMD_ALL, PHY_PCS_RX, 0},
	{0x0F, 0x0, PMD_ALL, PHY_PCS_RX, 0},
	{0x65, 0x01, PMD_ALL, PHY_PCS_RX, 0},
	{0x69, 0x01, PMD_ALL, PHY_PCS_RX, 0},
	{0x21, 0x00, PMD_ALL, PHY_PCS_RX, 0},
	{0x22, 0x00, PMD_ALL, PHY_PCS_RX, 0},
	{0x84, 0x01, PMD_ALL, PHY_PCS_RX, 0},
	{0x04, 0x01, PMD_ALL, PHY_PCS_TX, 0},
	{0x8F, 0x3E, PMD_ALL, PHY_PCS_TX, 0},
	{0x200, 0x0, PMD_ALL, PHY_PCS_COMN, 0},
	{0x9536, 0x4E20, PMD_ALL, UNIPRO_DBG_MIB, 0},
	{0x9564, 0x2e820183, PMD_ALL, UNIPRO_DBG_MIB, 0},
	{0x155E, 0x0, PMD_ALL, UNIPRO_STD_MIB, 0},
	{0x3000, 0x0, PMD_ALL, UNIPRO_STD_MIB, 0},
	{0x3001, 0x1, PMD_ALL, UNIPRO_STD_MIB, 0},
	{0x4021, 0x1, PMD_ALL, UNIPRO_STD_MIB, 0},
	{0x4020, 0x1, PMD_ALL, UNIPRO_STD_MIB, 0},
	{0x8C, 0x80, PMD_ALL, PHY_PMA_COMN, 0},
	{0x74, 0x10, PMD_ALL, PHY_PMA_COMN, 0},
	{0x110, 0xB5, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x134, 0x43, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x16C, 0x20, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x178, 0xC0, PMD_ALL, PHY_PMA_TRSV, 0},
	{0xE0, 0x12, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x164, 0x58, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x1B0, 0x18, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x8C, 0xC0, PMD_ALL, PHY_PMA_COMN, 0},
	{0x8C, 0x00, PMD_ALL, PHY_PMA_COMN, 0},
	{0x00, 0xC8, PMD_ALL, COMMON_WAIT, 0},
	{},
};

static struct UfsCalPhyCfg post_init_cfg[] = {
	{0x9529, 0x01, PMD_ALL, UNIPRO_DBG_MIB, 0},
	{0x15A4, 0xFA, PMD_ALL, UNIPRO_STD_MIB, 0},
	{0x9529, 0x00, PMD_ALL, UNIPRO_DBG_MIB, 0},
	{0x200, 0x40, PMD_ALL, PHY_PCS_COMN, 0},
	{0x35, 0x05, PMD_ALL, PHY_PCS_RX, 0},
	{0x73, 0x01, PMD_ALL, PHY_PCS_RX, 0},
	{0x41, 0x02, PMD_ALL, PHY_PCS_RX, 0},
	{0x42, 0xAC, PMD_ALL, PHY_PCS_RX, 0},
	{0x200, 0x00, PMD_ALL, PHY_PCS_COMN, 0},
	{},
};

static struct UfsCalPhyCfg calib_of_pwm[] = {
	{0x2041, 8064, PMD_PWM, UNIPRO_STD_MIB, 0},
	{0x2042, 28224, PMD_PWM, UNIPRO_STD_MIB, 0},
	{0x2043, 20160, PMD_PWM, UNIPRO_STD_MIB, 0},
	{0x15B0, 12000, PMD_PWM, UNIPRO_STD_MIB, 0},
	{0x15B1, 32000, PMD_PWM, UNIPRO_STD_MIB, 0},
	{0x15B2, 16000, PMD_PWM, UNIPRO_STD_MIB, 0},

	{0x7888, 8064, PMD_PWM, UNIPRO_DBG_APB, 0},
	{0x788C, 28224, PMD_PWM, UNIPRO_DBG_APB, 0},
	{0x7890, 20160, PMD_PWM, UNIPRO_DBG_APB, 0},
	{0x78B8, 12000, PMD_PWM, UNIPRO_DBG_APB, 0},
	{0x78BC, 32000, PMD_PWM, UNIPRO_DBG_APB, 0},
	{0x78C0, 16000, PMD_PWM, UNIPRO_DBG_APB, 0},

	//MPHY tuning value
	{0xC8, 0x40, PMD_PWM, PHY_PMA_TRSV, 0},
	{0xF0, 0x77, PMD_PWM, PHY_PMA_TRSV, 0},
	{0x120, 0x80, PMD_PWM, PHY_PMA_TRSV, 0},
	{0x128, 0x00, PMD_PWM, PHY_PMA_TRSV, 0},
	{0x12C, 0x00, PMD_PWM, PHY_PMA_TRSV, 0},
	{0x134, 0x43, PMD_PWM, PHY_PMA_TRSV, 0},
	{},
};

static struct UfsCalPhyCfg post_calib_of_pwm[] = {
	{},
};
static struct UfsCalPhyCfg calib_of_hs_rate_a[] = {
	{0x2041, 8064, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x2042, 28224, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x2043, 20160, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x15B0, 12000, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x15B1, 32000, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x15B2, 16000, PMD_HS, UNIPRO_STD_MIB, 0},

	{0x7888, 8064, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x788C, 28224, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x7890, 20160, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x78B8, 12000, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x78BC, 32000, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x78C0, 16000, PMD_HS, UNIPRO_DBG_APB, 0},

	//MPHY tuning value
	{0xC8, 0xBC, PMD_HS, PHY_PMA_TRSV, 0},
	{0xF0, 0x7F, PMD_HS, PHY_PMA_TRSV, 0},
	{0x120, 0xC0, PMD_HS, PHY_PMA_TRSV, 0},
	{0x128, 0x08, PMD_HS_G1_L2, PHY_PMA_TRSV, 0},
	{0x128, 0x02, PMD_HS_G2_L2, PHY_PMA_TRSV, 0},
	{0x128, 0x00, PMD_HS_G3_L2, PHY_PMA_TRSV, 0},
	{0x12C, 0x00, PMD_HS_G2_L2, PHY_PMA_TRSV, 0},
	{0x134, 0xd3, PMD_HS_G1_L2, PHY_PMA_TRSV, 0},
	{0x134, 0x73, PMD_HS_G2_L2, PHY_PMA_TRSV, 0},
	{0x134, 0x63, PMD_HS_G3_L2, PHY_PMA_TRSV, 0},
	{},
};

static struct UfsCalPhyCfg post_calib_of_hs_rate_a[] = {
	{0x1fc, 0x40, PMD_HS, PHY_CDR_AFC_WAIT, 0},
	{},
};

static struct UfsCalPhyCfg calib_of_hs_rate_b[] = {
	{0x2041, 8064, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x2042, 28224, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x2043, 20160, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x15B0, 12000, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x15B1, 32000, PMD_HS, UNIPRO_STD_MIB, 0},
	{0x15B2, 16000, PMD_HS, UNIPRO_STD_MIB, 0},

	{0x7888, 8064, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x788C, 28224, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x7890, 20160, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x78B8, 12000, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x78BC, 32000, PMD_HS, UNIPRO_DBG_APB, 0},
	{0x78C0, 16000, PMD_HS, UNIPRO_DBG_APB, 0},
	/*MPHY tuning value*/
	{0xC8, 0xBC, PMD_HS, PHY_PMA_TRSV, 0},
	{0xF0, 0x7F, PMD_HS, PHY_PMA_TRSV, 0},
	{0x120, 0xC0, PMD_HS, PHY_PMA_TRSV, 0},
	{0x128, 0x08, PMD_HS_G1_L2, PHY_PMA_TRSV, 0},
	{0x128, 0x02, PMD_HS_G2_L2, PHY_PMA_TRSV, 0},
	{0x128, 0x00, PMD_HS_G3_L2, PHY_PMA_TRSV, 0},
	{0x12C, 0x00, PMD_HS_G2_L2, PHY_PMA_TRSV, 0},
	{0x134, 0xd3, PMD_HS_G1_L2, PHY_PMA_TRSV, 0},
	{0x134, 0x73, PMD_HS_G2_L2, PHY_PMA_TRSV, 0},
	{0x134, 0x63, PMD_HS_G3_L2, PHY_PMA_TRSV, 0},
	{},
};

static struct UfsCalPhyCfg post_calib_of_hs_rate_b[] = {
	{0x1fc, 0x40, PMD_HS, PHY_CDR_AFC_WAIT, 0},
	{},
};

/*
static struct UfsCalPhyCfg lane1_sq_off[] = {
	{0x0C4, 0x19, PMD_ALL, PHY_PMA_TRSV_LANE1_SQ_OFF, 0},
	{0x0E8, 0xFF, PMD_ALL, PHY_PMA_TRSV_LANE1_SQ_OFF, 0},
	{},
};
*/
static struct UfsCalPhyCfg post_h8_enter[] = {
	{0x0C4, 0x99, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x0E8, 0x7F, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x0F0, 0x7F, PMD_ALL, PHY_PMA_TRSV, 0},
	{0x004, 0x02, PMD_ALL, PHY_PMA_COMN, 0},
	{},
};

static struct UfsCalPhyCfg pre_h8_exit[] = {
	{0x004, 0x3F, PMD_HS, PHY_PMA_COMN,      0},
	{0x0C4, 0xD9, PMD_ALL, PHY_PMA_TRSV,     0},
	{0x0E8, 0x77, PMD_ALL, PHY_PMA_TRSV,     0},
	{0x00,  0x0A, PMD_HS,  COMMON_WAIT,      0},
	{0x0F0, 0xFF, PMD_HS,  PHY_PMA_TRSV,     0},
	{0x1fc, 0x01, PMD_HS,  PHY_CDR_AFC_WAIT, 0},
	{},
};


static inline UfsCalError ufs_cal_wait_cdr_afc_check(void *hba,
			UINT32 addr, UINT32 mask, int lane)
{
	UINT32 delay_us = 1;
	UINT32 delay2_us = 40;
	UINT32 reg = 0;
	UINT32 i;

	for (i = 0; i < 100; i++) {
		MicroSecondDelay(delay2_us);

		reg = ufs_lld_pma_read(hba, PHY_PMA_TRSV_ADDR(addr, lane));
		if (mask == (reg & mask))
			return UFS_CAL_NO_ERROR;

		MicroSecondDelay(delay_us);
		ufs_lld_pma_write(hba, 0x7F, PHY_PMA_TRSV_ADDR(0xF0, lane));
		ufs_lld_pma_write(hba, 0xFF, PHY_PMA_TRSV_ADDR(0xF0, lane));
	}

	return UFS_CAL_ERROR;
}

static inline UfsCalError __match_board_by_cfg(UINT8 board, UINT8 cfg_board)
{
	UfsCalError match = UFS_CAL_ERROR;

	if (cfg_board == 0)
		match = UFS_CAL_NO_ERROR;
	else if (board == cfg_board)
		match = UFS_CAL_NO_ERROR;

	return match;
}

static inline UfsCalError __match_mode_by_cfg(struct UicPwrMode *Pmd,
								int mode)
{
	UfsCalError match = UFS_CAL_ERROR;
	UINT8 _m, _l, _g;

	_m = Pmd->Mode;
	_g = Pmd->Gear;
	_l = Pmd->Lane;

	if (mode == PMD_ALL)
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && mode == PMD_HS)
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && mode == PMD_PWM)
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && _g == 1 && _l == 1
		&& (mode & (PMD_HS_G1_L1|PMD_HS_G1_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && _g == 1 && _l == 2
		&& (mode & (PMD_HS_G1_L1|PMD_HS_G1_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && _g == 2 && _l == 1
		&& (mode & (PMD_HS_G2_L1|PMD_HS_G2_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && _g == 2 && _l == 2
		&& (mode & (PMD_HS_G2_L1|PMD_HS_G2_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && _g == 3 && _l == 1
		&& (mode & (PMD_HS_G3_L1|PMD_HS_G3_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_HS(_m) && _g == 3 && _l == 2
		&& (mode & (PMD_HS_G3_L1|PMD_HS_G3_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 1 && _l == 1
		&& (mode & (PMD_PWM_G1_L1|PMD_PWM_G1_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 1 && _l == 2
		&& (mode & (PMD_PWM_G1_L1|PMD_PWM_G1_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 2 && _l == 1
		&& (mode & (PMD_PWM_G2_L1|PMD_PWM_G2_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 2 && _l == 2
		&& (mode & (PMD_PWM_G2_L1|PMD_PWM_G2_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 3 && _l == 1
		&& (mode & (PMD_PWM_G3_L1|PMD_PWM_G3_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 3 && _l == 2
		&& (mode & (PMD_PWM_G3_L1|PMD_PWM_G3_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 4 && _l == 1
		&& (mode & (PMD_PWM_G4_L1|PMD_PWM_G4_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 4 && _l == 2
		&& (mode & (PMD_PWM_G4_L1|PMD_PWM_G4_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 5 && _l == 1
		&& (mode & (PMD_PWM_G5_L1|PMD_PWM_G5_L2)))
		match = UFS_CAL_NO_ERROR;
	else if (IS_PWR_MODE_PWM(_m) && _g == 5 && _l == 2
		&& (mode & (PMD_PWM_G5_L1|PMD_PWM_G5_L2)))
		match = UFS_CAL_NO_ERROR;

	return match;
}

static UfsCalError ufs_cal_wait_pll_lock(void *hba, UINT32 addr, UINT32 mask)
{
  UINT32 reg;

  for (INTN i = 0; i < 100; i++) {
    reg = ufs_lld_pma_read(hba, PHY_PMA_COMN_ADDR(addr));
    if (mask == (reg & mask))
      return UFS_CAL_NO_ERROR;
    MicroSecondDelay(1);
  }

  DEBUG((EFI_D_ERROR, "UFS CAL: PLL lock timeout\n"));
  return UFS_CAL_ERROR;
}

static UfsCalError ufs_cal_wait_cdr_lock(void *hba, UINT32 addr, UINT32 mask, int lane)
{
  UINT32 reg;

  for (INTN i = 0; i < 1000; i++) {
    reg = ufs_lld_pma_read(hba, PHY_PMA_TRSV_ADDR(addr, lane));
    if (mask == (reg & mask))
      return UFS_CAL_NO_ERROR;
    MicroSecondDelay(1);
  }

  DEBUG((EFI_D_ERROR, "UFS CDR lock timeout! lane %d\n", lane));
  return UFS_CAL_ERROR;
}

static UfsCalError ufs_cal_config_uic(struct UfsCalParam *p,
				  const struct UfsCalPhyCfg *cfg,
				  struct UicPwrMode *Pmd)
{
	void *hba = p->Host;
	UINT8 i = 0;

	if (!cfg)
		return UFS_CAL_INV_ARG;

	for_each_phy_cfg(cfg) {
		// was target_lane, though AvailableLane seems to be fine?
		for (i = 0; i < p->AvailableLane; i++) {
			if (UFS_CAL_ERROR ==
					__match_board_by_cfg(p->Board, cfg->Board))
				continue;
			if (Pmd && UFS_CAL_ERROR ==
					__match_mode_by_cfg(Pmd, cfg->Flag))
				continue;

			switch (cfg->Layer) {
			case PHY_PCS_COMN:
			case UNIPRO_STD_MIB:
			case UNIPRO_DBG_MIB:
				if (i == 0)
					ufs_lld_dme_set(hba, UIC_ARG_MIB(cfg->Address),
						cfg->Value);
				break;
			case PHY_PCS_RXTX:
				ufs_lld_dme_set(hba, UIC_ARG_MIB_SEL(cfg->Address, i),
						cfg->Value);
				break;
			case UNIPRO_DBG_PRD:
				if (i == 0)
					ufs_lld_dme_set(hba, UIC_ARG_MIB(cfg->Address),
						UNIPRO_MCLK_PERIOD(p));
				break;
			case PHY_PCS_RX:
				ufs_lld_dme_set(hba, UIC_ARG_MIB_SEL(cfg->Address,
					RX_LANE_0+i), cfg->Value);
				break;
			case PHY_PCS_TX:
				ufs_lld_dme_set(hba, UIC_ARG_MIB_SEL(cfg->Address,
					TX_LANE_0+i), cfg->Value);
				break;
			case PHY_PCS_RX_PRD:
				ufs_lld_dme_set(hba, UIC_ARG_MIB_SEL(cfg->Address,
					RX_LANE_0+i), UNIPRO_MCLK_PERIOD(p));
				break;

			case PHY_PCS_TX_PRD:
				ufs_lld_dme_set(hba, UIC_ARG_MIB_SEL(cfg->Address,
					TX_LANE_0+i), UNIPRO_MCLK_PERIOD(p));
				break;
			case PHY_PMA_COMN:
				if (i == 0)
					ufs_lld_pma_write(hba, cfg->Value,
						PHY_PMA_COMN_ADDR(cfg->Address));
				break;
			case PHY_PMA_TRSV:
				ufs_lld_pma_write(hba, cfg->Value,
						PHY_PMA_TRSV_ADDR(cfg->Address, i));
				break;
			case PHY_PMA_TRSV_LANE1_SQ_OFF:
				if (i == 1)
					ufs_lld_pma_write(hba, cfg->Value,
						PHY_PMA_TRSV_ADDR(cfg->Address, i));
				break;
			case UNIPRO_DBG_APB:
				ufs_lld_unipro_write(hba, cfg->Value, cfg->Address);
				break;
			case PHY_PLL_WAIT:
				if (i == 0) {
					if (ufs_cal_wait_pll_lock(hba,
						cfg->Address, cfg->Value) ==
								UFS_CAL_ERROR)
						return UFS_CAL_TIMEOUT;
				}
				break;
			case PHY_CDR_WAIT:
				if (ufs_cal_wait_cdr_lock(hba,
						cfg->Address, cfg->Value, i) ==
								UFS_CAL_ERROR)
					return UFS_CAL_TIMEOUT;
				break;
			case PHY_CDR_AFC_WAIT:
				if (ufs_cal_wait_cdr_afc_check(hba,
						cfg->Address, cfg->Value, i) ==
						UFS_CAL_ERROR)
					return UFS_CAL_TIMEOUT;
				break;
			case COMMON_WAIT:
				if (i == 0)
					MicroSecondDelay(cfg->Value);
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
static void ufs_cal_calib_hibern8_values(void *hba)
{
	UINT32 hw_cap_min_tactivate;
	UINT32 peer_rx_min_actv_time_cap;
	UINT32 max_rx_hibern8_time_cap;

	ufs_lld_dme_get(hba, UIC_ARG_MIB_SEL(0x8F, RX_LANE_0),
			&hw_cap_min_tactivate);	/* HW Capability of MIN_TACTIVATE */

	ufs_lld_dme_get(hba, UIC_ARG_MIB(0x15A8),
			&peer_rx_min_actv_time_cap);	/* PA_TActivate */
	ufs_lld_dme_get(hba, UIC_ARG_MIB(0x15A7),
			&max_rx_hibern8_time_cap);	/* PA_Hibern8Time */

	if (peer_rx_min_actv_time_cap >= hw_cap_min_tactivate)
		ufs_lld_dme_peer_set(hba, UIC_ARG_MIB(0x15A8),
				peer_rx_min_actv_time_cap + 1);
	ufs_lld_dme_set(hba, UIC_ARG_MIB(0x15A7), max_rx_hibern8_time_cap + 1);
}

UfsCalError ufs_cal_post_h8_enter(struct UfsCalParam *p)
{
	UfsCalError ret = UFS_CAL_NO_ERROR;

	ret = ufs_cal_config_uic(p, post_h8_enter, p->Pmd);

	return ret;
}

UfsCalError ufs_cal_pre_h8_exit(struct UfsCalParam *p)
{
	UfsCalError ret = UFS_CAL_NO_ERROR;

	ret = ufs_cal_config_uic(p, pre_h8_exit, p->Pmd);

	return ret;
}

/*
 * This currently uses only SLOW_MODE and FAST_MODE.
 * If you want others, you should modify this function.
 */
UfsCalError UfsCalPrePmc(struct UfsCalParam *p)
{
	UfsCalError ret = UFS_CAL_NO_ERROR;
	struct UfsCalPhyCfg *cfg;

	if ((p->Pmd->Mode == SLOW_MODE) || (p->Pmd->Mode == SLOWAUTO_MODE))
		cfg = calib_of_pwm;
	else if (p->Pmd->HsSeries == PA_HS_MODE_B)
		cfg = calib_of_hs_rate_b;
	else if (p->Pmd->HsSeries == PA_HS_MODE_A)
		cfg = calib_of_hs_rate_a;
	else
		return UFS_CAL_INV_ARG;

	/*
	 * If a number of target lanes is 1 and a host's
	 * a number of available lanes is 2,
	 * you should turn off phy power of lane #1.
	 *
	 * This must be modified when a number of avaiable lanes
	 * would grow in the future.
	 */
	// lets ignore this and pray shall we.
	//if ((p->available_lane == 2) && (p->target_lane == 1))
	//	ret = ufs_cal_config_uic(p, lane1_sq_off, NULL);

	ret = ufs_cal_config_uic(p, cfg, p->Pmd);

	return ret;
}

/*
 * This currently uses only SLOW_MODE and FAST_MODE.
 * If you want others, you should modify this function.
 */
UfsCalError UfsCalPostPmc(struct UfsCalParam *p)
{
	UfsCalError ret = UFS_CAL_NO_ERROR;
	struct UfsCalPhyCfg *cfg;

	if ((p->Pmd->Mode == SLOWAUTO_MODE) || (p->Pmd->Mode == SLOW_MODE))
		cfg = post_calib_of_pwm;
	else if (p->Pmd->HsSeries == PA_HS_MODE_B)
		cfg = post_calib_of_hs_rate_b;
	else if (p->Pmd->HsSeries == PA_HS_MODE_A)
		cfg = post_calib_of_hs_rate_a;
	else
		return UFS_CAL_INV_ARG;

	ret = ufs_cal_config_uic(p, cfg, p->Pmd);

	return ret;
}

UfsCalError UfsCalPostLink(struct UfsCalParam *p)
{
	UfsCalError ret = UFS_CAL_NO_ERROR;

	ufs_cal_calib_hibern8_values(p->Host);

	ret = ufs_cal_config_uic(p, post_init_cfg, NULL);

	return ret;
}

UfsCalError UfsCalPreLink(struct UfsCalParam *p)
{
	UfsCalError ret = UFS_CAL_NO_ERROR;

	ret = ufs_cal_config_uic(p, init_cfg, NULL);

	return ret;
}

UfsCalError UfsCalInit (struct UfsCalParam *p)
{
  ufs_cal[0]            = p;
  ufs_cal_lock_timeout  = 0xFFFFFFFF;

  return UFS_CAL_NO_ERROR;
}

UINT8 UfsCalGetTargetBoard (VOID)
{
  return BRD_UNIV; // change to zero if it doesnt match any board
}