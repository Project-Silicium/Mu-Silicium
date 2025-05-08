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
  PLATFORM_NAME                  = bale
  PLATFORM_GUID                  = 6F021B78-8DAE-474D-9B43-5E2412A76E32
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/balePkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = balePkg/bale.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|balePkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|balePkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DevicePrePiLib|balePkg/Library/DevicePrePiLib/DevicePrePiLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"InternalHellhound"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0xA7600000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Realme"
  !if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT Neo6"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"bale"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"GT_Neo6_bale"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"GT Neo6"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT6"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"bale"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"GT6_bale"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"GT6"
!else
!error "Invalid Model! specify "0" for GT Neo6 or "1" for GT6."
!endif

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1264
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2780
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703FD40
  gQcomPkgTokenSpaceGuid.PcdDTBExtensionAddr|0xA703F0E8

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xE6740000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1264
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2780
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1264
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2780
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|158
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|146
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|158
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|146

!include SM8635Pkg/SM8635Pkg.dsc.inc
