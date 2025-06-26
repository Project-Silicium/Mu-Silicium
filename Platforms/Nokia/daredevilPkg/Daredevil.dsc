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
  PLATFORM_NAME                  = daredevil
  PLATFORM_GUID                  = 9B026CA1-3CD5-434C-8CB9-79FA64ECF5CC
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/DaredevilPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = daredevilPkg/daredevil.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  SOC_TYPE                       = 0


# If not don't add "-DSOC_TYPE=$(SOC_TYPE)" to the Build Options.
[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|daredevilPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|daredevilPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"Creepreee"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"HMD Global"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Nokia 7.2"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"daredevil"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Nokia 7.2_daredevil"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Nokia 7.2"

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2340
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|24

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0x85800000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|TRUE            # If your Phone has no SD Card Slot, Set it to FALSE.
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE            # This should be TRUE unless your UsbConfigDxe is Patched to be Dual Role.

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|120
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|120

!include SDM660Pkg/SDM660.dsc.inc
