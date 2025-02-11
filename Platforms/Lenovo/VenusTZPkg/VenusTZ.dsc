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
  PLATFORM_NAME                  = VenusTZ
  PLATFORM_GUID                  = F9287658-DC58-4EE4-9606-9A3CFE9A0105
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/VenusTZPkg
  SUPPORTED_ARCHITECTURES        = ARM
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = VenusTZPkg/VenusTZ.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 1

  #
  # 0 = T30L
  # 1 = T30
  # 2 = AP33
  # 3 = T33
  #
  SOC_TYPE                       = 1

  #
  # 0 = DDR3-L
  # 1 = LPDDR2
  #
  RAM_MODEL                      = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DRAM_MODEL=$(RAM_MODEL) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|VenusTZPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  KeypadDeviceLib|VenusTZPkg/GPLLibrary/KeypadDeviceLib/KeypadDeviceLib.inf
  DevicePrePiLib|VenusTZPkg/Library/DevicePrePiLib/DevicePrePiLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"No Maintainer"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x80200000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x80201000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Lenovo Group Limited"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"IdeaPad Yoga 11"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"VenusTZ"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"IdeaPad_Yoga_11_VenusTZ"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"IdeaPad Yoga 11"

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1366
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|768
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM
  gNvidiaPkgTokenSpaceGuid.PcdRamPartitionBase|0xFD15A000

[Components]
  # Keypad
  SiliciumPkg/Drivers/KeypadDxe/KeypadDxe.inf
  SiliciumPkg/Drivers/KeypadDeviceDxe/KeypadDeviceDxe.inf

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1366
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1366
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|170
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|40
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|170
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|40

!include Tegra30Pkg/Tegra30Pkg.dsc.inc
