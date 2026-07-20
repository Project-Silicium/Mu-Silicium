##
#
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = macan
  PLATFORM_GUID                  = 49301A81-23FB-4BDB-851D-1B4FBE643E04
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/macanPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = macanPkg/macan.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!include MolokaiPkg/MolokaiPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"OnePlus"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"15R"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"macan"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"CPH2769"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"24831"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1272
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2800
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703E118
  gQcomPkgTokenSpaceGuid.PcdSchedulerInterfaceAddr|0xA703EE98

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|macanPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|macanPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
