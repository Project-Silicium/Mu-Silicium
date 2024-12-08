/**
  Copyright (C) 2010-2014, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _TEGRA_CLOCK_H_
#define _TEGRA_CLOCK_H_

typedef struct clk_pll {
  UINT32 pll_base;
  UINT32 pll_out[2];
  UINT32 pll_misc;
} PllClock;

typedef struct clk_pll_simple {
  UINT32 pll_base;
  UINT32 pll_misc;
} SimplePllClock;

typedef struct clk_set_clr {
  UINT32 set;
  UINT32 clr;
} ClockSetClear;

enum {
  TEGRA_CLK_PLLS        = 6,
  TEGRA_CLK_SIMPLE_PLLS = 3,
  TEGRA_CLK_REGS        = 3,
  TEGRA_CLK_SOURCES     = 64,
  TEGRA_CLK_REGS_VW     = 2,
  TEGRA_CLK_SOURCES_VW  = 32,
  TEGRA_CLK_SOURCES_X   = 32,
  TEGRA_CLK_SOURCES_Y   = 18
};

typedef struct clk_rst_ctlr {
  UINT32 crc_rst_src;
  UINT32 crc_rst_dev[TEGRA_CLK_REGS];
  UINT32 crc_clk_out_enb[TEGRA_CLK_REGS];
  UINT32 crc_reserved0;
  UINT32 crc_cclk_brst_pol;
  UINT32 crc_super_cclk_div;
  UINT32 crc_sclk_brst_pol;
  UINT32 crc_super_sclk_div;
  UINT32 crc_clk_sys_rate;
  UINT32 crc_prog_dly_clk;
  UINT32 crc_aud_sync_clk_rate;
  UINT32 crc_reserved1;
  UINT32 crc_cop_clk_skip_plcy;
  UINT32 crc_clk_mask_arm;
  UINT32 crc_misc_clk_enb;
  UINT32 crc_clk_cpu_cmplx;
  UINT32 crc_osc_ctrl;
  UINT32 crc_pll_lfsr;
  UINT32 crc_osc_freq_det;
  UINT32 crc_osc_freq_det_stat;
  UINT32 crc_reserved2[8];

  PllClock crc_pll[TEGRA_CLK_PLLS];
  SimplePllClock crc_pll_simple[TEGRA_CLK_SIMPLE_PLLS];

  UINT32 crc_reserved10;
  UINT32 crc_reserved11;
  UINT32 crc_clk_src[TEGRA_CLK_SOURCES];
  UINT32 crc_reserved20[32];
  UINT32 crc_clk_out_enb_x;
  UINT32 crc_clk_enb_x_set;
  UINT32 crc_clk_enb_x_clr;
  UINT32 crc_rst_devices_x;
  UINT32 crc_rst_dev_x_set;
  UINT32 crc_rst_dev_x_clr;
  UINT32 crc_clk_out_enb_y;
  UINT32 crc_clk_enb_y_set;
  UINT32 crc_clk_enb_y_clr;
  UINT32 crc_rst_devices_y;
  UINT32 crc_rst_dev_y_set;
  UINT32 crc_rst_dev_y_clr;
  UINT32 crc_reserved21[17];
  UINT32 crc_dfll_base;
  UINT32 crc_reserved22[2];

  ClockSetClear crc_rst_dev_ex[TEGRA_CLK_REGS];

  UINT32 crc_reserved30[2];

  ClockSetClear crc_clk_enb_ex[TEGRA_CLK_REGS];

  UINT32 crc_reserved31[2];
  UINT32 crc_cpu_cmplx_set;
  UINT32 crc_cpu_cmplx_clr;
  UINT32 crc_clk_cpu_cmplx_set;
  UINT32 crc_clk_cpu_cmplx_clr;
  UINT32 crc_reserved32[2];
  UINT32 crc_rst_dev_vw[TEGRA_CLK_REGS_VW];
  UINT32 crc_clk_out_enb_vw[TEGRA_CLK_REGS_VW];
  UINT32 crc_cclkg_brst_pol;
  UINT32 crc_super_cclkg_div;
  UINT32 crc_cclklp_brst_pol;
  UINT32 crc_super_cclkp_div;
  UINT32 crc_clk_cpug_cmplx;
  UINT32 crc_clk_cpulp_cmplx;
  UINT32 crc_cpu_softrst_ctrl;
  UINT32 crc_cpu_softrst_ctrl1;
  UINT32 crc_cpu_softrst_ctrl2;
  UINT32 crc_reserved33[9];
  UINT32 crc_clk_src_vw[TEGRA_CLK_SOURCES_VW];

  ClockSetClear crc_rst_dev_ex_vw[TEGRA_CLK_REGS_VW];
  ClockSetClear crc_clk_enb_ex_vw[TEGRA_CLK_REGS_VW];

  UINT32 crc_rst_cpug_cmplx_set;
  UINT32 crc_rst_cpug_cmplx_clr;
  UINT32 crc_rst_cpulp_cmplx_set;
  UINT32 crc_rst_cpulp_cmplx_clr;
  UINT32 crc_clk_cpug_cmplx_set;
  UINT32 crc_clk_cpug_cmplx_clr;
  UINT32 crc_clk_cpulp_cmplx_set;
  UINT32 crc_clk_cpulp_cmplx_clr;
  UINT32 crc_cpu_cmplx_status;
  UINT32 crc_reserved40[1];
  UINT32 crc_intstatus;
  UINT32 crc_intmask;
  UINT32 crc_utmip_pll_cfg0;
  UINT32 crc_utmip_pll_cfg1;
  UINT32 crc_utmip_pll_cfg2;
  UINT32 crc_plle_aux;
  UINT32 crc_sata_pll_cfg0;
  UINT32 crc_sata_pll_cfg1;
  UINT32 crc_pcie_pll_cfg0;
  UINT32 crc_prog_audio_dly_clk;
  UINT32 crc_audio_sync_clk_i2s0;
  UINT32 crc_audio_sync_clk_i2s1;
  UINT32 crc_audio_sync_clk_i2s2;
  UINT32 crc_audio_sync_clk_i2s3;
  UINT32 crc_audio_sync_clk_i2s4;
  UINT32 crc_audio_sync_clk_spdif;

  SimplePllClock plld2;

  UINT32 crc_utmip_pll_cfg3;
  UINT32 crc_pllrefe_base;
  UINT32 crc_pllrefe_misc;
  UINT32 crs_reserved_50[7];
  UINT32 crc_pllc2_base;
  UINT32 crc_pllc2_misc0;
  UINT32 crc_pllc2_misc1;
  UINT32 crc_pllc2_misc2;
  UINT32 crc_pllc2_misc3;
  UINT32 crc_pllc3_base;
  UINT32 crc_pllc3_misc0;
  UINT32 crc_pllc3_misc1;
  UINT32 crc_pllc3_misc2;
  UINT32 crc_pllc3_misc3;
  UINT32 crc_pllx_misc1;
  UINT32 crc_pllx_misc2;
  UINT32 crc_pllx_misc3;
  UINT32 crc_xusbio_pll_cfg0;
  UINT32 crc_xusbio_pll_cfg1;
  UINT32 crc_plle_aux1;
  UINT32 crc_pllp_reshift;
  UINT32 crc_utmipll_hw_pwrdn_cfg0;
  UINT32 crc_pllu_hw_pwrdn_cfg0;
  UINT32 crc_xusb_pll_cfg0;
  UINT32 crc_reserved51[1];
  UINT32 crc_clk_cpu_misc;
  UINT32 crc_clk_cpug_misc;
  UINT32 crc_clk_cpulp_misc;
  UINT32 crc_pllx_hw_ctrl_cfg;
  UINT32 crc_pllx_sw_ramp_cfg;
  UINT32 crc_pllx_hw_ctrl_status;
  UINT32 crc_reserved52[1];
  UINT32 crc_super_gr3d_clk_div;
  UINT32 crc_spare_reg0;
  UINT32 _rsv32[4];
  UINT32 crc_plld2_ss_cfg;
  UINT32 _rsv32_1[7];

  SimplePllClock plldp;

  UINT32 crc_plldp_ss_cfg;
  UINT32 _rsrv32_2[25];
  UINT32 crc_clk_src_x[TEGRA_CLK_SOURCES_X];
  UINT32 crc_reserved61[5];
  UINT32 crc_clk_src_y[TEGRA_CLK_SOURCES_Y];
} ClockResetController;

#endif /* _TEGRA_CLOCK_H_ */
