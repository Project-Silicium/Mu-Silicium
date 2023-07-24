## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

# SoC Defines
[Defines]
  USE_PHYSICAL_TIMER             = 1

[BuildOptions.common]
  *_*_*_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xf200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xf300000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00006125
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0xf300000

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  gQcomTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 665 @ 2.00 GHz"
  gQcomTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM6125"

[LibraryClasses.common]
  PowerServicesLib|SM6125Pkg/Library/PowerServicesLib/PowerServicesLib.inf
  PlatformPrePiLib|SM6125Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SM6125Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  MsPlatformDevicesLib|SM6125Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf

[Components.common]
  SM6125Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/Qcom.dsc.inc