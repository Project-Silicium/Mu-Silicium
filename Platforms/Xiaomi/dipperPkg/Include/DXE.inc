  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/dipper/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/dipper/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/dipper/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/dipper/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/dipper/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/dipper/QcomPkg/Drivers/PdcDxe/PdcDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/dipper/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/dipper/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/dipper/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/dipper/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/dipper/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  #INF Binaries/dipper/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/dipper/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/dipper/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/dipper/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
