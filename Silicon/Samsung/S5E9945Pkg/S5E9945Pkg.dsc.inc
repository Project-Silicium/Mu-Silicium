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
  HAS_CNTFRQ_EL0_EMPTY_ERRATA                = 0

!include SamsungPkg/SamsungPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv9.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # ACPI
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x2035343939453553
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x9945

  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|10
  gArmPlatformTokenSpaceGuid.PcdClusterCount|4

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10240000

  #
  # GPIO
  # List Contents: 0x10830000, 0x11030000, 0x11830000, 0x12850000, 0x13020000, 0x13EA0000, 0x14030000, 0x17040000, 0x17060000, 0x18030000
  #
  gSamsungPkgTokenSpaceGuid.PcdGpioCtrlAddr|{ 0x00, 0x00, 0x83, 0x10, 0x00, 0x00, 0x03, 0x11, 0x00, 0x00, 0x83, 0x11, 0x00, 0x00, 0x85, 0x12, 0x00, 0x00, 0x02, 0x13, 0x00, 0x00, 0xEA, 0x13, 0x00, 0x00, 0x03, 0x14, 0x00, 0x00, 0x04, 0x17, 0x00, 0x00, 0x06, 0x17, 0x00, 0x00, 0x03, 0x18 }

  #
  # RAM Manager
  #
  gSamsungPkgTokenSpaceGuid.PcdHasSecurePgtbl|TRUE
  gSamsungPkgTokenSpaceGuid.PcdHasHarx|TRUE
  gSamsungPkgTokenSpaceGuid.PcdHasDrmPlugin|TRUE

  #
  # SMBIOS
  #
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 2400 @ 3.21 GHz"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 2400e @ 3.11 GHz"
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"S5E9945"

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|S5E9945Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # IO Libraries
  #
  GpioBankLib|S5E9945Pkg/Library/GpioBankLib/GpioBankLib.inf
