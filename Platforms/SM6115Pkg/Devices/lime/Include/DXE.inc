  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/lime/ScmDxe/ScmDxe.inf
  INF Binaries/lime/TzDxe/TzDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

  INF Binaries/lime/ResetRuntimeDxe/ResetRuntimeDxe.inf

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/lime/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/lime/FontDxe/FontDxe.inf
  INF Binaries/lime/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/lime/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
    
  INF Binaries/lime/ChipInfo/ChipInfo.inf
  INF Binaries/lime/GlinkDxe/GlinkDxe.inf
  INF Binaries/lime/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf

  INF Binaries/lime/ULogDxe/ULogDxe.inf
  INF Binaries/lime/NpaDxe/NpaDxe.inf
  
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf
  
  INF Binaries/lime/DALSys/DALSys.inf
  INF Binaries/lime/VcsDxe/VcsDxe.inf
  #INF Binaries/lime/ClockDxe/ClockDxe.inf
  INF Binaries/lime/HALIOMMU/HALIOMMU.inf
  INF Binaries/lime/HWIODxeDriver/HWIODxeDriver.inf
  INF Binaries/lime/GpiDxe/GpiDxe.inf
  INF Binaries/lime/I2C/I2C.inf
  INF Binaries/lime/SPMI/SPMI.inf
  INF Binaries/lime/SdccDxe/SdccDxe.inf
  INF Binaries/lime/UFSDxe/UFSDxe.inf
  INF Binaries/lime/DALTLMM/DALTLMM.inf
  INF Binaries/lime/MinidumpTADxe/MinidumpTADxe.inf
  
!if $(USE_DISPLAYDXE) == TRUE
  INF Binaries/lime/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf
  
  INF Binaries/lime/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  INF Binaries/lime/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/lime/PmicDxe/PmicDxe.inf

  INF QcomPkg/Drivers/ColorbarsDxe/ColorbarsDxe.inf
  INF MdeModulePkg/Universal/TimestampDxe/TimestampDxe.inf

  INF Binaries/lime/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/lime/QcomChargerDxeLA/QcomChargerDxeLA.inf
  INF Binaries/lime/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/lime/AdcDxe/AdcDxe.inf
  INF Binaries/lime/TsensDxe/TsensDxe.inf
  INF Binaries/lime/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/lime/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/lime/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/lime/UsbDeviceDxe/UsbDeviceDxe.inf

  INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/lime/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/lime/HashDxe/HashDxe.inf
  INF Binaries/lime/CipherDxe/CipherDxe.inf
  INF Binaries/lime/MacDxe/MacDxe.inf
  INF Binaries/lime/RngDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf

  #INF Binaries/lime/PILDxe/PILDxe.inf
  INF Binaries/lime/QcomChargerApp/QcomChargerApp.inf
  INF Binaries/lime/ShmBridgeDxe/ShmBridgeDxe.inf
