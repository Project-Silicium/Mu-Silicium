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
  SWITCHSLOT_SUPPORT             = TRUE

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000         # Starting address
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000        # 8GB Size

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x9FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x9FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000      # 256K stack

  # Simple FrameBuffer
  gQcomTokenSpaceGuid.PcdMipiFrameBufferWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferHeight|2400
  gQcomTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleWidth|1080
  gQcomTokenSpaceGuid.PcdMipiFrameBufferVisibleHeight|2400

  # Device Info
  gQcomTokenSpaceGuid.PcdDeviceVendor|"Xiaomi"
  gQcomTokenSpaceGuid.PcdDeviceProduct|"Xiaomi 11T Pro"
  gQcomTokenSpaceGuid.PcdDeviceCodeName|"vili"

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|2400
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|1080
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|2400
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|150
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|150

[Components.common]
  # Platform Drivers
  Binaries/vili/ArmTimerDxe/ArmTimerDxe.inf
  Binaries/vili/ShmBridgeDxeLA/ShmBridgeDxeLA.inf
  Binaries/vili/ScmDxeLA/ScmDxeLA.inf
  Binaries/vili/TzDxeLA/TzDxeLA.inf
  Binaries/vili/FontDxe/FontDxe.inf
  Binaries/vili/QcomWDogDxe/QcomWDogDxe.inf
  Binaries/vili/ChipInfo/ChipInfo.inf
  Binaries/vili/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  Binaries/vili/ProjectInfoDxeDriver/ProjectInfoDxeDriver.inf
  Binaries/vili/DALSys/DALSys.inf
  Binaries/vili/HALIOMMU/HALIOMMU.inf
  Binaries/vili/HWIODxeDriver/HWIODxeDriver.inf
  Binaries/vili/UFSDxe/UFSDxe.inf
  Binaries/vili/CmdDbDxe/CmdDbDxe.inf
  Binaries/vili/PwrUtilsDxe/PwrUtilsDxe.inf
  Binaries/vili/RpmhDxe/RpmhDxe.inf
  Binaries/vili/NpaDxe/NpaDxe.inf
  Binaries/vili/ULogDxe/ULogDxe.inf
  Binaries/vili/VcsDxe/VcsDxe.inf
  Binaries/vili/ClockDxe/ClockDxe.inf
  Binaries/vili/ICBDxe/ICBDxe.inf
  Binaries/vili/SmemDxe/SmemDxe.inf
  Binaries/vili/GpiDxe/GpiDxe.inf
  Binaries/vili/I2C/I2C.inf
  Binaries/vili/SPI/SPI.inf
  Binaries/vili/SPMI/SPMI.inf
  Binaries/vili/PmicDxe/PmicDxe.inf
  Binaries/vili/SdccDxe/SdccDxe.inf
  Binaries/vili/DALTLMM/DALTLMM.inf
  Binaries/vili/FeatureEnablerDxe/FeatureEnablerDxe.inf
  Binaries/vili/MinidumpTADxe/MinidumpTADxe.inf
  Binaries/vili/ButtonsDxe/ButtonsDxe.inf
  Binaries/vili/ChargerExDxe/ChargerExDxe.inf
  Binaries/vili/IPCCDxe/IPCCDxe.inf
  Binaries/vili/GlinkDxe/GlinkDxe.inf
  Binaries/vili/PmicGlinkDxe/PmicGlinkDxe.inf
  Binaries/vili/QcomChargerDxeLA/QcomChargerDxeLA.inf
  Binaries/vili/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  Binaries/vili/TsensDxe/TsensDxe.inf
  Binaries/vili/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  Binaries/vili/DDRInfoDxe/DDRInfoDxe.inf
  Binaries/vili/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  Binaries/vili/XhciPciEmulation/XhciPciEmulation.inf
  Binaries/vili/UsbMsdDxe/UsbMsdDxe.inf
  Binaries/vili/UsbDeviceDxe/UsbDeviceDxe.inf
  Binaries/vili/UsbConfigDxe/UsbConfigDxe.inf
  Binaries/vili/UsbInitDxe/UsbInitDxe.inf
  Binaries/vili/UCDxe/UCDxe.inf
  Binaries/vili/RngDxe/RngDxe.inf
  Binaries/vili/PILDxe/PILDxe.inf
  Binaries/vili/PILProxyDxe/PILProxyDxe.inf
  Binaries/vili/SPSSDxe/SPSSDxe.inf
  Binaries/vili/FvDxe/FvDxe.inf
  Binaries/vili/ParserDxe/ParserDxe.inf
  Binaries/vili/QcomChargerApp/QcomChargerApp.inf
  Binaries/vili/XhciDxe/XhciDxe.inf
  Binaries/vili/UsbBusDxe/UsbBusDxe.inf
  Binaries/vili/UsbKbDxe/UsbKbDxe.inf
  Binaries/vili/UsbMassStorageDxe/UsbMassStorageDxe.inf
  Binaries/vili/ResetRuntimeDxe/ResetRuntimeDxe.inf
