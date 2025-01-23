  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/munch/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/munch/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/munch/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  
  INF Binaries/munch/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  
  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  
  INF Binaries/munch/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/munch/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF Binaries/munch/QcomPkg/Drivers/PdcDxe/PdcDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/munch/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  
  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf
  
  INF Binaries/munch/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/munch/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

  # Only used for DisplayDxe for Qualcomm Demura
  #INF Binaries/munch/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

  INF Binaries/munch/QcomPkg/Drivers/VibratorDxe/VibratorDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/munch/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/munch/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/munch/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/munch/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/munch/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/XhciDxe/XhciDxe.inf
  
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/munch/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  
  INF Binaries/munch/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/munch/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
