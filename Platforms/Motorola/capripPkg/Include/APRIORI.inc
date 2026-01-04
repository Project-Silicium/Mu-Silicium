APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/caprip/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  INF Binaries/caprip/MbmPkg/Core/Drivers/MotUtagsDxe/MotUtagsDxe.inf
  INF Binaries/caprip/MbmPkg/Core/Drivers/MotUtagsDictDxe/MotUtagsDictDxe.inf

  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/caprip/MbmPkg/Core/Drivers/MotBootFlagsDxe/MotBootFlagsDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/caprip/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/caprip/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/caprip/MbmPkg/Core/Drivers/MotStorageDxe/MotStorageDxe.inf
  INF Binaries/caprip/MbmPkg/Core/Drivers/MotHwIdDxe/MotHwIdDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/caprip/MbmPkg/Core/Drivers/BacklightDxe/BacklightDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/caprip/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/caprip/QcomPkg/Drivers/PILDxe/PILDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/caprip/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/caprip/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/caprip/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
