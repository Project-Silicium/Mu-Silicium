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
  PlatformMemoryMapLib|SM6115Pkg/Devices/lime/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

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
  gSM6115TokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi"
  gSM6115TokenSpaceGuid.PcdSmbiosSystemModel|"Redmi 9T"
  gSM6115TokenSpaceGuid.PcdSmbiosSystemRetailModel|"lime"
  gSM6115TokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_9T_lime"
  gSM6115TokenSpaceGuid.PcdSmbiosBoardModel|"Redmi 9T"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2340
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150

[Components.common]
  # Platform Drivers
  Binaries/lime/ScmDxe/ScmDxe.inf
  Binaries/lime/TzDxe/TzDxe.inf
  Binaries/lime/FontDxe/FontDxe.inf
  Binaries/lime/QcomWDogDxe/QcomWDogDxe.inf
  Binaries/lime/QcomMpmTimerDxe/QcomMpmTimerDxe.inf 
  Binaries/lime/ChipInfo/ChipInfo.inf
  Binaries/lime/GlinkDxe/GlinkDxe.inf
  Binaries/lime/SmemDxe/SmemDxe.inf
  Binaries/lime/ULogDxe/ULogDxe.inf
  Binaries/lime/NpaDxe/NpaDxe.inf
  Binaries/lime/DALSys/DALSys.inf
  Binaries/lime/VcsDxe/VcsDxe.inf
  Binaries/lime/ClockDxe/ClockDxe.inf
  Binaries/lime/HALIOMMU/HALIOMMU.inf
  Binaries/lime/HWIODxeDriver/HWIODxeDriver.inf
  Binaries/lime/GpiDxe/GpiDxe.inf
  Binaries/lime/I2C/I2C.inf
  Binaries/lime/SPMI/SPMI.inf
  Binaries/lime/SdccDxe/SdccDxe.inf
  Binaries/lime/UFSDxe/UFSDxe.inf
  Binaries/lime/DALTLMM/DALTLMM.inf
  Binaries/lime/MinidumpTADxe/MinidumpTADxe.inf
!if $(USE_DISPLAYDXE) == TRUE
  Binaries/lime/DisplayDxe/DisplayDxe.inf
!endif
  Binaries/lime/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  Binaries/lime/ButtonsDxe/ButtonsDxe.inf
  Binaries/lime/PmicDxe/PmicDxe.inf
  Binaries/lime/ChargerExDxe/ChargerExDxe.inf
  Binaries/lime/QcomChargerDxeLA/QcomChargerDxeLA.inf
  Binaries/lime/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  Binaries/lime/AdcDxe/AdcDxe.inf
  Binaries/lime/TsensDxe/TsensDxe.inf
  Binaries/lime/DDRInfoDxe/DDRInfoDxe.inf
  Binaries/lime/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  Binaries/lime/UsbMsdDxe/UsbMsdDxe.inf
  Binaries/lime/UsbDeviceDxe/UsbDeviceDxe.inf
  Binaries/lime/UsbConfigDxe/UsbConfigDxe.inf
  Binaries/lime/HashDxe/HashDxe.inf
  Binaries/lime/CipherDxe/CipherDxe.inf
  Binaries/lime/MacDxe/MacDxe.inf
  Binaries/lime/RngDxe/RngDxe.inf
  Binaries/lime/PILDxe/PILDxe.inf
  Binaries/lime/QcomChargerApp/QcomChargerApp.inf
  Binaries/lime/ShmBridgeDxe/ShmBridgeDxe.inf
