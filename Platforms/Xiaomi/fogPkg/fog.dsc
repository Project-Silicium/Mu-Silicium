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
  PLATFORM_NAME                  = fog
  PLATFORM_GUID                  = c62242f8-e275-4150-b6f3-1cc4fac6425c
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/fogPkg-$(ARCH)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = fogPkg/fog.fdf
  DISPLAY_USES_RGBA              = 0
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1
  USE_UART                       = 0

[LibraryClasses.common]
  PlatformMemoryMapLib|fogPkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000          # Starting address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000         # 4 GB Size

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Statzar"

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF00000
 
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi 10C"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"fog"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_10C_fog"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi 10C"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1650
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1650
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1650
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|206
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|206

!include SM6225Pkg/SM6225Pkg.dsc.inc
