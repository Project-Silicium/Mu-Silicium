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
  PLATFORM_NAME                  = alioth
  PLATFORM_GUID                  = 7F67CDAE-8312-4ECC-B948-D86C870D04D9
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/aliothPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = aliothPkg/alioth.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM8250
  # 1 = SM8250-AB
  # 2 = SM8250-AC
  #
  SOC_TYPE                       = 2

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000        

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0xAE, 0xCD, 0x67, 0x7F, 0x12, 0x83, 0xCC, 0x4E, 0xB9, 0x48, 0xD8, 0x6C, 0x87, 0x0D, 0x04, 0xD9 }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"alioth"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"POCO F3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2012K11AG"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"K11AG"
!elseif $(DEVICE_MODEL) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi K40"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2012K11AC"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"K11AC"
!elseif $(DEVICE_MODEL) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Mi 11X"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"M2012K11AI"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"K11AI"
!else
!error "Invalid Model Type! 0, 1 or 2 are Valid Model Types."
!endif

  # Simple Frame Buffer
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2400
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xA0000000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

[LibraryClasses]
  MemoryMapLib|aliothPkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|aliothPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|aliothPkg/Library/AcpiDeviceUpdateLib/AcpiDeviceUpdateLib.inf

!include KonaPkg/KonaPkg.dsc.inc