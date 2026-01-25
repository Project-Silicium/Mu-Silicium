APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/lisa/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/lisa/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/lisa/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/I2CDxe/I2CDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/lisa/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/lisa/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/lisa/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/lisa/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/lisa/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/lisa/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/lisa/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
