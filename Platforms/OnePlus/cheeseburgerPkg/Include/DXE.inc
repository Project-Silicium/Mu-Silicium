  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  #INF Binaries/cheeseburger/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/MdtpDxe/MdtpDxe.inf

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

  INF Binaries/cheeseburger/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/cheeseburger/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/cheeseburger/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/cheeseburger/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/SdccDxe/SdccDxe.inf

  INF Binaries/cheeseburger/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF Binaries/cheeseburger/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/cheeseburger/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/cheeseburger/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/MpPowerDxe/MpPowerDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/cheeseburger/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/cheeseburger/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
