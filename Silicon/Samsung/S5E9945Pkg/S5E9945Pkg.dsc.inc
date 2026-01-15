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
  *_*_AARCH64_CC_FLAGS = -march=armv9.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|10
  gArmPlatformTokenSpaceGuid.PcdClusterCount|4

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10240000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10240000

  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30

  # DDR Info
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x4F000
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x848

  # SmBios
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 2400 @ 3.21 GHz"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 2400e @ 3.11 GHz"
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9945"

  # SmBios CPU Extras
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3210
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3110
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x2000       # Unofficial Value

  # SmBios Memory Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|4200             # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

[LibraryClasses]
  #
  # UEFI Libraries
  #
  HwResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf

  #
  # Silicium Libraries
  #
  PlatformPrePiLib|S5E9945Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  SoCPlatformLib|S5E9945Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

  #
  # Custom Libraries
  #
  GpioBankLib|S5E9945Pkg/Library/GpioBankLib/GpioBankLib.inf

!include SamsungPkg/SamsungPkg.dsc.inc
