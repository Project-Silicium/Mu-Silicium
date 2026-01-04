  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/peridot/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/peridot/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/peridot/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/peridot/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/peridot/QcomPkg/Drivers/PsStateDxe/PsStateDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/peridot/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/peridot/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/peridot/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  #INF Binaries/peridot/QcomPkg/Drivers/RscDxe/RscDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/BatterySecretDxe/ac_slg/OneWireDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/peridot/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/peridot/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  #INF Binaries/peridot/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/peridot/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/peridot/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/Smp2pDxe/Smp2pDxe.inf
  #INF Binaries/peridot/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/peridot/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/peridot/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SSRDxe/SSRDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/ParserDxe/ParserEnhancedDxe.inf
  INF Binaries/peridot/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
