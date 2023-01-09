  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  #INF Binaries/lilac/TzDxe/TzDxe.inf
  #INF Binaries/lilac/MdtpDxe/MdtpDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  INF Binaries/lilac/ResetRuntimeDxe/ResetRuntimeDxe.inf

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/lilac/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  #INF Binaries/lilac/FontDxe/FontDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
    
  #INF Binaries/lilac/ChipInfo/ChipInfo.inf
  #INF Binaries/lilac/GlinkDxe/GlinkDxe.inf
  #INF Binaries/lilac/SmemDxe/SmemDxe.inf
  
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf

  #INF Binaries/lilac/ULogDxe/ULogDxe.inf
  #INF Binaries/lilac/NpaDxe/NpaDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf

  #INF Binaries/lilac/DALSys/DALSys.inf
  #INF Binaries/lilac/ClockDxe/ClockDxe.inf
  #INF Binaries/lilac/HWIODxeDriver/HWIODxeDriver.inf
  #INF Binaries/lilac/I2C/I2C.inf
  #INF Binaries/lilac/SPI/SPI.inf
  #INF Binaries/lilac/SPMI/SPMI.inf
  #INF Binaries/lilac/SdccDxe/SdccDxe.inf
  #INF Binaries/lilac/UFSDxe/UFSDxe.inf
  
  #INF Binaries/lilac/DALTLMM/DALTLMM.inf

!if $(USE_DISPLAYDXE) == FALSE
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf

  #INF Binaries/lilac/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  #INF Binaries/lilac/ButtonsDxe/ButtonsDxe.inf
  #INF Binaries/lilac/PmicDxe/PmicDxe.inf

  INF QcomPkg/Drivers/ColorbarsDxe/ColorbarsDxe.inf
  INF MdeModulePkg/Universal/TimestampDxe/TimestampDxe.inf

  #INF Binaries/lilac/ChargerExDxe/ChargerExDxe.inf
  #INF Binaries/lilac/QcomChargerDxeLA/QcomChargerDxeLA.inf
  #INF Binaries/lilac/AdcDxe/AdcDxe.inf
  #INF Binaries/lilac/TsensDxe/TsensDxe.inf
  #INF Binaries/lilac/LimitsDxe/LimitsDxe.inf
  #INF Binaries/lilac/MpPowerDxe/MpPowerDxe.inf
  #INF Binaries/lilac/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  #INF Binaries/lilac/UsbMsdDxe/UsbMsdDxe.inf
  #INF Binaries/lilac/UsbDeviceDxe/UsbDeviceDxe.inf

  INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  #INF Binaries/lilac/UsbConfigDxe/UsbConfigDxe.inf
  #INF Binaries/lilac/HashDxe/HashDxe.inf
  #INF Binaries/lilac/RngDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf

  #INF Binaries/lilac/QdssDxe/QdssDxe.inf
  #INF Binaries/lilac/QcomChargerApp/QcomChargerApp.inf
