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
  PLATFORM_NAME                  = gta9pwifi
  PLATFORM_GUID                  = EDCED809-DBA9-4B74-9881-0B3EDCF5075C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/gta9pwifiPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = gta9pwifiPkg/gta9pwifi.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x09, 0xD8, 0xCE, 0xED, 0xA9, 0xDB, 0x74, 0x4B, 0x98, 0x81, 0x0B, 0x3E, 0xDC, 0xF5, 0x07, 0x5C }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Samsung"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Tab A9+ Wi-Fi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"gta9pwifi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Tab_A9+_Wi-Fi_gta9pwifi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Tab A9+ Wi-Fi"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1200
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1920
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xA8100000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE

[LibraryClasses]
  MemoryMapLib|gta9pwifiPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|gta9pwifiPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include StraitPkg/StraitPkg.dsc.inc
