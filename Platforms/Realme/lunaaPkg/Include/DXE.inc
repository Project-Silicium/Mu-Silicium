  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/lunaa/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/lunaa/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/lunaa/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  #INF Binaries/lunaa/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/lunaa/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/HWIODxe/HWIODxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/lunaa/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/lunaa/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/lunaa/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/lunaa/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/lunaa/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/OcdtDxe/OppoProject.inf
  INF Binaries/lunaa/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
  INF Binaries/lunaa/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/lunaa/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/lunaa/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
