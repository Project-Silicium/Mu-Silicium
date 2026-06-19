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
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdFvDecompressAddr|0xA7045878

  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17260000

  #
  # SMBIOS
  #
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 7s Gen 2"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM7435"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 6 Gen 1"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM6450"
!endif
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2400
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2200
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x400
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|3200
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|2750
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|NetraniPkg/Library/PlatformSecLib/PlatformSecLib.inf

[Components]
  #
  # Custom Drivers
  #
  QcomPkg/Drivers/IortDtDxe/IortDtDxe.inf
