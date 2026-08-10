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
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc -mtune=cortex-a57

[PcdsFixedAtBuild]
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x50041000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x50042000

  #
  # SMBIOS
  #
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
!elseif $(SOC_TYPE) == 4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 1.40 GHz"
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

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|EristaPkg/Library/PlatformSecLib/PlatformSecLib.inf
