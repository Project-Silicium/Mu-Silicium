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
  ENABLE_KEP                                 = 0
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 0
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 0
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 0
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 0

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|25600000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10240000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10240000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00002400

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 2400 @ 3.21 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"E2400"

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|10
  gArmPlatformTokenSpaceGuid.PcdClusterCount|4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3210

  # CPU Caches
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x2000 # Unoffical Value

  # Memory Speed & Type
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|4200       # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

  # DDR Info
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x4F000
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x848

[LibraryClasses]
  PlatformPrePiLib|E2400Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|E2400Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

!include SamsungPkg/SamsungPkg.dsc.inc
