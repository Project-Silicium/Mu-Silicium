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
  PlatformMemoryMapLib|MSM8998Pkg/Devices/lilac/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000         # Starting address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000        # 4GB Size

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|1280
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|24
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|720
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|1280

  # Device Info
  gQcomTokenSpaceGuid.PcdDeviceVendor|"Sony"
  gQcomTokenSpaceGuid.PcdDeviceProduct|"Xperia XZ1 Compact"
  gQcomTokenSpaceGuid.PcdDeviceCodeName|"lilac"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1280
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1280
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150

[Components.common]
  # Platform Drivers
  #Binaries/lilac/TzDxe/TzDxe.inf
  #Binaries/lilac/MdtpDxe/MdtpDxe.inf
  #Binaries/lilac/ResetRuntimeDxe/ResetRuntimeDxe.inf
  #Binaries/lilac/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  #Binaries/lilac/FontDxe/FontDxe.inf
  #Binaries/lilac/ChipInfo/ChipInfo.inf
  #Binaries/lilac/GlinkDxe/GlinkDxe.inf
  #Binaries/lilac/SmemDxe/SmemDxe.inf
  #Binaries/lilac/ULogDxe/ULogDxe.inf
  #Binaries/lilac/NpaDxe/NpaDxe.inf
  #Binaries/lilac/DALSys/DALSys.inf
  #Binaries/lilac/ClockDxe/ClockDxe.inf
  #Binaries/lilac/HWIODxeDriver/HWIODxeDriver.inf
  #Binaries/lilac/I2C/I2C.inf
  #Binaries/lilac/SPI/SPI.inf
  #Binaries/lilac/SPMI/SPMI.inf
  #Binaries/lilac/SdccDxe/SdccDxe.inf
  #Binaries/lilac/UFSDxe/UFSDxe.inf
  #Binaries/lilac/DALTLMM/DALTLMM.inf
  #Binaries/lilac/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  #Binaries/lilac/ButtonsDxe/ButtonsDxe.inf
  #Binaries/lilac/PmicDxe/PmicDxe.inf
  #Binaries/lilac/ChargerExDxe/ChargerExDxe.inf
  #Binaries/lilac/QcomChargerDxeLA/QcomChargerDxeLA.inf
  #Binaries/lilac/AdcDxe/AdcDxe.inf
  #Binaries/lilac/TsensDxe/TsensDxe.inf
  #Binaries/lilac/LimitsDxe/LimitsDxe.inf
  #Binaries/lilac/MpPowerDxe/MpPowerDxe.inf
  #Binaries/lilac/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  #Binaries/lilac/UsbMsdDxe/UsbMsdDxe.inf
  #Binaries/lilac/UsbDeviceDxe/UsbDeviceDxe.inf
  #Binaries/lilac/UsbConfigDxe/UsbConfigDxe.inf
  #Binaries/lilac/HashDxe/HashDxe.inf
  #Binaries/lilac/RngDxe/RngDxe.inf
  #Binaries/lilac/QdssDxe/QdssDxe.inf
  #Binaries/lilac/QcomChargerApp/QcomChargerApp.inf
