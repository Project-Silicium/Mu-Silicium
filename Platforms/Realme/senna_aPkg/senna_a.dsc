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
  PLATFORM_NAME                  = senna_a
  PLATFORM_GUID                  = D3762F38-80DD-4BD9-8966-AD628C00F9D8
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/senna_aPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = senna_aPkg/senna_a.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM8475    (Underclocked)
  # 1 = SM8475    (Default)
  # 2 = SM7475-AB
  #
  SOC_TYPE                       = 2

!include PalimaPkg/PalimaPkg.dsc.inc

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
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT Neo5 SE"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"senna_a"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"GT_Neo5_SE_senna_a"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"GT Neo5 SE"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1240
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2772
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|senna_aPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|senna_aPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
