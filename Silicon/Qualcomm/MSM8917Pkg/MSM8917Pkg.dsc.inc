## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

# SoC Defines
[Defines]
  USE_PHYSICAL_TIMER = 1

[BuildOptions.common]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFeatureFlag.common]
  gArmTokenSpaceGuid.PcdArmGicV3WithV2Legacy|TRUE

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|2
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|3
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|4
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|1
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xB000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xB002000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xB000000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00008917
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0xB000000

  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 425 @ 1.40 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"MSM8917"

  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1

[LibraryClasses.common]
  PlatformPeiLib|MSM8917Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|MSM8917Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|MSM8917Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|MSM8917Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  ResetSystemLib|MSM8917Pkg/Library/ResetSystemLib/ResetSystemLib.inf

[Components.common]
  MSM8917Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
  MSM8917Pkg/Drivers/GpioTlmmDxe/GpioTlmmDxe.inf
  MSM8917Pkg/Drivers/ClockDxe/ClockDxe.inf
  MSM8917Pkg/Drivers/SpmiDxe/SpmiDxe.inf
  MSM8917Pkg/Drivers/Pm8x41Dxe/Pm8x41Dxe.inf
  MSM8917Pkg/Drivers/SmemDxe/SmemDxe.inf
  MSM8917Pkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf

!include QcomPkg/QcomPkg.dsc.inc