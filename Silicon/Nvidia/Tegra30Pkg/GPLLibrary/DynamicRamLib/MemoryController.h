/**
  Copyright (C) 2014, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

typedef struct mc_ctlr {
  UINT32 reserved0[4];
  UINT32 mc_smmu_config;
  UINT32 mc_smmu_tlb_config;
  UINT32 mc_smmu_ptc_config;
  UINT32 mc_smmu_ptb_asid;
  UINT32 mc_smmu_ptb_data;
  UINT32 reserved1[3];
  UINT32 mc_smmu_tlb_flush;
  UINT32 mc_smmu_ptc_flush;
  UINT32 mc_smmu_asid_security;
  UINT32 reserved2[5];
  UINT32 mc_emem_cfg;
  UINT32 mc_emem_adr_cfg;
  UINT32 mc_emem_adr_cfg_dev0;
  UINT32 mc_emem_adr_cfg_dev1;
  UINT32 reserved3[12];
  UINT32 mc_emem_arb_reserved[28];
  UINT32 reserved4[338];
  UINT32 mc_video_protect_bom;
  UINT32 mc_video_protect_size_mb;
  UINT32 mc_video_protect_reg_ctrl;
} MemoryController;

#endif /* _MEMORY_CONTROLLER_H_ */
