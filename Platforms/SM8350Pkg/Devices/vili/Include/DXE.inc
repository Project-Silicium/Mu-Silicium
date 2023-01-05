  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/vili/ShmBridgeDxeLA/ShmBridgeDxeLA.inf
  INF Binaries/vili/ScmDxeLA/ScmDxeLA.inf
  INF Binaries/vili/TzDxeLA/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

  INF Binaries/vili/ResetRuntimeDxe/ResetRuntimeDxe.inf

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/vili/FontDxe/FontDxe.inf
  INF Binaries/vili/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  INF Binaries/vili/ArmTimerDxe/ArmTimerDxe.inf

  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
    
  INF Binaries/vili/ChipInfo/ChipInfo.inf
  INF Binaries/vili/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  INF Binaries/vili/ProjectInfoDxeDriver/ProjectInfoDxeDriver.inf
  INF Binaries/vili/DALSys/DALSys.inf
  INF Binaries/vili/HALIOMMU/HALIOMMU.inf
  INF Binaries/vili/HWIODxeDriver/HWIODxeDriver.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf

  INF Binaries/vili/UFSDxe/UFSDxe.inf
  INF Binaries/vili/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/vili/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/vili/RpmhDxe/RpmhDxe.inf
  INF Binaries/vili/NpaDxe/NpaDxe.inf
  INF Binaries/vili/ULogDxe/ULogDxe.inf
  INF Binaries/vili/VcsDxe/VcsDxe.inf
  INF Binaries/vili/ClockDxe/ClockDxe.inf
  INF Binaries/vili/ICBDxe/ICBDxe.inf
  INF Binaries/vili/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf

  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF MdeModulePkg/Universal/EsrtFmpDxe/EsrtFmpDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  INF Binaries/vili/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF Binaries/vili/GpiDxe/GpiDxe.inf
  INF Binaries/vili/I2C/I2C.inf
  INF Binaries/vili/SPI/SPI.inf
  INF Binaries/vili/SPMI/SPMI.inf
  INF Binaries/vili/PmicDxe/PmicDxe.inf

  INF QcomPkg/Drivers/ColorbarsDxe/ColorbarsDxe.inf
  INF MdeModulePkg/Universal/TimestampDxe/TimestampDxe.inf

  INF Binaries/vili/SdccDxe/SdccDxe.inf
  INF Binaries/vili/DALTLMM/DALTLMM.inf
  INF Binaries/vili/FeatureEnablerDxe/FeatureEnablerDxe.inf
  INF Binaries/vili/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_DISPLAYDXE) == FALSE
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf

  INF Binaries/vili/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/vili/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/vili/IPCCDxe/IPCCDxe.inf
  INF Binaries/vili/GlinkDxe/GlinkDxe.inf
  INF Binaries/vili/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/vili/QcomChargerDxeLA/QcomChargerDxeLA.inf
  INF Binaries/vili/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/vili/TsensDxe/TsensDxe.inf
  INF Binaries/vili/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/vili/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/vili/XhciPciEmulation/XhciPciEmulation.inf
  INF Binaries/vili/XhciDxe/XhciDxe.inf
  INF Binaries/vili/UsbBusDxe/UsbBusDxe.inf
  INF Binaries/vili/UsbKbDxe/UsbKbDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf

  INF Binaries/vili/UsbMassStorageDxe/UsbMassStorageDxe.inf
  INF Binaries/vili/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/vili/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/vili/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/vili/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/vili/UCDxe/UCDxe.inf
  INF Binaries/vili/RngDxe/RngDxe.inf
  INF Binaries/vili/PILDxe/PILDxe.inf
  INF Binaries/vili/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/vili/SPSSDxe/SPSSDxe.inf
  INF Binaries/vili/FvDxe/FvDxe.inf
  INF Binaries/vili/ParserDxe/ParserDxe.inf
  INF Binaries/vili/QcomChargerApp/QcomChargerApp.inf
