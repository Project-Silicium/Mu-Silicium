  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/giulia/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/giulia/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/giulia/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/giulia/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/giulia/QcomPkg/Drivers/PsStateDxe/PsStateDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/giulia/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/giulia/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF Binaries/giulia/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/giulia/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/giulia/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/OplusStorageDxe/OplusStorageDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/giulia/QcomPkg/Drivers/RscDxe/RscDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/giulia/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/giulia/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/giulia/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/giulia/QcomPkg/Drivers/QRKSDxe/QRKSDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/Smp2pDxe/Smp2pDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/SSRDxe/SSRDxe.inf
  # Crashes
  # INF Binaries/giulia/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/giulia/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/giulia/QcomPkg/Drivers/PhoenixDxe/PhoenixDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/Phoenix2_0Dxe/Phoenix2_0Dxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/OplusVibrDxe/OplusVibrDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/giulia/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
  INF Binaries/giulia/QcomPkg/Drivers/ParserDxe/ParserEnhancedDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  INF Binaries/giulia/QcomPkg/Drivers/OplusOrdumpDxe/OplusOrdump.inf
  INF Binaries/giulia/QcomPkg/Drivers/OplusSecurityDxe/OplusSecurityDxe.inf
