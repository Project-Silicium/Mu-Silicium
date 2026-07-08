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
  USE_PHYSICAL_TIMER                         = 1

  #
  # KEP
  #
  HAS_CNTFRQ_EL0_EMPTY_ERRATA                = 0

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
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10102000

  #
  # RAM Manager
  #
  gSamsungPkgTokenSpaceGuid.PcdHasSecurePgtbl|TRUE

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 9820 @ 2.73 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9820"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2730
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x20                         # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20                         # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x80                             # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x400                            # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|2730                                 # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E9820Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # UEFI Libraries
  #
  HwResetSystemLib|S5E9820Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  ResetSystemLib|S5E9820Pkg/Library/ResetSystemLib/ResetSystemLib.inf

  #
  # IO Libraries
  #
  GpioLib|S5E9820Pkg/Library/GpioLib/GpioLib.inf
  SpeedyLib|S5E9820Pkg/Library/SpeedyLib/SpeedyLib.inf

  #
  # Power Management Libraries
  #
  PmicLib|S5E9820Pkg/Library/PmicLib/PmicLib.inf

  #
  # Samsung Libraries
  #
  UsiLib|S5E9820Pkg/Library/UsiLib/UsiLib.inf
