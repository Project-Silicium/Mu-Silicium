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
  ENABLE_KEP                                 = 1
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 0
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 0
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 0
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 1

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17B00000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17B00000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00000660

  # SmBios
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 660"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SDM660"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 636"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SDM636"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 630"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SDM630"
!endif

  # CPU
!if $(SOC_TYPE) == 1
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|1800
!else
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|2200
!endif
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  # CPU Cache
  gQcomPkgTokenSpaceGuid.PcdHasLevel2Cache|TRUE
  gQcomPkgTokenSpaceGuid.PcdLevel1InstCacheSize|0x40
  gQcomPkgTokenSpaceGuid.PcdLevel1DataCacheSize|0x40
  gQcomPkgTokenSpaceGuid.PcdLevel2CacheSize|0x400

  # Memory Speed
  gQcomPkgTokenSpaceGuid.PcdMemorySpeed|1866

  # Platform Pei HOBs
  gQcomPkgTokenSpaceGuid.PcdEnableInfoBlkHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableShimHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableFvDecompressHob|FALSE
  gQcomPkgTokenSpaceGuid.PcdEnableProdmodeHob|FALSE

[LibraryClasses]
  PlatformPrePiLib|SDM660Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SDM660Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|SDM660Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

!include QcomPkg/QcomPkg.dsc.inc
