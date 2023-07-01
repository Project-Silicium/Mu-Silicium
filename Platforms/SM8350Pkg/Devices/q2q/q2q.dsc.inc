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
  PlatformMemoryMapLib|SM8350Pkg/Devices/q2q/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

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

  # Simple Init
  gSimpleInitTokenSpaceGuid.PcdGuiDefaultDPI|300

!if $(USE_UART) == 1
  # UART
  gQcomTokenSpaceGuid.PcdDebugUartPortBase|0x98c000
!endif

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
