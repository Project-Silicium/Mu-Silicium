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
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2030313839453553
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x9810

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
  # GPIO
  # List Contents: 0x10430000, 0x10830000, 0x11050000, 0x11430000, 0x13880000, 0x13A80000, 0x14050000, 0x14220000, 0x17C60000
  #
  gSamsungPkgTokenSpaceGuid.PcdGpioCtrlAddr|{ 0x00, 0x00, 0x43, 0x10, 0x00, 0x00, 0x83, 0x10, 0x00, 0x00, 0x05, 0x11, 0x00, 0x00, 0x43, 0x11, 0x00, 0x00, 0x88, 0x13, 0x00, 0x00, 0xA8, 0x13, 0x00, 0x00, 0x05, 0x14, 0x00, 0x00, 0x22, 0x14, 0x00, 0x00, 0xC6, 0x17 }

  #
  # SPEEDY
  # List Contents: 0x141C0000
  #
  gSamsungPkgTokenSpaceGuid.PcdSpeedyBusAddr|{ 0x00, 0x00, 0x1C, 0x14 }

  #
  # Hypervisor
  #
  gSamsungPkgTokenSpaceGuid.PcdVmmStackOffset|0x2000

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
  # IO Libraries
  #
  GpioBankLib|S5E9810Pkg/Library/GpioBankLib/GpioBankLib.inf

  #
  # Power Management Libraries
  #
  PmicLib|S5E9810Pkg/Library/PmicLib/PmicLib.inf

  #
  # Storage Libraries
  #
  PlatformUfsLib|S5E9810Pkg/Library/PlatformUfsLib/PlatformUfsLib.inf
