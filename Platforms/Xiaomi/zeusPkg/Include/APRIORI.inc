APRIORI DXE {

  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF Binaries/zeus/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Core Drivers
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  #INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
  
  INF Binaries/zeus/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/zeus/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/zeus/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/zeus/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/zeus/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
 
  INF Binaries/zeus/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf

  INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/zeus/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
