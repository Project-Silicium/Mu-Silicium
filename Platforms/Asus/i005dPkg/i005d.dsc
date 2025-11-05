##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = i005d
  PLATFORM_GUID                  = DD9B11D5-D3A2-45A7-ADA2-6EF6EBB9CCC8
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/i005dPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = i005dPkg/i005d.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM8350
  # 1 = SM8350-AB
  # 2 = SM8350-AC
  #
  SOC_TYPE                       = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0xD5, 0x11, 0x9B, 0xDD, 0xA2, 0xD3, 0xA7, 0x45, 0xAD, 0xA2, 0x6E, 0xF6, 0xEB, 0xB9, 0xCC, 0xC8 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Asus"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"ROG Phone 5"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"i005d"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"ROG_Phone_5_i005d"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"ROG Phone 5"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2448
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0x9FC37968

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xE7400000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  MemoryMapLib|i005dPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|i005dPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include LahainaPkg/LahainaPkg.dsc.inc
