  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/gts9wifi/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/gts9wifi/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

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

  INF Binaries/gts9wifi/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  #INF Binaries/gts9wifi/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/gts9wifi/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf


  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/gts9wifi/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/gts9wifi/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SdccDxe/SdccDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/gts9wifi/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/gts9wifi/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/gts9wifi/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SamsungDxe/RedriverDxe/RedriverDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/gts9wifi/QcomPkg/Drivers/SamsungDxe/SubPmicDxe/SubPmicDxe.inf
