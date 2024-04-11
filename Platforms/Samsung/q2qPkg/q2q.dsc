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
  PLATFORM_NAME                  = q2q
  PLATFORM_GUID                  = D6D8E3F8-5A15-4B49-844B-35EFCE9B947B
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/q2qPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = q2qPkg/q2q.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0

  # 0 = SM8350
  # 1 = SM8350-AB
  # 2 = SM8350-AC
  SOC_TYPE                       = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DAB_SLOT_SUPPORT=$(AB_SLOT_SUPPORT)

[LibraryClasses]
  DeviceMemoryMapLib|q2qPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|q2qPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Azkali"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Samsung Electronics"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Z Fold 3"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"q2q"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Z_Fold_3_q2q"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Z Fold 3"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|2208
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1768
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xF0B00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|FALSE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|2208
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|2208
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|276
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|93
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|276
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|93

!include SM8350Pkg/SM8350Pkg.dsc.inc
