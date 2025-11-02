  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/a52q/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/a52q/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/a52q/QcomPkg/Drivers/PdcDxe/PdcDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/a52q/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/a52q/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/a52q/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/a52q/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/a52q/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SamsungDxe/ChgDxe/ChgDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SamsungDxe/VibDxe/VibDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SamsungDxe/LazyPanicDxe/LazyPanicDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/SamsungDxe/GuidedFvDxe/GuidedFvDxe.inf
  INF Binaries/a52q/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
