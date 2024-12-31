  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/sagit/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

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

  INF Binaries/generic/CobaltWP/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/generic/CobaltWP/DALSys/DALSys.inf
  INF Binaries/generic/CobaltWP/HWIODxeDriver/HWIODxeDriver.inf
  INF Binaries/generic/CobaltWP/ChipInfo/ChipInfo.inf
  INF Binaries/generic/CobaltWP/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf

  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  INF Binaries/generic/CobaltWP/GlinkDxe/GlinkDxe.inf
  INF Binaries/generic/CobaltWP/ULogDxe/ULogDxe.inf
  INF Binaries/generic/CobaltWP/NpaDxe/NpaDxe.inf
  INF Binaries/generic/CobaltWP/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF Binaries/generic/CobaltWP/SdccDxe/SdccDxe.inf
  INF Binaries/generic/CobaltWP/UFSDxe/UFSDxe.inf

  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/sagit/QcomPkg/Drivers/QdssDxe/QdssDxe.inf

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

  INF Binaries/generic/CobaltWP/DALTLMM/DALTLMM.inf
  INF Binaries/generic/CobaltWP/I2C/I2C.inf
  INF Binaries/generic/CobaltWP/SPMI/SPMI.inf
  INF Binaries/generic/CobaltWP/SPI/SPI.inf
  INF Binaries/generic/CobaltWP/PmicDxe/PmicDxe.inf
  INF Binaries/generic/CobaltWP/AdcDxe/AdcDxe.inf
# INF Binaries/poplar/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/generic/CobaltWP/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/CobaltWP/XhciPciEmulation/XhciPciEmulation.inf
  INF Binaries/generic/CobaltWP/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF Binaries/generic/CobaltWP/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  INF Binaries/generic/CobaltWP/UsbTypeCDxe/UsbTypeCDxe.inf
  INF Binaries/generic/CobaltWP/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/generic/CobaltWP/UsbfnChgDxe/UsbfnChgDxe.inf
  INF Binaries/generic/CobaltWP/UsbMouseDxe/UsbMouseDxe.inf
  INF Binaries/generic/CobaltWP/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/generic/CobaltWP/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/generic/CobaltWP/UsbConfigDxe/UsbConfigDxe.inf

  # DebugPortDxe
  # SerialDxe

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/sagit/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  # TsFwUpgradeDxe
# INF Binaries/sagit/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/sagit/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif