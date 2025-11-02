  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  #INF Binaries/poplar/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/poplar/QcomPkg/Drivers/MdtpDxe/MdtpDxe.inf

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

  INF Binaries/poplar/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/SdccDxe/SdccDxe.inf

  # Disabled to Prevent UFS Wipe from the OSs
  #INF Binaries/poplar/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/poplar/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/poplar/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/poplar/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/poplar/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/MpPowerDxe/MpPowerDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/QdssDxe/QdssDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/MazeDetDxe/MazeDetDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/SobaDxe/SobaDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/XHwresetDxe/XHwresetDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/ResetCfgDxe/ResetCfgDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/XResetReasonDxe/XResetReasonDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/XOemCertDxe/XOemCertDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/StartFlagDxe/StartFlagDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/BootCounterDxe/BootCounterDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/XBootPALDxe/XBootPALDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/XDeviceTreeDxe/XDeviceTreeDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/TaDxe/TaDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/poplar/SomcPkg/Drivers/XDisplayDxe/XDisplayDxe.inf
!endif
