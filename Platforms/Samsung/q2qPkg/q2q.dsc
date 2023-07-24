## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#  Copyright (c) 2022, Xilin Wu. All rights reserved.
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
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = q2qPkg/q2q.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0
  USE_UART                       = 0

[LibraryClasses.common]
  PlatformMemoryMapLib|q2qPkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|q2qPkg/Library/PlatformPei/PlatformPeiLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000         # Starting address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x300000000        # 12GB Size

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|2208
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|1768
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

  # UART
  gQcomTokenSpaceGuid.PcdDebugUartPortBase|0x98c000

  # Device Info
  gQcomTokenSpaceGuid.PcdSmbiosSystemVendor|"Samsung"
  gQcomTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Z Fold 3"
  gQcomTokenSpaceGuid.PcdSmbiosSystemRetailModel|"q2q"
  gQcomTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Z_Fold_3_q2q"
  gQcomTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Z Fold 3"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1768
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2208
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1768
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2208

!include SM8350Pkg/SM8350.dsc.inc