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
  PLATFORM_NAME                  = fog
  PLATFORM_GUID                  = C62242F8-E275-4150-B6F3-1CC4FAC6425C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/fogPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = fogPkg/fog.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

  # 0 = SM6225
  # 1 = SM6225-AD
  SOC_TYPE                       = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DAB_SLOT_SUPPORT=$(AB_SLOT_SUPPORT)

[LibraryClasses]
  DeviceMemoryMapLib|fogPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|fogPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000

  # Device Maintainer
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Statzar"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi Inc"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi 10C"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"fog"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_10C_fog"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi 10C"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1650
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0x60000000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdSDCardSlotPresent|TRUE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1650
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1650
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|86
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|86

!include SM6225Pkg/SM6225Pkg.dsc.inc
