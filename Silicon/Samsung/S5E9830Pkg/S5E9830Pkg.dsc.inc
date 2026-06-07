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
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10101000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10102000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10102000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000

  #
  # RAM Manager
  #
  gSamsungPkgTokenSpaceGuid.PcdHasSecurePgtbl|TRUE
  gSamsungPkgTokenSpaceGuid.PcdHasHarx|TRUE

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 990 @ 2.73 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9830"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2730
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x1000
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|2750                                 # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E9830Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # IO Libraries
  #
  GpioLib|S5E9830Pkg/Library/GpioLib/GpioLib.inf
  SpeedyLib|S5E9830Pkg/Library/SpeedyLib/SpeedyLib.inf

  #
  # Storage Libraries
  #
  PlatformUfsLib|S5E9830Pkg/Library/PlatformUfsLib/PlatformUfsLib.inf
