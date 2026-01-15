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
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc -mtune=cortex-a57

[PcdsFixedAtBuild]
  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x50041000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x50042000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x50042000

  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1+ @ 2.10 GHz"

!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"ODNX10-A1"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"TM675M-A1"
!endif

  # SmBios CPU Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x30
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x800

  # SmBios Memory Extras
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|1600    # Incorrect?
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|30

  # Clock
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
  PlatformPrePiLib|MarikoPkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  HwResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmPsciResetSystemLib/ArmPsciResetSystemLib.inf
  SoCPlatformLib|MarikoPkg/Library/SoCPlatformLib/SoCPlatformLib.inf
  DynamicRamLib|MarikoPkg/GPLLibrary/DynamicRamLib/DynamicRamLib.inf
  TegraClockLib|MarikoPkg/GPLLibrary/TegraClockLib/TegraClockLib.inf

!include NvidiaPkg/NvidiaPkg.dsc.inc
