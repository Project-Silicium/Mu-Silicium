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
  PLATFORM_NAME                  = j706f
  PLATFORM_GUID                  = EC1C4ABA-C35E-404F-B515-E616D5AA9BFF
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/j706fPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = j706fPkg/j706f.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM7150-AA
  # 1 = SM7150-AB
  # 2 = SM7150-AC
  #
  SOC_TYPE                       = 1

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|j706fPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|j706fPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DevicePrePiLib|j706fPkg/Library/DevicePrePiLib/DevicePrePiLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"CloudSweets"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Lenovo Group Limited"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"P11 Pro"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"j706f"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"P11_Pro_j706f"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"P11 Pro"

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|2560
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1600
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xA2370000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|320
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|84
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|320
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|84

!include SM7150Pkg/SM7150Pkg.dsc.inc
