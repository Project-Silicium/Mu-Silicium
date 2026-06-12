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
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 0
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 0
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 0
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 0

!include QcomPkg/QcomPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv9.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdFvDecompressAddr|0xA7049DF8

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
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 8s Gen 4"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM8735"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3200
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x10                      # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x10                      # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x100                         # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x400                         # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|4800
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|BonitoPkg/Library/PlatformSecLib/PlatformSecLib.inf

[Components]
  #
  # Custom Drivers
  #
  QcomPkg/Drivers/IortDtDxe/IortDtDxe.inf
