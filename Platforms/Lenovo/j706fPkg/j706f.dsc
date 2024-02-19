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
  PLATFORM_NAME                  = j706f
  PLATFORM_GUID                  = ec1c4aba-c35e-404f-b515-e616d5aa9bff
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/j706fPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = j706fPkg/j706f.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

  # 0 = SM7150-AA
  # 1 = SM7150-AB
  # 2 = SM7150-AC
  SOC_TYPE                       = 1

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE)

[LibraryClasses.common]
  DeviceMemoryMapLib|j706fPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|j706fPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000                # Starting Address

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"CloudSweets"  # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000             # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Lenovo Group Limited"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"P11 Pro"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"j706f"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"P11_Pro_j706f"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"P11 Pro"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|2560
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1600
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xA2370000

  # SD Card
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|TRUE
  
  # Usb Init
  gQcomPkgTokenSpaceGuid.PcdUSBInitOnBoot|TRUE

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|320
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|84
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|320
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|84

!include SM7150Pkg/SM7150Pkg.dsc.inc
