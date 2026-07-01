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

!include MediaTekPkg/MediaTekPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x0C000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x0C040000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x0C040000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"MediaTek (TM) Helio G99 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"MT6789"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2200
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x80
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|2133
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|MT6789Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Custom Libraries
  #
  GpioImplLib|MT6789Pkg/Library/GpioImplLib/GpioImplLib.inf
  ClockImplLib|MT6789Pkg/Library/ClockImplLib/ClockImplLib.inf
  PmicWrapperImplLib|MT6789Pkg/Library/PmicWrapperImplLib/PmicWrapperImplLib.inf
  MsdcImplLib|MT6789Pkg/Library/MsdcImplLib/MsdcImplLib.inf
