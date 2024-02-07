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
  PLATFORM_NAME                  = pipa
  PLATFORM_GUID                  = 0965e99c-81b3-4f49-9abf-a7a39f591528
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/pipaPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = pipaPkg/pipa.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

  # 0 = SM8250
  # 1 = SM8250-AB
  # 2 = SM8250-AC
  SOC_TYPE                       = 2

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE)

[LibraryClasses.common]
  DeviceMemoryMapLib|pipaPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|pipaPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000        # Starting Address

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"6adp" # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000     # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi Inc"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pad 6"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"pipa"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Pad_pipa"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Pad 6"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1800
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2880
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xB4500000

  # SD Card
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|FALSE

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1800
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2880
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2880
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|225
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|151
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|225
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|151

!include SM8250Pkg/SM8250Pkg.dsc.inc
