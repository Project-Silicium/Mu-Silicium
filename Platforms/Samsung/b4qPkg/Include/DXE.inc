  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/SecShareInfoDxe/SecShareInfoDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/b4q/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/b4q/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
  INF Binaries/b4q/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF Binaries/b4q/QcomPkg/Drivers/FontDxe/FontDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/b4q/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  # Samsung Drivers Require this to work properly.
  INF Binaries/b4q/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/b4q/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/b4q/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

  INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/b4q/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/b4q/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/b4q/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/b4q/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/b4q/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/RNGDxe/RngDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/SecEnvDxe/SecEnvDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/BoardInfoDxe/BoardInfoDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/CcicDxe/CcicDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/RedriverDxe/RedriverDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/MuicDxe/MuicDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/VibDxe/VibDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/ChgDxe/ChgDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/SubPmicDxe/SubPmicDxe.inf
  INF Binaries/b4q/QcomPkg/Drivers/SamsungDxe/GuidedFvDxe/GuidedFvDxe.inf
