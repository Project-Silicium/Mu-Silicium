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

!include EristaPkg/EristaPkg.dsc.inc
!else
  #
  # 0 = ODNX10-A1
  # 1 = TM675M-A1
  #
  SOC_TYPE                       = 0

!include MarikoPkg/MarikoPkg.dsc.inc
!endif

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xAA200000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Nintendo"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"nx"
!if ($(DEVICE_MODEL) == 0) || ($(DEVICE_MODEL) == 1)
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Switch"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"HAC-001"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"icosa"
!else
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"HAC-001(-01)"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"iowa"
!endif
!elseif $(DEVICE_MODEL) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Switch Lite"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"HDH-001"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"hoag"
!elseif $(DEVICE_MODEL) == 3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Switch OLED"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"HEG-001"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"aula"
!endif

  #
  # Simple Frame Buffer (TODO: Rotate Screen Somehow)
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1280
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|nxPkg/Library/MemoryMapLib/MemoryMapLib.inf
