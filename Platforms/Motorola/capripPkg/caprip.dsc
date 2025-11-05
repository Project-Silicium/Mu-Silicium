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
  PLATFORM_NAME                  = caprip
  PLATFORM_GUID                  = 01132323-0EEB-46B1-A847-680594920C62
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/capripPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = capripPkg/caprip.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000 

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000  

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x23, 0x23, 0x13, 0x01, 0xEB, 0x0E, 0xB1, 0x46, 0xA8, 0x47, 0x68, 0x05, 0x94, 0x92, 0x0C, 0x62 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Motorola"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Moto G30"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"caprip"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Moto_G30_caprip"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Moto G30"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1600
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0x68900000

  # Storage
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE

[LibraryClasses]
  MemoryMapLib|capripPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|capripPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include KamortaPkg/KamortaPkg.dsc.inc
