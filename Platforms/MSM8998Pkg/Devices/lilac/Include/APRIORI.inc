APRIORI DXE {

    # First, install the PCD driver and call DxeInit
    INF MdeModulePkg/Core/Dxe/DxeMain.inf
    INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
    
    INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
    INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
    INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

    # Next, install cpu protocol and enable the interrupt controller
    INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
    INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

    # Core Drivers
    INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
    INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

    #INF Binaries/lilac/SmemDxe/SmemDxe.inf
    #INF Binaries/lilac/DALSys/DALSys.inf
    #INF Binaries/lilac/HWIODxeDriver/HWIODxeDriver.inf
    #INF Binaries/lilac/ChipInfo/ChipInfo.inf
    #INF Binaries/lilac/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf

    INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

    #INF Binaries/lilac/GlinkDxe/GlinkDxe.inf
    #INF Binaries/lilac/ULogDxe/ULogDxe.inf
    #INF Binaries/lilac/NpaDxe/NpaDxe.inf
    #INF Binaries/lilac/ClockDxe/ClockDxe.inf
    
    # Used to speed up booting. must be loaded after ClockDxe
    INF QcomPkg/Drivers/SetCPUFreqDxe/SetCPUFreqDxe.inf

    INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf

    #INF Binaries/lilac/SdccDxe/SdccDxe.inf
    #INF Binaries/lilac/UFSDxe/UFSDxe.inf

    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
    INF FatPkg/EnhancedFatDxe/Fat.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    #INF Binaries/lilac/QdssDxe/QdssDxe.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
    INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
    
    INF Binaries/lilac/ResetRuntimeDxe/ResetRuntimeDxe.inf
    
    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    #INF Binaries/lilac/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    #INF Binaries/lilac/DALTLMM/DALTLMM.inf
    #INF Binaries/lilac/I2C/I2C.inf
    #INF Binaries/lilac/SPMI/SPMI.inf
    #INF Binaries/lilac/SPI/SPI.inf
    #INF Binaries/lilac/PmicDxe/PmicDxe.inf
    #INF Binaries/lilac/AdcDxe/AdcDxe.inf
    #INF Binaries/lilac/QcomChargerDxeLA/QcomChargerDxeLA.inf
    #INF Binaries/lilac/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

    INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
    
    #INF Binaries/lilac/UsbConfigDxe/UsbConfigDxe.inf
    
    INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

    INF Binaries/lilac/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
    #INF Binaries/lilac/ButtonsDxe/ButtonsDxe.inf
    #INF Binaries/lilac/TsensDxe/TsensDxe.inf

!if $(USE_DISPLAYDXE) == FALSE
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    #INF Binaries/lilac/TzDxe/TzDxe.inf
    #INF Binaries/lilac/LimitsDxe/LimitsDxe.inf
    
    INF MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
    INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
    INF MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
    
    #INF Binaries/lilac/HashDxe/HashDxe.inf
    #INF Binaries/lilac/RngDxe/RngDxe.inf
    #INF Binaries/lilac/MpPowerDxe/MpPowerDxe.inf
    
    INF QcomPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
}
