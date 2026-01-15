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

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x104E0000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x104E2000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x104E2000

  # Timer
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30

  # DDR Info (Unknown)
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x0
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x0

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 7870 @ 1.60 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E7870"

  # SmBios CPU Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|1600
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x100    # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x100    # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x200        # Unofficial Value

  # SmBios Memory Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|933              # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|29

[LibraryClasses]
  #
  # UEFI Libraries
  #
  HwResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf

  #
  # Silicium Libraries
  #
  PlatformPrePiLib|S5E7870Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  SoCPlatformLib|S5E7870Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

  #
  # Custom Libraries
  #
  GpioBankLib|S5E7870Pkg/Library/GpioBankLib/GpioBankLib.inf

[Components]
  SamsungPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
  S5E7870Pkg/Drivers/MshcDxe/MshcDxe.inf

!include SamsungPkg/SamsungPkg.dsc.inc
