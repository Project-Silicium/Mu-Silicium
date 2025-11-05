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
  PLATFORM_NAME                  = nx
  PLATFORM_GUID                  = 2FA9729E-6BED-4F30-ACDD-8EB3C1B4ACA5
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/nxPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = nxPkg/nx.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!if $(DEVICE_MODEL) == 0
  #
  # 0 = ODNX02-A2
  # 1 = TM670D-A1
  # 2 = TM670M-A2
  # 3 = TM671D-A2
  # 4 = TM660M-A2
  #
  SOC_TYPE                       = 0
!elseif $(DEVICE_MODEL) == 1
  #
  # 0 = ODNX10-A1
  # 1 = TM675M-A1
  #
  SOC_TYPE                       = 0
!else
!error "Invalid Model Type! 0 for Icosa, 1 for Iowa"
!endif

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x80201000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x9E, 0x72, 0xA9, 0x2F, 0xED, 0x6B, 0x30, 0x4F, 0xAC, 0xDD, 0x8E, 0xB3, 0xC1, 0xB4, 0xAC, 0xA5 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Nintendo"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Switch"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"nx"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"icosa"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"iowa"
!endif
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Switch"

  # Simple Frame Buffer (TODO: Rotate Screen Somehow)
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1280
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Dynamic RAM
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xF5D85000

[LibraryClasses]
  MemoryMapLib|nxPkg/Library/MemoryMapLib/MemoryMapLib.inf
  KeypadDeviceLib|nxPkg/Library/KeypadDeviceLib/KeypadDeviceLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[Components]
  # Keypad
  SiliciumPkg/Drivers/KeypadDxe/KeypadDxe.inf
  SiliciumPkg/Drivers/KeypadDeviceDxe/KeypadDeviceDxe.inf

!if $(DEVICE_MODEL) == 0
!include EristaPkg/EristaPkg.dsc.inc
!else
!include MarikoPkg/MarikoPkg.dsc.inc
!endif
