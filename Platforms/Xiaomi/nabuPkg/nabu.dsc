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
  PLATFORM_NAME                  = nabu
  PLATFORM_GUID                  = C613F7A8-7FF8-42A8-92F7-FF34EA2B9866
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/nabuPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = nabuPkg/nabu.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM8150
  # 1 = SM8150-AB
  # 2 = SM8150-AC
  #
  SOC_TYPE                       = 2

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|nabuPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|nabuPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DevicePrePiLib|nabuPkg/Library/DevicePrePiLib/DevicePrePiLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"iNUCi"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi Inc"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pad 5"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"nabu"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Pad_nabu"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Pad 5"

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1600
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2560
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xC0300000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|200
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|134
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|200
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|134

!include SM8150Pkg/SM8150Pkg.dsc.inc
