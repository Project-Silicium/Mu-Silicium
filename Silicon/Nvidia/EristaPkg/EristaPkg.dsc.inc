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

!include NvidiaPkg/NvidiaPkg.dsc.inc

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
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x50041000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x50042000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x50042000

  #
  # SMBIOS
  #
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
!elseif $(SOC_TYPE) == 4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 1.42 GHz"
!endif
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"ODNX02-A2"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"TM670D-A1"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"TM670M-A2"
!elseif $(SOC_TYPE) == 3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"TM671D-A2"
!elseif $(SOC_TYPE) == 4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"TM660M-A2"
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x30
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x800
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|1600                            # Incorrect?
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30                               # LPDDR3 Variants do Exist

  #
  # Clock
  #
  gNvidiaPkgTokenSpaceGuid.PcdCGeneralID|0
  gNvidiaPkgTokenSpaceGuid.PcdMemoryID|1
  gNvidiaPkgTokenSpaceGuid.PcdPeriphID|2
  gNvidiaPkgTokenSpaceGuid.PcdAudioID|3
  gNvidiaPkgTokenSpaceGuid.PcdUsbID|4
  gNvidiaPkgTokenSpaceGuid.PcdDisplayID|5
  gNvidiaPkgTokenSpaceGuid.PcdDisplay2ID|0xFF
  gNvidiaPkgTokenSpaceGuid.PcdXCpuID|6
  gNvidiaPkgTokenSpaceGuid.PcdEpciID|7
  gNvidiaPkgTokenSpaceGuid.PcdSfrom32KhzID|8
  gNvidiaPkgTokenSpaceGuid.PcdDpID|9
  gNvidiaPkgTokenSpaceGuid.Pcd32KhzID|10
  gNvidiaPkgTokenSpaceGuid.PcdOscID|11
  gNvidiaPkgTokenSpaceGuid.PcdClkmID|12
  gNvidiaPkgTokenSpaceGuid.PcdIDCount|13

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|EristaPkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Custom Libraries
  #
  TegraClockLib|EristaPkg/GPLLibrary/TegraClockLib/TegraClockLib.inf
