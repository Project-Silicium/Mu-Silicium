  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/laurel_sprout/ScmDxe/ScmDxe.inf
  INF Binaries/laurel_sprout/TzDxe/TzDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  INF Binaries/laurel_sprout/ResetRuntimeDxe/ResetRuntimeDxe.inf

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/laurel_sprout/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/laurel_sprout/FontDxe/FontDxe.inf
  INF Binaries/laurel_sprout/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/laurel_sprout/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
    
  INF Binaries/laurel_sprout/ChipInfo/ChipInfo.inf
  INF Binaries/laurel_sprout/GlinkDxe/GlinkDxe.inf
  INF Binaries/laurel_sprout/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf

  INF Binaries/laurel_sprout/ULogDxe/ULogDxe.inf
  INF Binaries/laurel_sprout/NpaDxe/NpaDxe.inf
  
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf
  
  INF Binaries/laurel_sprout/DALSys/DALSys.inf
  #INF Binaries/laurel_sprout/ClockDxe/ClockDxe.inf
  INF Binaries/laurel_sprout/HALIOMMU/HALIOMMU.inf
  INF Binaries/laurel_sprout/HWIODxeDriver/HWIODxeDriver.inf
  INF Binaries/laurel_sprout/GpiDxe/GpiDxe.inf
  INF Binaries/laurel_sprout/I2C/I2C.inf
  INF Binaries/laurel_sprout/SPMI/SPMI.inf
  #INF Binaries/laurel_sprout/SdccDxe/SdccDxe.inf
  #INF Binaries/laurel_sprout/UFSDxe/UFSDxe.inf
  INF Binaries/laurel_sprout/DALTLMM/DALTLMM.inf
  
!if $(USE_DISPLAYDXE) == TRUE
  INF Binaries/laurel_sprout/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf
  
  INF Binaries/laurel_sprout/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  INF Binaries/laurel_sprout/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/laurel_sprout/PmicDxe/PmicDxe.inf

  INF QcomPkg/Drivers/ColorbarsDxe/ColorbarsDxe.inf
  INF MdeModulePkg/Universal/TimestampDxe/TimestampDxe.inf

  INF Binaries/laurel_sprout/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/laurel_sprout/QcomChargerDxeLA/QcomChargerDxeLA.inf
  INF Binaries/laurel_sprout/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/laurel_sprout/AdcDxe/AdcDxe.inf
  INF Binaries/laurel_sprout/TsensDxe/TsensDxe.inf
  INF Binaries/laurel_sprout/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/laurel_sprout/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/laurel_sprout/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/laurel_sprout/UsbDeviceDxe/UsbDeviceDxe.inf

  INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/laurel_sprout/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/laurel_sprout/HashDxe/HashDxe.inf
  INF Binaries/laurel_sprout/CipherDxe/CipherDxe.inf
  INF Binaries/laurel_sprout/RngDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf

  #INF Binaries/laurel_sprout/PILDxe/PILDxe.inf
  INF Binaries/laurel_sprout/QcomChargerApp/QcomChargerApp.inf
  INF Binaries/laurel_sprout/ShmBridgeDxe/ShmBridgeDxe.inf
