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
  PLATFORM_NAME                  = marble
  PLATFORM_GUID                  = A183C345-F702-489C-B599-4035AE2483A9
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/marblePkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = marblePkg/marble.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  AB_SLOT_SUPPORT                = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM8450
  # 1 = SM7475
  #
  SOC_TYPE                       = 1

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DAB_SLOT_SUPPORT=$(AB_SLOT_SUPPORT) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"YuzuruRiverflow"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Poco F5"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"marble"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Poco_F5_marble"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Poco F5"

  # Simple Frame Buffer - Poco F5 Display (1080x2400 120Hz)
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703C920 # May need adjustment for SM7475
  gQcomPkgTokenSpaceGuid.PcdDTBExtensionAddr|0xA703B0C8     # May need adjustment for SM7475

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xE0000000     # Based on iomem reserved region

  # SD Card Slot - Poco F5 has no SD card slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # USB Controller - Poco F5 has USB-C
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  # Display Configuration - Poco F5 (1080x2400)
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2400
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2400

  # Console Configuration (adjusted for 1080x2400 resolution)
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135  # 1080/8 chars
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150     # 2400/16 lines
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|150

[LibraryClasses]
  DeviceMemoryMapLib|marblePkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|marblePkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include SM7475Pkg/SM7475Pkg.dsc.inc
