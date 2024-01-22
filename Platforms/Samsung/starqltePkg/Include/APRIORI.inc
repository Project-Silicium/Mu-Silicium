APRIORI DXE {

  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/starqlte/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Core Drivers
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf

  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  # DppDxe

  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/starqlte/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf

  # DebugPortDxe
  # SerialDxe

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  # TsFwUpgradeDxe
  INF Binaries/starqlte/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_DISPLAYDXE) == 1
  INF Binaries/starqlte/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/starqlte/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  # OSConfigDxe

  INF SDM845Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  # MemoryMapDxe
  # AdapterInformationDxe
}