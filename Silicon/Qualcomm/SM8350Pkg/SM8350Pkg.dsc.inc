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
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00008350

  # SmBios
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 888 5G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SM8350"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 888+ 5G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SM8350-AB"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 888 4G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SM8350-AC"
!elseif $(SOC_TYPE) == 3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) G3x Gen 1"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"SG8175P"
!endif

  # CPU
!if ($(SOC_TYPE) == 1 || $(SOC_TYPE) == 3)
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|3000
!else
  gQcomPkgTokenSpaceGuid.PcdMaxCpuFreq|2840
!endif
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  # CPU Cache
  gQcomPkgTokenSpaceGuid.PcdHasLevel2Cache|TRUE
  gQcomPkgTokenSpaceGuid.PcdHasLevel3Cache|TRUE
  gQcomPkgTokenSpaceGuid.PcdLevel1InstCacheSize|0x100
  gQcomPkgTokenSpaceGuid.PcdLevel1DataCacheSize|0x100
  gQcomPkgTokenSpaceGuid.PcdLevel2CacheSize|0x400
  gQcomPkgTokenSpaceGuid.PcdLevel3CacheSize|0x1000

  # Memory Speed
  gQcomPkgTokenSpaceGuid.PcdMemorySpeed|3200

  # Platform Pei HOBs
  gQcomPkgTokenSpaceGuid.PcdEnableInfoBlkHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableShimHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableFvDecompressHob|TRUE
  gQcomPkgTokenSpaceGuid.PcdEnableProdmodeHob|FALSE

[LibraryClasses]
  PlatformPrePiLib|SM8350Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  RFSProtectionLib|SM8350Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
  AcpiPlatformUpdateLib|SM8350Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|SM8350Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

!include QcomPkg/QcomPkg.dsc.inc
