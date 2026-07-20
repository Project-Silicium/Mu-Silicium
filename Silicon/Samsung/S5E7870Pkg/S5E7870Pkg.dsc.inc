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
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x104E1000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x104E2000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 7870 @ 1.60 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E7870"

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E7870Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Memory Libraries
  #
  IoMmuLib|MdeModulePkg/Library/IoMmuLibNull/IoMmuLibNull.inf

  #
  # IO Libraries
  #
  GpioLib|S5E7870Pkg/Library/GpioLib/GpioLib.inf

[Components]
  #
  # Custom Drivers
  #
  S5E7870Pkg/Drivers/MshcDxe/MshcDxe.inf
