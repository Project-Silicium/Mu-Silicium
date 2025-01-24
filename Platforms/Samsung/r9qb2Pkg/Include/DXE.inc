  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  # INF Binaries/r9qb2/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/HWIODxe/HWIODxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/r9qb2/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/r9qb2/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/BoardInfoDxe/BoardInfoDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/r9qb2/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/r9qb2/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/r9qb2/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  #INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  #INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/VibDxe/VibDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/GuidedFvDxe/GuidedFvDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/SubPmicDxe/SubPmicDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/GpioExpanderDxe/GpioExpanderDxe.inf
  INF Binaries/r9qb2/QcomPkg/Drivers/SamsungDxe/ChgDxe/ChgDxe.inf
