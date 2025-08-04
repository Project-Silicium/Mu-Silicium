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
  PLATFORM_NAME                  = b4q
  PLATFORM_GUID                  = 292FF268-8CC3-4DD9-BD66-B835819E8F0A
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/b4qPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = b4qPkg/b4q.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1
  HAS_BUILD_IN_KEYBOARD          = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000  

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"Robotix"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0xA7600000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000  

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Samsung Electronics"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Z Flip 4"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"b4q"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Z_Flip_4_b4q"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Z Flip 4"

  # Simple Frame Buffer (Main Screen)
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|2640
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Simple Frame Buffer (Cover Screen)
  gSiliciumPkgTokenSpaceGuid.PcdSecondaryFrameBufferOffset|0x00AE0600
  gSiliciumPkgTokenSpaceGuid.PcdSecondaryFrameBufferWidth|260
  gSiliciumPkgTokenSpaceGuid.PcdSecondaryFrameBufferHeight|512
  gSiliciumPkgTokenSpaceGuid.PcdSecondaryFrameBufferColorDepth|32
  gSiliciumPkgTokenSpaceGuid.PcdSecondaryFrameBufferDebug|FALSE

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703B920
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703B0C8

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xF5C00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2640
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2640
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|138
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|138

[LibraryClasses]
  DeviceMemoryMapLib|b4qPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|b4qPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|b4qPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[Components]
  QcomPkg/Drivers/XblDeviceTreeDxe/XblDeviceTreeDxe.inf

!include PalimaPkg/PalimaPkg.dsc.inc
