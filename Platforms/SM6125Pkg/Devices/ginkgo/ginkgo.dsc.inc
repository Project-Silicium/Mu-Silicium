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

[LibraryClasses.common]
  PlatformMemoryMapLib|SM6125Pkg/Devices/ginkgo/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

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

  # Simple Init
  gSimpleInitTokenSpaceGuid.PcdGuiDefaultDPI|320

  # Device Info
  gSM6125TokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi"
  gSM6125TokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Note 8/8T"
  gSM6125TokenSpaceGuid.PcdSmbiosSystemRetailModel|"ginkgo"
  gSM6125TokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_Note_8/8T_ginkgo"
  gSM6125TokenSpaceGuid.PcdSmbiosBoardModel|"Redmi Note 8/8T"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150
