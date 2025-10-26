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
  PLATFORM_NAME                  = spes
  PLATFORM_GUID                  = A43A1082-0066-4CC0-BC78-E3A59A3E5832
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/spesPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = spesPkg/spes.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM6225
  # 1 = SM6225-AD
  #
  SOC_TYPE                       = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD) -DUSE_CUSTOM_DISPLAY_DRIVER=$(USE_CUSTOM_DISPLAY_DRIVER)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000

  # Device Maintainer
  #gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"No Maintainer"

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 11"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"spes"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_Note_11_spes"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi Note 11"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0x63C00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[LibraryClasses]
  DeviceMemoryMapLib|spesPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|spesPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|spesPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include DivarPkg/DivarPkg.dsc.inc
