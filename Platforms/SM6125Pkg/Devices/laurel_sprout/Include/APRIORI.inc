APRIORI DXE {

    # First, install the PCD driver and call DxeInit
    INF MdeModulePkg/Core/Dxe/DxeMain.inf
    INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

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

    INF Binaries/laurel_sprout/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
    INF Binaries/laurel_sprout/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf

    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
    INF FatPkg/EnhancedFatDxe/Fat.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/PILDxe/PILDxe.inf

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf

    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

    INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

    INF Binaries/laurel_sprout/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

!if $(USE_DISPLAYDXE) == 1
    INF Binaries/laurel_sprout/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    INF Binaries/laurel_sprout/QcomPkg/Drivers/HashDxe/HashDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/CipherDxe/CipherDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/RNGDxe/RngDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
    INF Binaries/laurel_sprout/QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf

    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

    INF Binaries/laurel_sprout/MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
