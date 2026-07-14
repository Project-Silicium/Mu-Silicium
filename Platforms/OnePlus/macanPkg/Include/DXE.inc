  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/macan/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/macan/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/macan/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/macan/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/macan/QcomPkg/Drivers/PsStateDxe/PsStateDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/macan/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/macan/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf 
  INF Binaries/macan/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/macan/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/QupDxe/QupDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF Binaries/macan/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/macan/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/macan/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/MailboxDxe/MailboxDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/QcomScmiDxe/QcomScmiDxe.inf
  #INF Binaries/macan/QcomPkg/Drivers/CalibrationDxe/CalibrationDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/macan/QcomPkg/Drivers/RscDxe/RscDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/macan/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/macan/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/macan/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/macan/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/macan/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  #INF Binaries/macan/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkSoccpDxe.inf
  #INF Binaries/macan/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLAGio.inf
  INF Binaries/macan/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/macan/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/macan/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
  INF Binaries/macan/QcomPkg/Drivers/ParserDxe/ParserEnhancedDxe.inf
