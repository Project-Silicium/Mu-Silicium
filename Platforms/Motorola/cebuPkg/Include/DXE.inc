  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  INF Binaries/cebu/MbmPkg/Core/Drivers/MotHwIdDxe/MotHwIdDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MotUtagsDxe/MotUtagsDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MotUtagsDictDxe/MotUtagsDictDxe.inf

  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/cebu/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/cebu/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/cebu/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/cebu/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/cebu/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/cebu/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/MinidumpTADxe/MinidumpTADxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MotStorageDxe/MotStorageDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MotBootFlagsDxe/MotBootFlagsDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MotCoreServicesDxe/MotCoreServicesDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MbmModeDxe/MbmModeDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/AbDxe/AbDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/MotSolDxe/MotSolDxe.inf
  INF Binaries/cebu/MbmPkg/Core/Drivers/BacklightDxe/BacklightDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/cebu/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/cebu/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/cebu/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/cebu/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/cebu/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/cebu/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/cebu/QcomPkg/SocPkg/KamortaPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  #INF Binaries/cebu/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/cebu/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
