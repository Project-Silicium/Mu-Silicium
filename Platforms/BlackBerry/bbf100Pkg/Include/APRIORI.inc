APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/bbf100/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Core Drivers
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/bbf100/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/bbf100/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  
  INF Binaries/bbf100/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF Binaries/bbf100/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  
  INF Binaries/bbf100/QcomPkg/Drivers/QdssDxe/QdssDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/bbf100/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  
  INF Binaries/bbf100/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/bbf100/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/bbf100/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf
  
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/bbf100/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  
  INF Binaries/bbf100/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/bbf100/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  #INF Binaries/bbf100/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/bbf100/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/MdtpDxe/MdtpDxe.inf
  
 
  INF Binaries/bbf100/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/bbf100/QcomPkg/Drivers/MpPowerDxe/MpPowerDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
 
}
