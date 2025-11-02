APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/pong/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/pong/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/pong/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/pong/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/pong/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/pong/QcomPkg/Drivers/I2CHapticDxe/I2CHapticDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf


  INF Binaries/pong/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/pong/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/pong/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/pong/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/pong/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/pong/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/pong/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/pong/QcomPkg/Drivers/NT_DeviceInfoDxe/NT_DeviceInfoDxe.inf
  INF Binaries/pong/QcomPkg/Drivers/BootloaderLoggingDxe/BootloaderLoggingDxe.inf
}
