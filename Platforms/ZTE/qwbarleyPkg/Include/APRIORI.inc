APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF QcomPkg/Drivers/IortDtDxe/IortDtDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/EnvDxe/EnvDxeEnhanced.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  # PlatformPei initializes XBLCore thread/TLS state (tpidr_el1
  # and the core-0 thread object) before DXE dispatch.  HALIOMMUDxe provides the
  # Qcom IOMMU protocol 54B6D3B4 required by SdccDxe/UFSDxe.
  INF Binaries/qwbarley/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/TzDxe/ScmDxeCompat.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/MailboxDxe/MailboxDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/QcomScmiDxe/QcomScmiDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/QupDxe/QupDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/I3CMasterDxe/I3CMasterDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf

  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf

  # NOTE: ZTE VendorLogfsDxe is deliberately NOT enabled.  It flushes the
  # ZTE ULog RAM buffer, which our DEBUG output never enters.  DEBUG goes to
  # the framebuffer via the standard SiliciumPkg FrameBufferSerialPortLib.

  # PMIC chain enabled (HALIOMMUDxe SMMU/IOMMU active).
  INF Binaries/qwbarley/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/qwbarley/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF Binaries/qwbarley/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

  INF FatPkg/EnhancedFatDxe/Fat.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
  # DISABLED (qwbarley): PIL drivers — PILDxe constructor fails with
  # EFI_NOT_FOUND (AutoGen.c +640) and ASSERTs; they load DSP/modem firmware
  # which is not needed to reach the payload. Re-enable once the underlying
  # protocol/HOB is available.
  # INF Binaries/qwbarley/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  # INF Binaries/qwbarley/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf
  # GLinkDxe disabled in DXE.inc (PmicGlinkSoccpDxe is used instead).

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkSoccpDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLAGio.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/qwbarley/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/qwbarley/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/qwbarley/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/qwbarley/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/qwbarley/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

  INF Binaries/qwbarley/QcomPkg/Drivers/RscDxe/RscDxe.inf
}
