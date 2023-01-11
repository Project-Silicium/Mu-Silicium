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
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|2340
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|2340

  # Device Info
  gQcomTokenSpaceGuid.PcdDeviceVendor|"Xiaomi"
  gQcomTokenSpaceGuid.PcdDeviceProduct|"Redmi Note 8/8T"
  gQcomTokenSpaceGuid.PcdDeviceCodeName|"ginkgo"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150

[Components.common]
  # Platform Drivers
  Binaries/ginkgo/ScmDxe/ScmDxe.inf
  Binaries/ginkgo/TzDxe/TzDxe.inf
  Binaries/ginkgo/ResetRuntimeDxe/ResetRuntimeDxe.inf
  Binaries/ginkgo/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  Binaries/ginkgo/FontDxe/FontDxe.inf
  Binaries/ginkgo/QcomWDogDxe/QcomWDogDxe.inf
  Binaries/ginkgo/QcomMpmTimerDxe/QcomMpmTimerDxe.inf
  Binaries/ginkgo/ChipInfo/ChipInfo.inf
  Binaries/ginkgo/GlinkDxe/GlinkDxe.inf
  Binaries/ginkgo/SmemDxe/SmemDxe.inf
  Binaries/ginkgo/ULogDxe/ULogDxe.inf
  Binaries/ginkgo/NpaDxe/NpaDxe.inf
  Binaries/ginkgo/DALSys/DALSys.inf
  Binaries/ginkgo/ClockDxe/ClockDxe.inf
  Binaries/ginkgo/HALIOMMU/HALIOMMU.inf
  Binaries/ginkgo/HWIODxeDriver/HWIODxeDriver.inf
  Binaries/ginkgo/GpiDxe/GpiDxe.inf
  Binaries/ginkgo/I2C/I2C.inf
  Binaries/ginkgo/SPMI/SPMI.inf
  Binaries/ginkgo/SdccDxe/SdccDxe.inf
  Binaries/ginkgo/UFSDxe/UFSDxe.inf
  Binaries/ginkgo/DALTLMM/DALTLMM.inf
!if $(USE_DISPLAYDXE) == TRUE
  Binaries/ginkgo/DisplayDxe/DisplayDxe.inf
!endif
  Binaries/ginkgo/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  Binaries/ginkgo/ButtonsDxe/ButtonsDxe.inf
  Binaries/ginkgo/PmicDxe/PmicDxe.inf
  Binaries/ginkgo/ChargerExDxe/ChargerExDxe.inf
  Binaries/ginkgo/QcomChargerDxeLA/QcomChargerDxeLA.inf
  Binaries/ginkgo/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  Binaries/ginkgo/AdcDxe/AdcDxe.inf
  Binaries/ginkgo/TsensDxe/TsensDxe.inf
  Binaries/ginkgo/DDRInfoDxe/DDRInfoDxe.inf
  Binaries/ginkgo/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  Binaries/ginkgo/UsbMsdDxe/UsbMsdDxe.inf
  Binaries/ginkgo/UsbDeviceDxe/UsbDeviceDxe.inf
  Binaries/ginkgo/UsbConfigDxe/UsbConfigDxe.inf
  Binaries/ginkgo/HashDxe/HashDxe.inf
  Binaries/ginkgo/CipherDxe/CipherDxe.inf
  Binaries/ginkgo/RngDxe/RngDxe.inf
  Binaries/ginkgo/PILDxe/PILDxe.inf
  Binaries/ginkgo/QcomChargerApp/QcomChargerApp.inf
  Binaries/ginkgo/ShmBridgeDxe/ShmBridgeDxe.inf
