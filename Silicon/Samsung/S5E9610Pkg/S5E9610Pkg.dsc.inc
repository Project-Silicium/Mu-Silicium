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

!include SamsungPkg/SamsungPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto

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
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x12302000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000

  #
  # PMIC
  #
  gSamsungPkgTokenSpaceGuid.PcdInitS2mpu09|TRUE

  #
  # DDR Info
  #
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x14000
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x848

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 9610 @ 2.30 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9610"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2300
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x40
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x200
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|1800
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

[LibraryClasses]
  #
  # UEFI Libraries
  #
  HwResetSystemLib|S5E9610Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  ResetSystemLib|S5E9610Pkg/Library/ResetSystemLib/ResetSystemLib.inf

  #
  # SEC Libraries
  #
  PlatformSecLib|S5E9610Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Custom Libraries
  #
  GpioBankLib|S5E9610Pkg/Library/GpioBankLib/GpioBankLib.inf

[Components]
  #
  # PMIC
  #
  SamsungPkg/Drivers/PmicDxe/S2mpu09Dxe.inf
