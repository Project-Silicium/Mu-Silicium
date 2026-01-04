APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/equuleus/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf

  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/equuleus/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/equuleus/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/equuleus/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/equuleus/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/equuleus/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/equuleus/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/equuleus/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
}
