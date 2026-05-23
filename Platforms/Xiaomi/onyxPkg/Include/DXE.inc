  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/onyx/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/onyx/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/onyx/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/onyx/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/onyx/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/onyx/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/QupDxe/QupDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ExFgDxe/ExFg.inf
  INF Binaries/onyx/QcomPkg/Drivers/ExCpDxe/ExCp.inf
  INF Binaries/onyx/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/onyx/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  #INF Binaries/onyx/QcomPkg/Drivers/RscDxe/RscDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/BatterySecretDxe/ac_slg/OneWireDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/BatterySecretDxe/maxim_ds28e30/Ds28e30Dxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/onyx/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/onyx/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/onyx/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/onyx/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/Smp2pDxe/Smp2pDxe.inf
  #INF Binaries/onyx/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/onyx/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/onyx/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SSRDxe/SSRDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/ParserDxe/ParserEnhancedDxe.inf
  INF Binaries/onyx/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf

