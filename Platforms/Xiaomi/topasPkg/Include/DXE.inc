  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/topas/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/topas/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/topas/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/QcomMpmTimerDxe/QcomMpmTimerDxe.inf

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

  INF Binaries/topas/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRAMDxe/DynamicRAMDxe.inf
  
  INF Binaries/topas/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  # Use Binary Version of these two Drivers to Make UFS work correct.
  INF Binaries/topas/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/topas/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/topas/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/topas/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/topas/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/MinidumpTADxe/MinidumpSMEMTADxe.inf

!if $(USE_DISPLAYDXE) == 1
  INF Binaries/topas/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/topas/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/topas/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/topas/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/topas/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/topas/QcomPkg/SocPkg/DivarPkg/Drivers/RNGDxe/RngDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/topas/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/topas/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
