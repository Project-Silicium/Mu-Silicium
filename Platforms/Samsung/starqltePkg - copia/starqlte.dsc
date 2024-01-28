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
  PLATFORM_NAME                  = starqlte
  PLATFORM_GUID                  = d6d8e3f8-5a15-4b49-844b-35efce9b947b
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/starqltePkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = starqltePkg/starqlte.fdf
  USE_DISPLAYDXE                 = 0

[LibraryClasses.common]
  DeviceMemoryMapLib|starqltePkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|starqltePkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000              # Starting Address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000             # 4 GB Size

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"vice2008"  # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000        # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Samsung Electronics"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy S9 (Qualcomm)"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"starqlte"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_S9_starqlte"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy S9 (Qualcomm)"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1440
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2960
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferColorDepth|32

  # Power Services
  gQcomPkgTokenSpaceGuid.PcdIsPowerOkImplemented|TRUE

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2960
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2960
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|180
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|155
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|180
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|155

!include SDM845Pkg/SDM845Pkg.dsc.inc
