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
  PLATFORM_NAME                  = gts8
  PLATFORM_GUID                  = 539f03c6-9ef9-4b7d-b7b8-455abe2bcc2e
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/gts8Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = gts8Pkg/gts8.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0
  USE_UART                       = 0

[LibraryClasses.common]
  PlatformMemoryMapLib|gts8Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|gts8Pkg/Library/PlatformPei/PlatformPeiLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000         # Starting address
!if $(RAM_SIZE) == 12
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x300000000        # 12GB Size
!else
!if $(RAM_SIZE) == 8
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000        # 8GB Size
!else
!error "Invaild RAM Size! Use 12 or 8."
!endif
!endif

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0xA7600000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1600
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|2560
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

  # UART
  gQcomTokenSpaceGuid.PcdDebugUartPortBase|0x99c000

  # Device Info
  gQcomTokenSpaceGuid.PcdSmbiosSystemVendor|"Samsung"
  gQcomTokenSpaceGuid.PcdSmbiosSystemModel|"Galaxy Tab S8 5G"
  gQcomTokenSpaceGuid.PcdSmbiosSystemRetailModel|"gts8"
  gQcomTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Galaxy_Tab_S8_5G_gts8"
  gQcomTokenSpaceGuid.PcdSmbiosBoardModel|"Galaxy Tab S8 5G"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1600
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2560

!include SM8450Pkg/SM8450.dsc.inc
