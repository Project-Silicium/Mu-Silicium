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
  PlatformMemoryMapLib|SDM845Pkg/Devices/beryllium/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000         # Starting address
!if $(MEM_SIZE) == 8
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000        # 8GB Size
!else
!if $(MEM_SIZE) == 6
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x180000000        # 6GB Size
!else
!error "Invaild Mem Size! Use 6 or 8."
!endif
!endif

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|2246
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|2246

  # Device Info
  gSDM845TokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi"
  gSDM845TokenSpaceGuid.PcdSmbiosSystemModel|"Poco F1"
  gSDM845TokenSpaceGuid.PcdSmbiosSystemRetailModel|"beryllium"
  gSDM845TokenSpaceGuid.PcdSmbiosSystemRetailSku|"Poco_F1_beryllium"
  gSDM845TokenSpaceGuid.PcdSmbiosBoardModel|"Poco F1"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2246
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2246
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150

[Components.common]
  # Platform Drivers
  Binaries/beryllium/TzDxe/TzDxe.inf
  Binaries/beryllium/ResetRuntimeDxe/ResetRuntimeDxe.inf
  Binaries/beryllium/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  Binaries/beryllium/FontDxe/FontDxe.inf
  Binaries/beryllium/ChipInfo/ChipInfo.inf
  Binaries/beryllium/SmemDxe/SmemDxe.inf
  Binaries/beryllium/ULogDxe/ULogDxe.inf
  Binaries/beryllium/NpaDxe/NpaDxe.inf
  Binaries/beryllium/CmdDbDxe/CmdDbDxe.inf
  Binaries/beryllium/RpmhDxe/RpmhDxe.inf
!if $(USE_DISPLAYDXE) == TRUE
  Binaries/beryllium/CPRDxe/CPRDxe.inf
!endif
  Binaries/beryllium/PdcDxe/PdcDxe.inf
  Binaries/beryllium/DALSys/DALSys.inf
  Binaries/beryllium/ClockDxe/ClockDxe.inf
  Binaries/beryllium/HALIOMMU/HALIOMMU.inf
  Binaries/beryllium/HWIODxeDriver/HWIODxeDriver.inf
  Binaries/beryllium/GpiDxe/GpiDxe.inf
  Binaries/beryllium/SPMI/SPMI.inf
  Binaries/beryllium/SdccDxe/SdccDxe.inf
  Binaries/beryllium/UFSDxe/UFSDxe.inf
  Binaries/beryllium/DALTLMM/DALTLMM.inf
!if $(USE_DISPLAYDXE) == TRUE
  Binaries/beryllium/DisplayDxe/DisplayDxe.inf
!endif
  Binaries/beryllium/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  Binaries/beryllium/ButtonsDxe/ButtonsDxe.inf
  Binaries/beryllium/PmicDxe/PmicDxe.inf
  Binaries/beryllium/ChargerExDxe/ChargerExDxe.inf
  Binaries/beryllium/QcomChargerDxeLA/QcomChargerDxeLA.inf
  Binaries/beryllium/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  Binaries/beryllium/AdcDxe/AdcDxe.inf
  Binaries/beryllium/TsensDxe/TsensDxe.inf
  Binaries/beryllium/LimitsDxe/LimitsDxe.inf
  Binaries/beryllium/DDRInfoDxe/DDRInfoDxe.inf
  Binaries/beryllium/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  Binaries/beryllium/UsbMsdDxe/UsbMsdDxe.inf
  Binaries/beryllium/UsbDeviceDxe/UsbDeviceDxe.inf
  Binaries/beryllium/UsbConfigDxe/UsbConfigDxe.inf
  Binaries/beryllium/HashDxe/HashDxe.inf
  Binaries/beryllium/RngDxe/RngDxe.inf
  Binaries/beryllium/QcomChargerApp/QcomChargerApp.inf
