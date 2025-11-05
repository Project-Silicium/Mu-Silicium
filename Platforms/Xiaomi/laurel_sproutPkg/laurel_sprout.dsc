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
  PLATFORM_NAME                  = laurel_sprout
  PLATFORM_GUID                  = 30FA3A0B-87B1-4B91-A716-CD283A813C72
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/laurel_sproutPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = laurel_sproutPkg/laurel_sprout.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x0B, 0x3A, 0xFA, 0x30, 0xB1, 0x87, 0x91, 0x4B, 0xA7, 0x16, 0xCD, 0x28, 0x3A, 0x81, 0x3C, 0x72 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Mi A3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"laurel_sprout"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Mi_A3_laurel_sprout"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Mi A3"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1560
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0x65D00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE

[LibraryClasses]
  MemoryMapLib|laurel_sproutPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|laurel_sproutPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include NicobarPkg/NicobarPkg.dsc.inc
