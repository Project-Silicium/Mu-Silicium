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
  PLATFORM_NAME                  = senna_bc
  PLATFORM_GUID                  = BD3BCA2B-A77C-41CA-9A19-52CBFAFABD82
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/senna_bcPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = senna_bcPkg/senna_bc.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0
  HAS_BUILD_IN_KEYBOARD          = 0

  #
  # 0 = SM8475
  # 1 = SM8475-AB
  # 2 = SM7475-AB
  #
  SOC_TYPE                       = 1

[BuildOptions]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DHAS_BUILD_IN_KEYBOARD=$(HAS_BUILD_IN_KEYBOARD)

[LibraryClasses]
  DeviceMemoryMapLib|senna_bcPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|senna_bcPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
  DeviceGuidLib|senna_bcPkg/Library/DeviceGuidLib/DeviceGuidLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # Device Maintainer
  gSiliciumPkgTokenSpaceGuid.PcdDeviceMaintainer|"artempeshkov"

  # UEFI Stack Addresses
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"realme"
!if $(DEVICE_MODEL) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT Neo5 150W"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"senna_b"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"GT_Neo5_150W_senna_b"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"GT Neo5 150W"
!elseif $(DEVICE_MODEL) == 1
    gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT Neo5 240W"
    gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"senna_c"
    gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"GT_Neo5_240W_senna_c"
    gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"GT Neo5 240W"
!elseif $(DEVICE_MODEL) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"GT3"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"senna_c"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"GT3_senna_c"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"GT3"
!else
!error "Invalid Model! specify "0" for GT Neo5 150W or "1" for GT Neo5 240W or "2" for GT3"
!endif

  # Simple FrameBuffer
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferWidth|1240
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferHeight|2772
  gSiliciumPkgTokenSpaceGuid.PcdPrimaryFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703C0C8
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703C928

  # Dynamic RAM Start Address
  gQcomPkgTokenSpaceGuid.PcdRamPartitionBase|0xFFC00000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # USB Controller
  gQcomPkgTokenSpaceGuid.PcdStartUsbController|TRUE

!include PalimaPkg/PalimaPkg.dsc.inc
