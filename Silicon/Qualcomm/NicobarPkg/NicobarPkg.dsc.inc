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
  # GIC
  USE_CUSTOM_GIC                             = 0

  # Timer
  USE_CUSTOM_TIMER                           = 0
  USE_PHYSICAL_TIMER                         = 1

  # KEP
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 0
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 0
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 0
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 1

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Platform Pei HOBs
  gQcomPkgTokenSpaceGuid.PcdEnableInfoBlkHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableShimHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableProdmodeHob|FALSE

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xF200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xF300000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xF300000

  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|30

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 665"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM6125"

  # SmBios CPU Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2000
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400

  # SmBios Memory Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|1866           # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

[LibraryClasses]
  PlatformPrePiLib|NicobarPkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|NicobarPkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  SoCPlatformLib|NicobarPkg/Library/SoCPlatformLib/SoCPlatformLib.inf

!include QcomPkg/QcomPkg.dsc.inc
