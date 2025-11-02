  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/spes/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/spes/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/spes/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/spes/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/spes/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  # Use Binary Version of these two Drivers to Make UFS work correct.
  INF Binaries/spes/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/spes/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/spes/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/spes/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/MinidumpTADxe/MinidumpSMEMTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/spes/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/spes/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/spes/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/spes/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/spes/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/spes/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/spes/QcomPkg/SocPkg/DivarPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  #INF Binaries/spes/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/spes/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
