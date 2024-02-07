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
  PLATFORM_NAME                  = ginkgo
  PLATFORM_GUID                  = 630d1379-7d3f-4511-a6f9-578abbc07518
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/ginkgoPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = ginkgoPkg/ginkgo.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0

[LibraryClasses.common]
  DeviceMemoryMapLib|ginkgoPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|ginkgoPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000              # Starting Address

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Robotix22"  # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000           # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi Inc"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 8/8T"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"ginkgo"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_Note_8/8T_ginkgo"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi Note 8/8T"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2340
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0x60000000

  # SD Card
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|TRUE

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|123
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|123

!include SM6125Pkg/SM6125Pkg.dsc.inc
