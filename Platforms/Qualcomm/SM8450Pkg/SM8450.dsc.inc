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
  USE_PHYSICAL_TIMER             = 0

[BuildOptions.common]
  *_*_*_CC_FLAGS = -march=armv9-a+crypto+rcpc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17100000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17180000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00008450
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0x17180000

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  gQcomTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 8 Gen 1 @ 3.00 GHz"
  gQcomTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM8450"

[LibraryClasses.common]
  PowerServicesLib|SM8450Pkg/Library/PowerServicesLib/PowerServicesLib.inf
  PlatformPrePiLib|SM8450Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  RFSProtectionLib|SM8450Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
  AcpiPlatformUpdateLib|SM8450Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  MsPlatformDevicesLib|SM8450Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf

[Components.common]
  SM8450Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/Qcom.dsc.inc