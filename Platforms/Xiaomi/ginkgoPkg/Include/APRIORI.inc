APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/PILDxe/PILDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/ginkgo/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/ginkgo/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  # MpPowerDxe
  INF Binaries/ginkgo/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/ginkgo/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
