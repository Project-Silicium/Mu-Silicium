# Core DXE initialization
INF Binaries/daredevil/MdeModulePkg/Core/Dxe/DxeMain.inf
INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

INF Binaries/daredevil/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

# Report Status and Error Handling Drivers
INF Binaries/daredevil/MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf

# Runtime Services
INF Binaries/daredevil/MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

# CPU & Interrupt Initialization
INF Binaries/daredevil/ArmPkg/Drivers/CpuDxe/CpuDxe.inf
INF Binaries/daredevil/ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

# Timer
INF Binaries/daredevil/ArmPkg/Drivers/TimerDxe/TimerDxe.inf
INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

# Watchdog Timer
INF Binaries/daredevil/Binaries/daredevil/MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

INF Binaries/daredevil/QcomPkg/Drivers/SecurityDxe/SecurityDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/ASN1X509Dxe/ASN1X509Dxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/SecRSADxe/SecRSADxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/VerifiedBootDxe/VerifiedBootDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

INF QcomPkg/Drivers/DynamicRAMDxe/DynamicRAMDxe.inf

INF Binaries/daredevil/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
INF Binaries/daredevil/EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

INF Binaries/daredevil/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

INF Binaries/daredevil/QcomPkg/Drivers/DiskIoDxe/DiskIoDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/PartitionDxe/PartitionDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
INF Binaries/daredevil/FatPkg/EnhancedFatDxe/Fat.inf
INF Binaries/daredevil/MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/QdssDxe/QdssDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf

INF Binaries/daredevil/QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
INF Binaries/daredevil/QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
INF Binaries/daredevil/EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/FontDxe/FontDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/QcomBds/QcomBds.inf
INF Binaries/daredevil/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
INF Binaries/daredevil/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
INF Binaries/daredevil/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

# Generic modules

INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

# Mu modules start
INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
# Mu modules end

INF Binaries/daredevil/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf

INF Binaries/daredevil/QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf
INF Binaries/daredevil/MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
INF Binaries/daredevil/MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/daredevil/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

INF Binaries/daredevil/QcomPkg/Drivers/TzDxe/TzDxeLA.inf
INF Binaries/daredevil/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/MdtpDxe/MdtpDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/HashDxe/HashDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/RNGDxe/RngDxe.inf
INF Binaries/daredevil/QcomPkg/Drivers/MpPowerDxe/MpPowerDxe.inf
