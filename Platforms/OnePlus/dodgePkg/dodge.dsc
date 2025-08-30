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
  PLATFORM_NAME                  = dodge
  PLATFORM_GUID                  = FA3B7E2A-4F73-4C78-BBF2-1D8E292FFBD7
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/dodgePkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = dodgePkg/dodge.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM8750-AB
  # 1 = SM8750-3-AB
  # 2 = SM8750-AC
  #
  SOC_TYPE                       = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"index986"

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000  

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"OnePlus"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"13"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"dodge"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"13_dodge"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"13"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1440
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|3168
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703C0F0
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703CE58

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xFF300000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|3168
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|23168
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|180
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|166
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|180
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|166

[LibraryClasses]
  DeviceMemoryMapLib|dodgePkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|dodgePkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|dodgePkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[Components]
  QcomPkg/Drivers/XblDeviceTreeDxe/XblDeviceTreeDxe.inf

!include PakalaPkg/PakalaPkg.dsc.inc
