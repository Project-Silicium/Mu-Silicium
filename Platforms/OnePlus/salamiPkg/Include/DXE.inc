  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/salami/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/salami/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/salami/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/salami/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/salami/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
#  INF Binaries/salami/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/salami/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF Binaries/salami/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/salami/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/salami/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  !if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/salami/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  !else
  INF Binaries/salami/QcomPkg/Drivers/ClockDxe/ClockDxe.old.inf
  !endif
  INF Binaries/salami/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

#  INF Binaries/salami/QcomPkg/Drivers/RscDxe/RscDxe.inf
#  INF Binaries/salami/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/salami/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/salami/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/salami/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/salami/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/salami/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/salami/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/salami/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/salami/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/salami/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
