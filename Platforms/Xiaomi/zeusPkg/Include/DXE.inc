  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  
  INF Binaries/zeus/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/zeus/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/zeus/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  
  INF Binaries/zeus/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/zeus/QcomPkg/Drivers/ExLEDDxe/ExLEDDxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/VcsDxe/VcsDxe.inf  
  INF Binaries/zeus/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/zeus/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/zeus/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  #INF Binaries/zeus/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/zeus/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/zeus/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  #INF Binaries/zeus/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/zeus/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/zeus/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/UCDxe/UCDxe.inf
  #INF Binaries/zeus/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  #INF Binaries/zeus/QcomPkg/Drivers/VibratorDxe/VibratorDxe.inf
