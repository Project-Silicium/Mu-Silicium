APRIORI DXE {
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/SecShareInfoDxe/SecShareInfoDxe.inf

  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/gts8/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/CpuClockHandlerDxe/CpuClockHandlerDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/gts8/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/gts8/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/gts8/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/SecEnvDxe/SecEnvDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF QcomPkg/Drivers/DdrClockHandlerDxe/DdrClockHandlerDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/BoardInfoDxe/BoardInfoDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/RedriverDxe/RedriverDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/ChgDxe/ChgDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SamsungDxe/SubPmicDxe/SubPmicDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/gts8/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/gts8/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  # PmicGlinkDxe
  INF Binaries/gts8/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  # QcomChargerDxeLA
  # ChargerExDxe
  INF Binaries/gts8/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/gts8/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/gts8/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/gts8/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/gts8/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
