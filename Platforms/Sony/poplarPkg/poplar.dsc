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
  PLATFORM_NAME                  = poplar
  PLATFORM_GUID                  = ef79c823-7988-42a1-8087-742f78a4734c
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/poplarPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = poplarPkg/poplar.fdf
  DISPLAY_USES_RGBA              = 1
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DDISPLAY_USES_RGBA=$(DISPLAY_USES_RGBA)

[LibraryClasses.common]
  DeviceMemoryMapLib|poplarPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|poplarPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000             # Starting Address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000            # 4GB Size

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Robotix22" # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000          # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Sony Group Corporation"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Xperia XZ1"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"poplar"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Xperia_XZ1_poplar"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Xperia XZ1"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1920
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|24

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1920
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1920
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|101
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|135
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|101

!include MSM8998Pkg/MSM8998Pkg.dsc.inc