APRIORI DXE {

  # First, install the PCD driver and call DxeInit
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  # INF Binaries/starqlte/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  # INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  # INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  # Next, install cpu protocol and enable the interrupt controller
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

  # Core Drivers
  INF Binaries/starqlte/EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/starqlte/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  
!if $(USE_DISPLAYDXE) == 1
  INF Binaries/starqlte/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif 

!if $(USE_DISPLAYDXE) == 1
  INF Binaries/starqlte/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/FontDxe/FontDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/starqlte/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/starqlte/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}