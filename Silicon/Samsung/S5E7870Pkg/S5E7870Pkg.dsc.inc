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
  # ACPI
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2030373837453553
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x7870

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
  # GPIO
  # List Contents: 0x10530000, 0x13750000, 0x139B0000, 0x139C0000, 0x139D0000, 0x139E0000, 0x139F0000, 0x148C0000
  #
  gSamsungPkgTokenSpaceGuid.PcdGpioCtrlAddr|{ 0x00, 0x00, 0x53, 0x10, 0x00, 0x00, 0x75, 0x13, 0x00, 0x00, 0x9B, 0x13, 0x00, 0x00, 0x9C, 0x13, 0x00, 0x00, 0x9D, 0x13, 0x00, 0x00, 0x9E, 0x13, 0x00, 0x00, 0x9F, 0x13, 0x00, 0x00, 0x8C, 0x14 }

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
  GpioBankLib|S5E7870Pkg/Library/GpioBankLib/GpioBankLib.inf

  #
  # Storage Libraries
  #
  PlatformMshcLib|S5E7870Pkg/Library/PlatformMshcLib/PlatformMshcLib.inf
