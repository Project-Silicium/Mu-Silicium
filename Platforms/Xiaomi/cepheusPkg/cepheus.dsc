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
  PLATFORM_NAME                  = cepheus
  PLATFORM_GUID                  = 20DA5DDD-A8A7-4C83-9853-7716FE7BCDD6
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/cepheusPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = cepheusPkg/cepheus.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM8150
  # 1 = SM8150-AB
  # 2 = SM8150-AC
  #
  SOC_TYPE                       = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FFB0000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00020000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0xDD, 0x5D, 0xDA, 0x20, 0xA7, 0xA8, 0x83, 0x4C, 0x98, 0x53, 0x77, 0x16, 0xFE, 0x7B, 0xCD, 0xD6 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Mi 9"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"cepheus"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Mi_9_cepheus"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Mi 9"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2340
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xC4800000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # Storage
  gQcomPkgTokenSpaceGuid.PcdStorageIsUfs3|FALSE

[LibraryClasses]
  MemoryMapLib|cepheusPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|cepheusPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include HanaPkg/HanaPkg.dsc.inc
