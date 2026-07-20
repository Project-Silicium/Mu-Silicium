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
  HAS_CNTFRQ_EL0_EMPTY_ERRATA                = 1

!include SamsungPkg/SamsungPkg.dsc.inc

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
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10101000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10102000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000

  #
  # RAM Manager
  #
  gSamsungPkgTokenSpaceGuid.PcdHasSecurePgtbl|TRUE

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 9810 @ 2.90 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9810"

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E9810Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # UEFI Libraries
  #
  HwResetSystemLib|S5E9810Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  ResetSystemLib|S5E9810Pkg/Library/ResetSystemLib/ResetSystemLib.inf

  #
  # IO Libraries
  #
  GpioLib|S5E9810Pkg/Library/GpioLib/GpioLib.inf
  SpeedyLib|S5E9810Pkg/Library/SpeedyLib/SpeedyLib.inf

  #
  # Power Management Libraries
  #
  PmicLib|S5E9810Pkg/Library/PmicLib/PmicLib.inf

  #
  # Storage Libraries
  #
  PlatformUfsLib|S5E9810Pkg/Library/PlatformUfsLib/PlatformUfsLib.inf
