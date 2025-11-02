  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/q2q/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/q2q/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/q2q/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/q2q/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/q2q/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/q2q/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/HWIODxe/HWIODxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/q2q/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/q2q/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/q2q/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/BoardInfoDxe/BoardInfoDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/q2q/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/q2q/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/q2q/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/q2q/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  #INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  #INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/VibDxe/VibDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/GuidedFvDxe/GuidedFvDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/SubPmicDxe/SubPmicDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/GpioExpanderDxe/GpioExpanderDxe.inf
  INF Binaries/q2q/QcomPkg/Drivers/SamsungDxe/ChgDxe/ChgDxe.inf
