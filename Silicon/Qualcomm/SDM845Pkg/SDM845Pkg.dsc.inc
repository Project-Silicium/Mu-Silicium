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
  USE_PHYSICAL_TIMER = 0

[BuildOptions.common]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17B00000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17A00000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00000845
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0x17A00000

  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 845 @ 2.80 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SDM845"

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  gQcomPkgTokenSpaceGuid.PcdHasLevel3Cache|FALSE

[LibraryClasses.common]
  PlatformPeiLib|SDM845Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|SDM845Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SDM845Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|SDM845Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components.common]
  SDM845Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/QcomPkg.dsc.inc