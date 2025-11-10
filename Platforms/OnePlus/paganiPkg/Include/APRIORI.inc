APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/pagani/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/MailboxDxe/MailboxDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/QcomScmiDxe/QcomScmiDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/CalibrationDxe/CalibrationDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/pagani/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/pagani/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/QupDxe/QupDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  
  INF Binaries/pagani/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/pagani/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/OplusStorageDxe/OplusStorageDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/pagani/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/OcdtDxe/OplusProject.inf
  INF Binaries/pagani/QcomPkg/Drivers/OplusS4ResetDxe/OplusS4ResetDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/OplusFpgaDxe/OplusFpga.inf
  INF Binaries/pagani/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/pagani/QcomPkg/Drivers/OplusVibrDxe/OplusVibrDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/OplusSecurityDxe/OplusSecurityDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/pagani/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/pagani/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/pagani/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/pagani/QcomPkg/Drivers/RscDxe/RscDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/QRKSDxe/QRKSDxe.inf
  INF Binaries/pagani/QcomPkg/Drivers/OplusOrdumpDxe/OplusOrdump.inf
  INF Binaries/pagani/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
}
