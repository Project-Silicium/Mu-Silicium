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
    INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
    INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
    INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf

    INF Binaries/laurel_sprout/SmemDxe/SmemDxe.inf
    INF Binaries/laurel_sprout/DALSys/DALSys.inf
    INF Binaries/laurel_sprout/HWIODxeDriver/HWIODxeDriver.inf
    INF Binaries/laurel_sprout/ChipInfo/ChipInfo.inf
    INF Binaries/laurel_sprout/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
    INF Binaries/laurel_sprout/HALIOMMU/HALIOMMU.inf
    INF Binaries/laurel_sprout/GlinkDxe/GlinkDxe.inf
    INF Binaries/laurel_sprout/ULogDxe/ULogDxe.inf
    INF Binaries/laurel_sprout/NpaDxe/NpaDxe.inf
    #INF Binaries/laurel_sprout/ClockDxe/ClockDxe.inf
    INF Binaries/laurel_sprout/ShmBridgeDxe/ShmBridgeDxe.inf
    INF Binaries/laurel_sprout/ScmDxe/ScmDxe.inf

    INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    INF Binaries/laurel_sprout/SdccDxe/SdccDxe.inf
    INF Binaries/laurel_sprout/UFSDxe/UFSDxe.inf

    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
    INF FatPkg/EnhancedFatDxe/Fat.inf

    INF Binaries/laurel_sprout/TzDxe/TzDxe.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    INF Binaries/laurel_sprout/QcomWDogDxe/QcomWDogDxe.inf
    INF Binaries/laurel_sprout/DALTLMM/DALTLMM.inf
    INF Binaries/laurel_sprout/SPMI/SPMI.inf
    INF Binaries/laurel_sprout/ResetRuntimeDxe/ResetRuntimeDxe.inf
    INF Binaries/laurel_sprout/PmicDxe/PmicDxe.inf
    INF Binaries/laurel_sprout/PILDxe/PILDxe.inf

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    INF Binaries/laurel_sprout/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    INF Binaries/laurel_sprout/GpiDxe/GpiDxe.inf
    INF Binaries/laurel_sprout/I2C/I2C.inf
    #INF Binaries/laurel_sprout/AdcDxe/AdcDxe.inf
    INF Binaries/laurel_sprout/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
    INF Binaries/laurel_sprout/QcomChargerDxeLA/QcomChargerDxeLA.inf
    INF Binaries/laurel_sprout/ChargerExDxe/ChargerExDxe.inf
    INF Binaries/laurel_sprout/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

    INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

    INF Binaries/laurel_sprout/UsbConfigDxe/UsbConfigDxe.inf
    INF Binaries/laurel_sprout/ButtonsDxe/ButtonsDxe.inf
    INF Binaries/laurel_sprout/TsensDxe/TsensDxe.inf

!if $(USE_DISPLAYDXE) == TRUE
    INF Binaries/laurel_sprout/DisplayDxe/DisplayDxe.inf
!else
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    INF Binaries/laurel_sprout/HashDxe/HashDxe.inf
    INF Binaries/laurel_sprout/CipherDxe/CipherDxe.inf
    INF Binaries/laurel_sprout/RngDxe/RngDxe.inf
    INF Binaries/laurel_sprout/DDRInfoDxe/DDRInfoDxe.inf

    INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

    INF Binaries/laurel_sprout/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
