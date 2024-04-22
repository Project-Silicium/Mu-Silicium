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
  AB_SLOT_SUPPORT                = 0
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DAB_SLOT_SUPPORT=$(AB_SLOT_SUPPORT) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|miatollPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|miatollPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000 

  # Device Maintainer
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"N1kroks"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000 

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi Inc"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 9S"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"miatoll"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_Note_9S_miatoll"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi Note 9S"

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xA1200000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|TRUE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE
  
[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2400
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2400
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|126
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|126

!include SM7125Pkg/SM7125Pkg.dsc.inc

[LibraryClasses]
  AcpiPlatformUpdateLib|miatollPkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
