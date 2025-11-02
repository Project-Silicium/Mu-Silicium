APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/bitra/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/bitra/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/bitra/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/bitra/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  # Only used for DisplayDxe for Qualcomm Demura
  #INF Binaries/bitra/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/bitra/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/I2CDxe/I2CDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/bitra/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/bitra/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/bitra/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/PhoenixDxe/PhoenixDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/bitra/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/bitra/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/MacDxe/MacDxe.inf
  INF Binaries/bitra/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/bitra/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
}
