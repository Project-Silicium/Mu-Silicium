  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/lemonade/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/lemonade/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/lemonade/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/lemonade/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/lemonade/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/lemonade/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/HWIODxe/HWIODxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/lemonade/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/lemonade/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  # Resets UEFI in Silicium Mass Storage due to Watchdog, disable for now
  #INF Binaries/lemonade/QcomPkg/Drivers/PhoenixDxe/PhoenixDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/lemonade/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/lemonade/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/lemonade/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/lemonade/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/lemonade/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/OcdtDxe/OppoProject.inf
  INF Binaries/lemonade/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/lemonade/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/lemonade/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/lemonade/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
