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
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 1
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 1
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 1
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 0

!include QcomPkg/QcomPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv9-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17100000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17180000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 8 Gen 1 @ 3.00 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"SM8450"

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|WaipioPkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # ACPI Libraries
  #
  AcpiTableUpdateLib|WaipioPkg/Library/AcpiTableUpdateLib/AcpiTableUpdateLib.inf

  #
  # Custom Libraries
  #
  RFSProtectionLib|WaipioPkg/Library/RFSProtectionLib/RFSProtectionLib.inf

[Components]
  #
  # Custom Drivers
  #
  QcomPkg/Drivers/IortDtDxe/IortDtDxe.inf
