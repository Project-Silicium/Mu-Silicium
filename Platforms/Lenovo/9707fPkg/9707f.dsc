## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
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
  PLATFORM_NAME                  = 9707f
  PLATFORM_GUID                  = cc6036dc-ca51-493f-b5dd-1a475103cbce
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/9707fPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = 9707fPkg/9707f.fdf
  DISPLAY_USES_RGBA              = 0
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

  # 0 = SM8250
  # 1 = SM8250-AB
  # 2 = SM8250-AC
  SOC_TYPE                       = 2

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DDEVICE_RAM=$(RAM_SIZE)

[LibraryClasses.common]
  DeviceMemoryMapLib|9707fPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|9707fPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000                # Starting Address
!if $(RAM_SIZE) == 8
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000               # 8 GB Size
!elseif $(RAM_SIZE) == 12
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x300000000               # 12 GB Size
!else
!error "Invaild RAM Size! Use 8 or 12."
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"CloudSweets"  # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000             # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Lenovo Limited"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Legion Tab Y700"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"9707f"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Legion_Tab_Y700_9707f"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Legion Tab Y700"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1600
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2560
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|200
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|134
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|200
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|134

!include SM8250Pkg/SM8250Pkg.dsc.inc