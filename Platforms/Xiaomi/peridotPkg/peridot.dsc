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
  PLATFORM_NAME                  = peridot
  PLATFORM_GUID                  = A685386-EDF8-44EC-966E-B14635B6DB08
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/peridotPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = peridotPkg/peridot.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD) 

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"PugzAreCute"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0xA7600000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"POCO F6"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"peridot"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"POCO_F6_peridot"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"POCO F6"

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1220
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2712
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703FD38
  gQcomPkgTokenSpaceGuid.PcdDTBExtensionAddr|0xA703F0E8

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xF3200000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1220
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2712
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1220
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2712
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|152
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|142
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|152
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|142

[LibraryClasses]
  DeviceMemoryMapLib|peridotPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|peridotPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include SM8635Pkg/SM8635Pkg.dsc.inc
