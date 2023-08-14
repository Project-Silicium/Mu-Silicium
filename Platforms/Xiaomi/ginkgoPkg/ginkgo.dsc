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
  PLATFORM_NAME                  = ginkgo
  PLATFORM_GUID                  = 630d1379-7d3f-4511-a6f9-578abbc07518
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/ginkgoPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = ginkgoPkg/ginkgo.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0
  USE_UART                       = 0

[LibraryClasses.common]
  PlatformMemoryMapLib|ginkgoPkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|ginkgoPkg/Library/PlatformPei/PlatformPeiLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000         # Starting address
!if $(RAM_SIZE) == 6
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x180000000        # 6GB Size
!else
!if $(RAM_SIZE) == 4
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000        # 4GB Size
!else
!if $(RAM_SIZE) == 3
  gArmTokenSpaceGuid.PcdSystemMemorySize|0xC0000000         # 3GB Size
!else
!error "Invaild Mem Size! Use 6, 4 or 3."
!endif
!endif
!endif

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|2340
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

  # UART
  gQcomTokenSpaceGuid.PcdDebugUartPortBase|0x4a90000

  # Device Info
  gQcomTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi"
  gQcomTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 8/8T"
  gQcomTokenSpaceGuid.PcdSmbiosSystemRetailModel|"ginkgo"
  gQcomTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_Note_8/8T_ginkgo"
  gQcomTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi Note 8/8T"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2340

!include SM6125Pkg/SM6125.dsc.inc
