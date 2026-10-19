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
  # ACPI
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2035393838453553
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x8895

  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10201000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10202000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|26000000

  #
  # SPEEDY
  # List Contents: 0x15B50000
  #
  gSamsungPkgTokenSpaceGuid.PcdSpeedyBusAddr|{ 0x00, 0x00, 0xB5, 0x15 }

  #
  # GPIO
  # List Contents: 0x104D0000, 0x10980000, 0x11050000, 0x11430000, 0x14080000, 0x15A30000, 0x164B0000, 0x17C60000
  #
  gSamsungPkgTokenSpaceGuid.PcdGpioCtrlAddr|{ 0x00, 0x00, 0x4D, 0x10, 0x00, 0x00, 0x98, 0x10, 0x00, 0x00, 0x05, 0x11, 0x00, 0x00, 0x43, 0x11, 0x00, 0x00, 0x08, 0x14, 0x00, 0x00, 0xA3, 0x15, 0x00, 0x00, 0x4B, 0x16, 0x00, 0x00, 0xC6, 0x17 }

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 8895 @ 2.30 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E8895"

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E8895Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Memory Libraries
  #
  RamManagerLib|SiliciumNullPkg/Library/RamManagerLibNull/RamManagerLibNull.inf

  #
  # IO Libraries
  #
  GpioBankLib|S5E8895Pkg/Library/GpioBankLib/GpioBankLib.inf

  #
  # Storage Libraries
  #
  # PlatformUfsLib|S5E8895Pkg/Library/PlatformUfsLib/PlatformUfsLib.inf
