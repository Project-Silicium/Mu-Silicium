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
  PLATFORM_NAME                  = giulia
  PLATFORM_GUID                  = EA17CDFB-30F1-4CC0-8BB8-7BF51482F746
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/giuliaPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = giuliaPkg/giulia.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM8650-AA
  # 1 = SM8650-AB
  # 2 = SM8650-AC
  #
  SOC_TYPE                       = 1

!include LanaiPkg/LanaiPkg.dsc.inc

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
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"giulia"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Ace 5"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"PJK110"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"13R"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"CPH2645"
!else
!error "Invalid Model Type! 0 or 1 are Valid Model Types."
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"24825"

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

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|giuliaPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|giuliaPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
