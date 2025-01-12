  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/aston/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/aston/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/aston/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/aston/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/aston/QcomPkg/Drivers/PsStateDxe/PsStateDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/aston/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/aston/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  #INF Binaries/aston/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/aston/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF Binaries/aston/MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF Binaries/aston/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/aston/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/aston/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  
  #INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf
  
  INF Binaries/aston/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  
  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/aston/QcomPkg/Drivers/RscDxe/RscDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/aston/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/aston/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/aston/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/aston/QcomPkg/Drivers/QRKSDxe/QRKSDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  #INF Binaries/aston/QcomPkg/Drivers/Phoenix2_0Dxe/Phoenix2_0Dxe.inf
  INF Binaries/aston/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/aston/QcomPkg/Drivers/OplusVibrDxe/OplusVibrDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/aston/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/aston/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/aston/QcomPkg/Drivers/OplusSecurityDxe/OplusSecurityDxe.inf
