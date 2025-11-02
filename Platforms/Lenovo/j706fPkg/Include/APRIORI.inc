APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/j706f/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/j706f/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/j706f/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/j706f/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/j706f/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/j706f/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/j706f/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/j706f/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
