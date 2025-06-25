APRIORI DXE {

# Core DXE initialization
INF MdeModulePkg/Core/Dxe/DxeMain.inf
# Add PCD immediately after DxeMain
INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

INF QcomPkg/Drivers/EnvDxe/EnvDxe.inf

# Report Status and Error Handling Drivers
INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf

# Runtime Services
INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

# CPU & Interrupt Initialization
INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

# Timer
INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

# Insert ArmPsciMpServices right after TimerDxe
INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

# Watchdog Timer
INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

# (Drivers corresponding to unidentified GUIDs in UEFITool dump would come next)
# For example:
# INF <Path>/SomeDriver.inf   ; for GUID 90A49AFD-422F-08AE-9611-E788D3804845
# INF <Path>/SomeOtherDriver.inf   ; for each subsequent GUID entry…
# ... (continue as needed based on your firmware’s UEFITool order)

INF QcomPkg/Drivers/SecurityDxe/SecurityDxe.inf
INF QcomPkg/Drivers/ASN1X509Dxe/ASN1X509Dxe.inf
INF QcomPkg/Drivers/SecRSADxe/SecRSADxe.inf
INF QcomPkg/Drivers/VerifiedBootDxe/VerifiedBootDxe.inf
INF QcomPkg/Drivers/SmemDxe/SmemDxe.inf

# Immediately after SmemDxe, add DynamicRAMDxe:
INF QcomPkg/Drivers/DynamicRAMDxe/DynamicRAMDxe.inf

INF QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
INF QcomPkg/Drivers/HWIODxe/HWIODxe.inf
INF QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
INF QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
INF QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
INF QcomPkg/Drivers/ULogDxe/ULogDxe.inf
INF QcomPkg/Drivers/NpaDxe/NpaDxe.inf
INF QcomPkg/Drivers/ClockDxe/ClockDxe.inf

# Right after ClockDxe, insert ClockSpeedUpDxe:
INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

INF QcomPkg/Drivers/DiskIoDxe/DiskIoDxe.inf
INF QcomPkg/Drivers/PartitionDxe/PartitionDxe.inf
INF QcomPkg/Drivers/SdccDxe/SdccDxe.inf
INF QcomPkg/Drivers/UFSDxe/UFSDxe.inf
INF FatPkg/EnhancedFatDxe/Fat.inf
INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
INF QcomPkg/Drivers/QdssDxe/QdssDxe.inf
INF MdeModulePkg/Universal/Variable/EmuRuntimeDxe/EmuVariableRuntimeDxe.inf

INF QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
INF  QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
INF QcomPkg/Drivers/FontDxe/FontDxe.inf
INF QcomPkg/Drivers/QcomBds/QcomBds.inf
INF QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
INF QcomPkg/Drivers/I2CDxe/I2CDxe.inf
INF QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
INF QcomPkg/Drivers/SPIDxe/SPIDxe.inf
INF QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
INF QcomPkg/Drivers/AdcDxe/AdcDxe.inf
INF QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
INF QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
INF QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
INF QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
INF QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
INF QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf
INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
INF QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
INF QcomPkg/Drivers/TsensDxe/TsensDxe.inf
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif
INF QcomPkg/Drivers/TzDxe/TzDxeLA.inf
INF QcomPkg/Drivers/MpPowerDxe/MpPowerDxe.inf
}
