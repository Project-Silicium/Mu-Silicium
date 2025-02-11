APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/poplar/SomcPkg/Drivers/MazeDetDxe/MazeDetDxe.inf
  INF Binaries/poplar/SomcPkg/Drivers/XHwresetDxe/XHwresetDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

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

  INF Binaries/poplar/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf

  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/poplar/QcomPkg/Drivers/QdssDxe/QdssDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  # DppDxe

  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/poplar/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/poplar/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf

  # DebugPortDxe
  # SerialDxe

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  # TsFwUpgradeDxe
  INF Binaries/poplar/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/poplar/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/poplar/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/poplar/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/MdtpDxe/MdtpDxe.inf

  INF MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf

  INF Binaries/poplar/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/poplar/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  # OSConfigDxe
  # MpParkDxe
  INF Binaries/poplar/QcomPkg/Drivers/MpPowerDxe/MpPowerDxe.inf

  INF QcomPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  # MemoryMapDxe
  # AdapterInformationDxe
}
