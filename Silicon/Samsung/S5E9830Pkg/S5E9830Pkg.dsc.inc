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
  USE_PHYSICAL_TIMER                         = 0

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
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

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
  gSamsungPkgTokenSpaceGuid.PcdHasHarx|TRUE

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 990 @ 2.73 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9830"

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E9830Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # IO Libraries
  #
  GpioLib|S5E9830Pkg/Library/GpioLib/GpioLib.inf
  SpeedyLib|S5E9830Pkg/Library/SpeedyLib/SpeedyLib.inf
  HsI2cLib|S5E9830Pkg/Library/HsI2cLib/HsI2cLib.inf

  #
  # Power Management Libraries
  #
  PmicLib|S5E9830Pkg/Library/PmicLib/PmicLib.inf

  #
  # Storage Libraries
  #
  PlatformUfsLib|S5E9830Pkg/Library/PlatformUfsLib/PlatformUfsLib.inf

  #
  # Samsung Libraries
  #
  UsiLib|S5E9830Pkg/Library/UsiLib/UsiLib.inf
