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
  PLATFORM_NAME                  = aston
  PLATFORM_GUID                  = 5CA58D2B-99C8-40DE-A7A5-44C5DAAE24BD
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/astonPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = astonPkg/aston.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|astonPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|astonPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DevicePrePiLib|astonPkg/Library/DevicePrePiLib/DevicePrePiLib.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000  

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"Shandorman"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0xA7600000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000  

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"OnePlus"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Ace 3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"aston"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Ace 3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Ace 3"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"12R"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"aston"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"12R"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"12R"
!else
!error "Invalid Model Type! 0 or 1 are Valid Model Types."
!endif

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1264
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2780
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdDTBExtensionAddr|0xA703A0C8
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703A950

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0x880000000 

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

!include SM8550Pkg/SM8550Pkg.dsc.inc
