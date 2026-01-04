  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/senna_bc/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/senna_bc/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  #INF Binaries/senna_bc/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/senna_bc/QcomPkg/Drivers/FontDxe/FontDxe.inf
  #INF Binaries/senna_bc/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/senna_bc/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF Binaries/senna_bc/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/senna_bc/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/senna_bc/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/senna_bc/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/senna_bc/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/senna_bc/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/senna_bc/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/OplusVibrDxe/OplusVibrDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/senna_bc/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/senna_bc/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
