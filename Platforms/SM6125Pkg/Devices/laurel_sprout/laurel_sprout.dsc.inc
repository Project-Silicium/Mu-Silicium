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

[Defines]
  # Platform Defines
  USE_DISPLAYDXE                 = TRUE
  
[LibraryClasses.common]
  PlatformMemoryMapLib|SM6125Pkg/Devices/ginkgo/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000         # Starting address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000        # 4GB Size

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|1560
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|720
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|1560

  # Device Info
  gSM6125TokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi"
  gSM6125TokenSpaceGuid.PcdSmbiosSystemModel|"Mi A3"
  gSM6125TokenSpaceGuid.PcdSmbiosSystemRetailModel|"laurel_sprout"
  gSM6125TokenSpaceGuid.PcdSmbiosSystemRetailSku|"Mi_A3_laurel_sprout"
  gSM6125TokenSpaceGuid.PcdSmbiosBoardModel|"Mi A3"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1560
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150

[Components.common]
  # Platform Drivers
  Binaries/laurel_sprout/ScmDxe/ScmDxe.inf
  Binaries/laurel_sprout/TzDxe/TzDxe.inf
  Binaries/laurel_sprout/ResetRuntimeDxe/ResetRuntimeDxe.inf
  Binaries/laurel_sprout/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  Binaries/laurel_sprout/FontDxe/FontDxe.inf
  Binaries/laurel_sprout/QcomWDogDxe/QcomWDogDxe.inf
  Binaries/laurel_sprout/QcomMpmTimerDxe/QcomMpmTimerDxe.inf
  Binaries/laurel_sprout/ChipInfo/ChipInfo.inf
  Binaries/laurel_sprout/GlinkDxe/GlinkDxe.inf
  Binaries/laurel_sprout/SmemDxe/SmemDxe.inf
  Binaries/laurel_sprout/ULogDxe/ULogDxe.inf
  Binaries/laurel_sprout/NpaDxe/NpaDxe.inf
  Binaries/laurel_sprout/DALSys/DALSys.inf
  Binaries/laurel_sprout/ClockDxe/ClockDxe.inf
  Binaries/laurel_sprout/HALIOMMU/HALIOMMU.inf
  Binaries/laurel_sprout/HWIODxeDriver/HWIODxeDriver.inf
  Binaries/laurel_sprout/GpiDxe/GpiDxe.inf
  Binaries/laurel_sprout/I2C/I2C.inf
  Binaries/laurel_sprout/SPMI/SPMI.inf
  Binaries/laurel_sprout/SdccDxe/SdccDxe.inf
  Binaries/laurel_sprout/UFSDxe/UFSDxe.inf
  Binaries/laurel_sprout/DALTLMM/DALTLMM.inf
!if $(USE_DISPLAYDXE) == TRUE
  Binaries/laurel_sprout/DisplayDxe/DisplayDxe.inf
!endif
  Binaries/laurel_sprout/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  Binaries/laurel_sprout/ButtonsDxe/ButtonsDxe.inf
  Binaries/laurel_sprout/PmicDxe/PmicDxe.inf
  Binaries/laurel_sprout/ChargerExDxe/ChargerExDxe.inf
  Binaries/laurel_sprout/QcomChargerDxeLA/QcomChargerDxeLA.inf
  Binaries/laurel_sprout/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  Binaries/laurel_sprout/AdcDxe/AdcDxe.inf
  Binaries/laurel_sprout/TsensDxe/TsensDxe.inf
  Binaries/laurel_sprout/DDRInfoDxe/DDRInfoDxe.inf
  Binaries/laurel_sprout/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  Binaries/laurel_sprout/UsbMsdDxe/UsbMsdDxe.inf
  Binaries/laurel_sprout/UsbDeviceDxe/UsbDeviceDxe.inf
  Binaries/laurel_sprout/UsbConfigDxe/UsbConfigDxe.inf
  Binaries/laurel_sprout/HashDxe/HashDxe.inf
  Binaries/laurel_sprout/CipherDxe/CipherDxe.inf
  Binaries/laurel_sprout/RngDxe/RngDxe.inf
  Binaries/laurel_sprout/PILDxe/PILDxe.inf
  Binaries/laurel_sprout/QcomChargerApp/QcomChargerApp.inf
  Binaries/laurel_sprout/ShmBridgeDxe/ShmBridgeDxe.inf
