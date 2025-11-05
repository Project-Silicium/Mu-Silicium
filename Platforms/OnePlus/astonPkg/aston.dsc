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
  PLATFORM_NAME                  = aston
  PLATFORM_GUID                  = 5CA58D2B-99C8-40DE-A7A5-44C5DAAE24BD
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/astonPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = astonPkg/aston.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1

  #
  # 0 = SM8550-AB
  # 1 = SM8550-AC
  #
  SOC_TYPE                       = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000  

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x2B, 0x8D, 0xA5, 0x5C, 0xC8, 0x99, 0xDE, 0x40, 0xA7, 0xA5, 0x44, 0xC5, 0xDA, 0xAE, 0x24, 0xBD }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"OnePlus"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Ace 3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"aston"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"PJE110"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"23801"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"12R"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"aston"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"CPH2585"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"23861"
!else
!error "Invalid Model Type! 0 or 1 are Valid Model Types."
!endif

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1264
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2780
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703A0C8
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703A950

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xE1200000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  MemoryMapLib|astonPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|astonPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include KailuaPkg/KailuaPkg.dsc.inc
