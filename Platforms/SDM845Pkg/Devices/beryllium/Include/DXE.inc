  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  
  INF Binaries/beryllium/TzDxe/TzDxe.inf
  
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  INF Binaries/beryllium/ResetRuntimeDxe/ResetRuntimeDxe.inf

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/beryllium/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/beryllium/FontDxe/FontDxe.inf
  INF Binaries/beryllium/EnvDxe/EnvDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

  INF Binaries/beryllium/ChipInfo/ChipInfo.inf
  INF Binaries/beryllium/SmemDxe/SmemDxe.inf
  
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf

  INF Binaries/beryllium/ULogDxe/ULogDxe.inf
  INF Binaries/beryllium/NpaDxe/NpaDxe.inf
  INF Binaries/beryllium/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/beryllium/RpmhDxe/RpmhDxe.inf
!if $(USE_DISPLAYDXE) == TRUE
  INF Binaries/beryllium/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/beryllium/PdcDxe/PdcDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf

  INF Binaries/beryllium/DALSys/DALSys.inf
  INF Binaries/beryllium/ClockDxe/ClockDxe.inf
  INF Binaries/beryllium/HALIOMMU/HALIOMMU.inf
  INF Binaries/beryllium/HWIODxeDriver/HWIODxeDriver.inf
  INF Binaries/beryllium/GpiDxe/GpiDxe.inf
  INF Binaries/beryllium/SPMI/SPMI.inf
  INF Binaries/beryllium/SdccDxe/SdccDxe.inf
  INF Binaries/beryllium/UFSDxe/UFSDxe.inf
  INF Binaries/beryllium/DALTLMM/DALTLMM.inf

!if $(USE_DISPLAYDXE) == TRUE
  INF Binaries/beryllium/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf

  INF Binaries/beryllium/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  INF Binaries/beryllium/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/beryllium/PmicDxe/PmicDxe.inf
  
  INF QcomPkg/Drivers/ColorbarsDxe/ColorbarsDxe.inf
  INF MdeModulePkg/Universal/TimestampDxe/TimestampDxe.inf

  #INF Binaries/beryllium/ChargerExDxe/ChargerExDxe.inf
  #INF Binaries/beryllium/QcomChargerDxeLA/QcomChargerDxeLA.inf
  #INF Binaries/beryllium/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/beryllium/AdcDxe/AdcDxe.inf
  INF Binaries/beryllium/TsensDxe/TsensDxe.inf
  INF Binaries/beryllium/LimitsDxe/LimitsDxe.inf
  INF Binaries/beryllium/DDRInfoDxe/DDRInfoDxe.inf
  #INF Binaries/beryllium/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  #INF Binaries/beryllium/UsbMsdDxe/UsbMsdDxe.inf
  #INF Binaries/beryllium/UsbDeviceDxe/UsbDeviceDxe.inf
  
  #INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  #INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  #INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  #INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  
  #INF Binaries/beryllium/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/beryllium/HashDxe/HashDxe.inf
  INF Binaries/beryllium/RngDxe/RngDxe.inf
  
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf

  #INF Binaries/beryllium/QcomChargerApp/QcomChargerApp.inf
