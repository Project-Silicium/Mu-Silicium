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

!include SamsungPkg/SamsungPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x104E0000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x104E2000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x104E2000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 7870 @ 1.60 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E7870"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|1600
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x100                       # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x100                       # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x200                           # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|933                                 # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|29

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E7870Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Memory Libraries
  #
  IoMmuLib|MdeModulePkg/Library/IoMmuLibNull/IoMmuLibNull.inf

  #
  # Custom Libraries
  #
  GpioBankLib|S5E7870Pkg/Library/GpioBankLib/GpioBankLib.inf

[Components]
  #
  # Custom Drivers
  #
  S5E7870Pkg/Drivers/MshcDxe/MshcDxe.inf
