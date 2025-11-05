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

  #
  # 0 = SM8635
  # 1 = SM7675
  #
  SOC_TYPE                       = 0

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x78, 0x1B, 0x02, 0x6F, 0xAE, 0x8D, 0x4D, 0x47, 0x9B, 0x43, 0x5E, 0x24, 0x12, 0xA7, 0x6E, 0x32 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Realme"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT Neo6"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"bale"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"RMX3852"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"23622"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT 6"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"bale"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"RMX3851"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"23718"
!else
!error "Invalid Model! specify "0" for GT Neo6 or "1" for GT 6."
!endif

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1264
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2780
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703FD40
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703F0E8

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xE6740000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  MemoryMapLib|balePkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|balePkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

!include PalawanPkg/PalawanPkg.dsc.inc
