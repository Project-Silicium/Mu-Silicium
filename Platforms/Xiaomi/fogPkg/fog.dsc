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
  PLATFORM_NAME                  = fog
  PLATFORM_GUID                  = C62242F8-E275-4150-B6F3-1CC4FAC6425C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/fogPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = fogPkg/fog.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM6225
  # 1 = SM6225-AD
  #
  SOC_TYPE                       = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0xF8, 0x42, 0x22, 0xC6, 0x75, 0xE2, 0x50, 0x41, 0xB6, 0xF3, 0x1C, 0xC4, 0xFA, 0xC6, 0x42, 0x5C }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi 10C"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"fog"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_10C_fog"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi 10C"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1650
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0x63C00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE

[LibraryClasses]
  MemoryMapLib|fogPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|fogPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include DivarPkg/DivarPkg.dsc.inc
