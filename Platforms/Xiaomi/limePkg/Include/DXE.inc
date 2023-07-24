  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/lime/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf

  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf
  INF Binaries/lime/QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/lime/MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/lime/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf

  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf

  INF Binaries/lime/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/lime/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_DISPLAYDXE) == 1
  INF Binaries/lime/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/lime/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/lime/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/lime/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/lime/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  #INF Binaries/lime/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/lime/QcomPkg/Application/QcomChargerApp/QcomChargerApp.inf
  INF Binaries/lime/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
