  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/bale/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/bale/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/bale/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/bale/QcomPkg/Drivers/PsStateDxe/PsStateDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/bale/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF Binaries/bale/MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF Binaries/bale/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/bale/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/bale/QcomPkg/Drivers/RscDxe/RscDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/bale/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/bale/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/bale/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/bale/QcomPkg/Drivers/QRKSDxe/QRKSDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/Smp2pDxe/Smp2pDxe.inf
  #INF Binaries/bale/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  #INF Binaries/bale/QcomPkg/Drivers/PhoenixDxe/PhoenixDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  #INF Binaries/bale/QcomPkg/Drivers/OplusVibrDxe/OplusVibrDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/bale/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/SSRDxe/SSRDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/OplusSecurityDxe/OplusSecurityDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
  INF Binaries/bale/QcomPkg/Drivers/ParserDxe/ParserEnhancedDxe.inf
  INF Binaries/bale/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
