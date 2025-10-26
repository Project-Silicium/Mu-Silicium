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
  PLATFORM_NAME                  = poplar
  PLATFORM_GUID                  = EF79C823-7988-42A1-8087-742F78A4734C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/poplarPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = poplarPkg/poplar.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD) -DUSE_CUSTOM_DISPLAY_DRIVER=$(USE_CUSTOM_DISPLAY_DRIVER)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Sony"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Xperia XZ1"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"poplar"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Xperia_XZ1_poplar"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Xperia XZ1"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|1920
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|24

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xA0000000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|FALSE

[LibraryClasses]
  DeviceMemoryMapLib|poplarPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|poplarPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|poplarPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include NazgulPkg/NazgulPkg.dsc.inc
