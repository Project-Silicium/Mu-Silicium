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
  PLATFORM_NAME                  = pipa
  PLATFORM_GUID                  = 0965e99c-81b3-4f49-9abf-a7a39f591528
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/pipaPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = pipaPkg/pipa.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

  # 0 = SM8250
  # 1 = SM8250-AB
  # 2 = SM8250-AC
  SOC_TYPE                       = 2

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DDEVICE_RAM=$(RAM_SIZE)

[LibraryClasses.common]
  DeviceMemoryMapLib|pipaPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|pipaPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000        # Starting Address
!if $(RAM_SIZE) == 8
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000       # 8 GB Size
!elseif $(RAM_SIZE) == 6
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x180000000       # 6 GB Size
!else
!error "Invaild RAM Size! Use 8 or 6."
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"6adp" # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000     # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi Inc"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pad 6"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"pipa"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Pad_pipa"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Pad 6"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1800
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2880
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1800
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2880
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1800
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2880
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|225
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|151
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|225
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|151

!include SM8250Pkg/SM8250Pkg.dsc.inc
