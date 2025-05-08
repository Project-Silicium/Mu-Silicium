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
  PLATFORM_NAME                  = HAC-001
  PLATFORM_GUID                  = 2FA9729E-6BED-4F30-ACDD-8EB3C1B4ACA5
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/HAC-001Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = HAC-001Pkg/HAC-001.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = ODNX02-A2
  # 1 = TM670D-A1
  # 2 = TM670M-A2
  # 3 = TM671D-A2
  # 4 = TM660M-A2
  # 5 = ODNX10-A1
  # 6 = TM675M-A1
  #
!if $(DEVICE_MODEL) == 0
  SOC_TYPE                       = 0
!elseif $(DEVICE_MODEL) == 1
  SOC_TYPE                       = 5
!else
!error "Invalid Model Type! 0 For Icosa (Unpatched), 1 for Iowa (Patched)."
!endif

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|HAC-001Pkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  KeypadDeviceLib|HAC-001Pkg/Library/KeypadDeviceLib/KeypadDeviceLib.inf
  DevicePrePiLib|HAC-001Pkg/Library/DevicePrePiLib/DevicePrePiLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"Robotix"

  # CPU Vector Address
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x80200000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x80201000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Nintendo K.K."
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Switch"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"HAC-001"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Icosa"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Switch"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Switch (Mariko)"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"HAC-001(-01)"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Iowa"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Switch (Mariko)"
!endif

  # Simple FrameBuffer (TODO: Rotate Screen Somehow)
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1280
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Dynamic RAM
  gNvidiaPkgTokenSpaceGuid.PcdRamPartitionBase|0xF5D85000

[Components]
  # Keypad
  SiliciumPkg/Drivers/KeypadDxe/KeypadDxe.inf
  SiliciumPkg/Drivers/KeypadDeviceDxe/KeypadDeviceDxe.inf

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1280
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1280
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|67
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|67

!include Tegra210Pkg/Tegra210Pkg.dsc.inc
