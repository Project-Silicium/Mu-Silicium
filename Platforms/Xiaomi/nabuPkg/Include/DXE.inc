  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/nabu/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/nabu/MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/nabu/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/nabu/QcomPkg/Drivers/PdcDxe/PdcDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/nabu/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/nabu/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/nabu/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/nabu/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/nabu/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/nabu/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
