  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/renoir/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/renoir/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/renoir/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/renoir/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/renoir/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/renoir/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/HWIODxe/HWIODxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/renoir/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  #INF Binaries/renoir/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/renoir/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/renoir/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/renoir/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/renoir/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/renoir/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/renoir/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/renoir/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/renoir/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
