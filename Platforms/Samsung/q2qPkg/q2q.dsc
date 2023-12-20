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
  PLATFORM_NAME                  = q2q
  PLATFORM_GUID                  = d6d8e3f8-5a15-4b49-844b-35efce9b947b
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/q2qPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = q2qPkg/q2q.fdf
  DISPLAY_USES_RGBA              = 0
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0

  # 0 = SM8350
  # 1 = SM8350-AB
  # 2 = SM8350-AC
  SOC_TYPE                       = 0

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DSOC_TYPE=$(SOC_TYPE) -DDISPLAY_USES_RGBA=$(DISPLAY_USES_RGBA)

[LibraryClasses.common]
  DeviceMemoryMapLib|q2qPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|q2qPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000           # Starting Address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x300000000          # 12 GB Size

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Azkali"  # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000        # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Samsung Electronics"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Z Fold 3"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"q2q"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Z_Fold_3_q2q"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Z Fold 3"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|2208
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1768
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|2208
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|2208
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|276
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|93
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|276
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|93

!include SM8350Pkg/SM8350Pkg.dsc.inc