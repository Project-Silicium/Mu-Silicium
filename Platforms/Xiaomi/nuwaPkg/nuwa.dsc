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
  PLATFORM_NAME                  = nuwa
  PLATFORM_GUID                  = 73D912C8-FB8D-49E2-9134-572119F96A3D
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/nuwaPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = nuwaPkg/nuwa.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 1
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM8550-AB
  # 1 = SM8550-AC
  #
  SOC_TYPE                       = 0

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000  

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"alfaonyt"

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000  

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi Inc"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"13 Pro"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"nuwa"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"13_Pro_nuwa"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"13 Pro"

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1440
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|3200
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703A948
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703A0C8

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xDFC00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE
  
  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|3200
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|3200
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|180
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|168
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|180
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|168

[LibraryClasses]
  DeviceMemoryMapLib|nuwaPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|nuwaPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|nuwaPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[Components]
  QcomPkg/Drivers/XblDeviceTreeDxe/XblDeviceTreeDxe.inf

!include KailuaPkg/KailuaPkg.dsc.inc
