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
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x12301000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x12302000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x12302000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 7885 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E7885"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2200
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x40                         # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x40                         # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x800                            # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|1866                                 # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E7885Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Custom Libraries
  #
  GpioBankLib|S5E7885Pkg/Library/GpioBankLib/GpioBankLib.inf
