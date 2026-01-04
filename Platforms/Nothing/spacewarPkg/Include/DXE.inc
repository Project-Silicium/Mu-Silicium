  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/spacewar/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/spacewar/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/spacewar/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/spacewar/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/spacewar/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/spacewar/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/HWIODxe/HWIODxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  
  #INF Binaries/spacewar/QcomPkg/Drivers/BootloaderLoggingDxe/BootloaderLoggingDxe.inf
  
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/spacewar/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  
  INF Binaries/spacewar/QcomPkg/Drivers/DeviceInfoDxe/DeviceInfoDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/spacewar/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/spacewar/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/SoftSKUDxe/SoftSKUDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/spacewar/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/spacewar/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/spacewar/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/spacewar/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
