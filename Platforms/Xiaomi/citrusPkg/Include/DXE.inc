  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/citrus/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/citrus/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/citrus/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/citrus/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/citrus/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/citrus/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/citrus/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
