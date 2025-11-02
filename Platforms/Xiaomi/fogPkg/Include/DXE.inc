  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/fog/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/fog/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/fog/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/fog/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  
  INF Binaries/fog/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  # Use Binary Version of these two Drivers to Make UFS work correct.
  INF Binaries/fog/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/fog/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/fog/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/fog/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/MinidumpTADxe/MinidumpSMEMTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/fog/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/fog/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/fog/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/fog/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/fog/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/fog/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/fog/QcomPkg/SocPkg/DivarPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/fog/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/fog/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
