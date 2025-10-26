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
  PLATFORM_NAME                  = gta4l
  PLATFORM_GUID                  = A22E8C3E-B0BE-4C0E-8192-57EE0D9980EC
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/gta4lPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = gta4lPkg/gta4l.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD) -DUSE_CUSTOM_DISPLAY_DRIVER=$(USE_CUSTOM_DISPLAY_DRIVER)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Samsung"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Tab A7 LTE"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"gta4l"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Tab_A7_LTE_gta4l"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Tab A7 LTE"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1200
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|2000
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0x71D00000

  # Storage
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[LibraryClasses]
  DeviceMemoryMapLib|gta4lPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|gta4lPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|gta4lPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include KamortaPkg/KamortaPkg.dsc.inc
