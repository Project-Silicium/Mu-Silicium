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

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10101000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10102000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10102000

  # Timer
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30

  # DDR Info
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x1EC00
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x848

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 9810 @ 2.90 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9810"

  # SmBios CPU Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2900
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x100    # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x80     # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x800        # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x1000       # Unofficial Value

  # SmBios Memory Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|1800             # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

[LibraryClasses]
  #
  # UEFI Libraries
  #
  HwResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf

  #
  # Silicium Libraries
  #
  PlatformPrePiLib|S5E9810Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  SoCPlatformLib|S5E9810Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

  #
  # Custom Libraries
  #
  GpioBankLib|S5E9810Pkg/Library/GpioBankLib/GpioBankLib.inf

!include SamsungPkg/SamsungPkg.dsc.inc
