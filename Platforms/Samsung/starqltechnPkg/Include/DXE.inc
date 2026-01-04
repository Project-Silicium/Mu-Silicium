  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/starqltechn/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/starqltechn/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/starqltechn/QcomPkg/Drivers/ChipInfo/ChipInfo.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/starqltechn/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/starqltechn/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/starqltechn/QcomPkg/Drivers/PdcDxe/PdcDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/starqltechn/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/HALIOMMU/HALIOMMU.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/starqltechn/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/starqltechn/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/starqltechn/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/ChgDxe/ChgDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/SubPmicDxe/SubPmicDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/DispPmicDxe/DispPmicDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/VibDxe/VibDxe.inf
  INF Binaries/starqltechn/QcomPkg/Drivers/SamsungDxe/SvcLedDxe/SvcLedDxe.inf
