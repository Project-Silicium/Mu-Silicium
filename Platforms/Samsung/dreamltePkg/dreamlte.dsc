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
  PLATFORM_NAME                  = dreamlte
  PLATFORM_GUID                  = A1B48372-32DF-4294-BE88-10BF0EE9F11B
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/dreamltePkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = dreamltePkg/dreamlte.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!include S5E8895Pkg/S5E8895Pkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xC9200000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Samsung"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy S8"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"dreamlte"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_S8_dreamlte"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"Galaxy S8"

  #
  # Simple Frame Buffer
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1440
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2960
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|dreamltePkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # Input Libraries
  #
  KeypadDeviceLib|dreamltePkg/Library/KeypadDeviceLib/KeypadDeviceLib.inf

[Components]
  #
  # ACPI Tables
  #
  #dreamlte/AcpiTables.inf

  #
  # Input
  #
  SiliciumPkg/Drivers/KeypadDxe/KeypadDxe.inf
  SiliciumPkg/Drivers/KeypadDeviceDxe/KeypadDeviceDxe.inf
