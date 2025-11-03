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
  PLATFORM_NAME                  = miatoll
  PLATFORM_GUID                  = 3BFFEB18-3948-4022-888F-608530C61CDD
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/miatollPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = miatollPkg/miatoll.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000 

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 9S"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"curtana"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2003J6A1G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"J6A1G"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 9 Pro India"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"curtana"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2003J6A1I"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"J6A1I"
!elseif $(DEVICE_MODEL) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 10 Lite"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"curtana"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2002F4LG"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"F4LG"
!elseif $(DEVICE_MODEL) == 3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 9 Pro Global"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"joyeuse"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2003J6B2G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"J6B2G"
!elseif $(DEVICE_MODEL) == 4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 9 Pro Max"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"excalibur"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2003J6B1I"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"J6B1I"
!elseif $(DEVICE_MODEL) == 5
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"POCO M2 Pro"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"gram"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2003J6CI"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"J6CI"
!else
!error "Invalid Model Type! 0, 1, 2, 3, 4 or 5 are Valid Model Types."
!endif

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"WP"       # Actually its "LA".
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0x9FC36588

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xA0000000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE

[LibraryClasses]
  DeviceMemoryMapLib|miatollPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|miatollPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|miatollPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include RennellPkg/RennellPkg.dsc.inc
