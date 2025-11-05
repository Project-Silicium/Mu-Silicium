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
  PLATFORM_NAME                  = nabu
  PLATFORM_GUID                  = C613F7A8-7FF8-42A8-92F7-FF34EA2B9866
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/nabuPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = nabuPkg/nabu.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1

  #
  # 0 = SM8150
  # 1 = SM8150-AB
  # 2 = SM8150-AC
  #
  SOC_TYPE                       = 2

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF91000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00020000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0xA8, 0xF7, 0x13, 0xC6, 0xF8, 0x7F, 0xA8, 0x42, 0x92, 0xF7, 0xFF, 0x34, 0xEA, 0x2B, 0x98, 0x66 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pad 5"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"nabu"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"21051182G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"K82"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1600
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2560
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xA3C00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  MemoryMapLib|nabuPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|nabuPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include HanaPkg/HanaPkg.dsc.inc
