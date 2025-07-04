  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/dodge/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/dodge/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/dodge/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/dodge/QcomPkg/Drivers/PsStateDxe/PsStateDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/FontDxe/FontDxe.inf
 # INF Binaries/dodge/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf 

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/QupDxe/QupDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF Binaries/dodge/MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF Binaries/dodge/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/dodge/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/OplusStorageDxe/OplusStorageDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/MailboxDxe/MailboxDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/QcomScmiDxe/QcomScmiDxe.inf
  #INF Binaries/dodge/QcomPkg/Drivers/CalibrationDxe/CalibrationDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  #INF QcomPkg/Drivers/CpuClockHandlerDxe/CpuClockHandlerDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/RscDxe/RscDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/dodge/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
 # INF Binaries/dodge/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/OplusFpgaDxe/OplusFpga.inf
  INF Binaries/dodge/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/dodge/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/dodge/QcomPkg/Drivers/QRKSDxe/QRKSDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  #INF Binaries/dodge/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/dodge/QcomPkg/Drivers/OplusS4ResetDxe/OplusS4ResetDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/dodge/QcomPkg/Drivers/PhoenixDxe/PhoenixDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/Phoenix2_0Dxe/Phoenix2_0Dxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/OplusVibrDxe/OplusVibrDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF QcomPkg/Drivers/DdrClockHandlerDxe/DdrClockHandlerDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/dodge/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
  INF Binaries/dodge/QcomPkg/Drivers/ParserDxe/ParserEnhancedDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  INF Binaries/dodge/QcomPkg/Drivers/OplusOrdumpDxe/OplusOrdump.inf
  INF Binaries/dodge/QcomPkg/Drivers/OplusSecurityDxe/OplusSecurityDxe.inf
