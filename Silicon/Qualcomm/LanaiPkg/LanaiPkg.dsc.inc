##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  #
  # Timer
  #
  USE_PHYSICAL_TIMER                         = 0

  #
  # KEP
  #
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 1
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 1
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 1
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 0

!include QcomPkg/QcomPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv9.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|4

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17100000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17180000

  #
  # SMBIOS
  #
!if $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 8 Gen 3 for Galaxy"
!else
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 8 Gen 3"
!endif
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM8650-AA"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM8650-AB"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM8650-AC"
!endif
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3052
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3302
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3398
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x3000
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|4800
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|LanaiPkg/Library/PlatformSecLib/PlatformSecLib.inf

[Components]
  #
  # Custom Drivers
  #
  QcomPkg/Drivers/IortDtDxe/IortDtDxe.inf
