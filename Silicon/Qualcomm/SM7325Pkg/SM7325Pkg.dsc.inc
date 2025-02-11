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
  USE_PHYSICAL_TIMER                         = 0

  # KEP
  ENABLE_KEP                                 = 1
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 1
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 0
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 1
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 0

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17A60000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17A60000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00007325

  # SmBios
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 778G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SM7325"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 778G+"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SM7325-AE"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 782G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SM7325-AF"
!endif

  # CPU
!if $(SOC_TYPE) == 1
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|2520
!elseif $(SOC_TYPE) == 2
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|2710
!else
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|2400
!endif
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  # CPU Cache
  gQcomPkgTokenSpaceGuid.PcdHasLevel2Cache|TRUE
  gQcomPkgTokenSpaceGuid.PcdHasLevel3Cache|TRUE
  gQcomPkgTokenSpaceGuid.PcdLevel1InstCacheSize|0x20
  gQcomPkgTokenSpaceGuid.PcdLevel1DataCacheSize|0x20
  gQcomPkgTokenSpaceGuid.PcdLevel2CacheSize|0x80
  gQcomPkgTokenSpaceGuid.PcdLevel3CacheSize|0x400

  # Memory Speed
  gQcomPkgTokenSpaceGuid.PcdMemorySpeed|3200

  # Platform Pei HOBs
  gQcomPkgTokenSpaceGuid.PcdEnableInfoBlkHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableShimHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableFvDecompressHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableProdmodeHob|FALSE

[LibraryClasses]
  PlatformPrePiLib|SM7325Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  RFSProtectionLib|SM7325Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
  AcpiPlatformUpdateLib|SM7325Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|SM7325Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

!include QcomPkg/QcomPkg.dsc.inc
