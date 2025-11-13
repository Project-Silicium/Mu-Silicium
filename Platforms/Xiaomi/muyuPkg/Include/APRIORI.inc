APRIORI DXE {
  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/muyu/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/muyu/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/muyu/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/muyu/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/muyu/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/muyu/QcomPkg/Drivers/FvUtilsDxe/FvUtilsEnhancedDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/HSUartDxe/HSUartDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/muyu/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/muyu/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/muyu/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/muyu/QcomPkg/Drivers/RscDxe/RscDxe.inf
  INF Binaries/muyu/QcomPkg/Drivers/RmVmDxe/RmVmDxe.inf
}
