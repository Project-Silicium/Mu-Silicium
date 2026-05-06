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
  PLATFORM_NAME                  = bale_b
  PLATFORM_GUID                  = 6F021B78-8DAE-474D-9B43-5E2412A76E32
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/bale_bPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = bale_bPkg/bale_b.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM8635
  # 1 = SM7675
  #
  SOC_TYPE                       = 0

!include PalawanPkg/PalawanPkg.dsc.inc

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
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Realme"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT Neo6"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"bale_b"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"RMX3852"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"23622"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1264
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2780
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdSchedulerInterfaceAddr|0xA703FD40
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703F0E8

  #
  # Storage
  #
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|bale_bPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|bale_bPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
