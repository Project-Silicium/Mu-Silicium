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
  PLATFORM_NAME                  = i005d
  PLATFORM_GUID                  = DD9B11D5-D3A2-45A7-ADA2-6EF6EBB9CCC8
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/i005dPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = i005dPkg/i005d.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

  # 0 = SM8350
  # 1 = SM8350-AB
  # 2 = SM8350-AC
  SOC_TYPE                       = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DAB_SLOT_SUPPORT=$(AB_SLOT_SUPPORT)

[LibraryClasses]
  DeviceMemoryMapLib|i005dPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|i005dPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"alfaonyt"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Asus Inc"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"ROG Phone 5"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"i005d"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"ROG_Phone_5_i005d"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"ROG Phone 5"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2448
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xE7400000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|FALSE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2448
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2448
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|128
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|128

!include SM8350Pkg/SM8350Pkg.dsc.inc
