/**
  Copyright (C) 2014 - 2015, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

typedef struct mc_ctrl {
  UINT32 reserved0[4];
  UINT32 mc_smmu_config;
  UINT32 mc_smmu_tlb_config;
  UINT32 mc_smmu_ptc_config;
  UINT32 mc_smmu_ptb_asid;
  UINT32 mc_smmu_ptb_data;
  UINT32 reserved1[3];
  UINT32 mc_smmu_tlb_flush;
  UINT32 mc_smmu_ptc_flush;
  UINT32 reserved2[6];
  UINT32 mc_emem_cfg;
  UINT32 mc_emem_adr_cfg;
  UINT32 mc_emem_adr_cfg_dev0;
  UINT32 mc_emem_adr_cfg_dev1;
  UINT32 reserved3[4];
  UINT32 mc_security_cfg0;
  UINT32 mc_security_cfg1;
  UINT32 reserved4[6];
  UINT32 mc_emem_arb_reserved[28];
  UINT32 reserved5[74];
  UINT32 mc_smmu_translation_enable_0;
  UINT32 mc_smmu_translation_enable_1;
  UINT32 mc_smmu_translation_enable_2;
  UINT32 mc_smmu_translation_enable_3;
  UINT32 mc_smmu_afi_asid;
  UINT32 mc_smmu_avpc_asid;
  UINT32 mc_smmu_dc_asid;
  UINT32 mc_smmu_dcb_asid;
  UINT32 reserved6[2];
  UINT32 mc_smmu_hc_asid;
  UINT32 mc_smmu_hda_asid;
  UINT32 mc_smmu_isp2_asid;
  UINT32 reserved7[2];
  UINT32 mc_smmu_msenc_asid;
  UINT32 mc_smmu_nv_asid;
  UINT32 mc_smmu_nv2_asid;
  UINT32 mc_smmu_ppcs_asid;
  UINT32 mc_smmu_sata_asid;
  UINT32 reserved8[1];
  UINT32 mc_smmu_vde_asid;
  UINT32 mc_smmu_vi_asid;
  UINT32 mc_smmu_vic_asid;
  UINT32 mc_smmu_xusb_host_asid;
  UINT32 mc_smmu_xusb_dev_asid;
  UINT32 reserved9[1];
  UINT32 mc_smmu_tsec_asid;
  UINT32 mc_smmu_ppcs1_asid;
  UINT32 reserved10[235];
  UINT32 mc_video_protect_bom;
  UINT32 mc_video_protect_size_mb;
  UINT32 mc_video_protect_reg_ctrl;
} MemoryController;

#endif /* _MEMORY_CONTROLLER_H_ */
