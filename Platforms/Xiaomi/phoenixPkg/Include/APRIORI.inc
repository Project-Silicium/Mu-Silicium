APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/phoenix/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/phoenix/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/phoenix/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/AW8624Dxe/AW8624Dxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/VibratorDxe/VibratorDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/phoenix/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/phoenix/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/phoenix/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/phoenix/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/phoenix/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
