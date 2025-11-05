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
  PLATFORM_NAME                  = pong
  PLATFORM_GUID                  = B1604F78-DF37-4803-8BC0-BEF397B882B9
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/pongPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = pongPkg/pong.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1

  #
  # 0 = SM8475
  # 1 = SM8475-AB
  # 2 = SM7475-AB
  #
  SOC_TYPE                       = 1

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x78, 0x4F, 0x60, 0xB1, 0x37, 0xDF, 0x03, 0x48, 0x8B, 0xC0, 0xBE, 0xF3, 0x97, 0xB8, 0x82, 0xB9 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Nothing"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Phone 2"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"pong"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Phone_2_pong"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Phone 2"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2412
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703C920
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703C0C8

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xFFC00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  MemoryMapLib|pongPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|pongPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include PalimaPkg/PalimaPkg.dsc.inc
