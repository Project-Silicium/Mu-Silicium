APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/tapas/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  
  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/tapas/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf

  # Use Binary Version of these two Drivers to Make UFS work correct.
  INF Binaries/tapas/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/tapas/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/tapas/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/tapas/QcomPkg/Drivers/VariableDxe/VariableDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/tapas/QcomPkg/Drivers/PILDxe/PILDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/tapas/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/tapas/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/tapas/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/tapas/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/tapas/QcomPkg/SocPkg/DivarPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/tapas/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
