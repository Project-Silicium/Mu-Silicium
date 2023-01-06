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

    INF Binaries/ginkgo/SmemDxe/SmemDxe.inf
    INF Binaries/ginkgo/DALSys/DALSys.inf
    INF Binaries/ginkgo/HWIODxeDriver/HWIODxeDriver.inf
    INF Binaries/ginkgo/ChipInfo/ChipInfo.inf
    INF Binaries/ginkgo/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
    INF Binaries/ginkgo/HALIOMMU/HALIOMMU.inf
    INF Binaries/ginkgo/GlinkDxe/GlinkDxe.inf
    INF Binaries/ginkgo/ULogDxe/ULogDxe.inf
    INF Binaries/ginkgo/NpaDxe/NpaDxe.inf
    INF Binaries/ginkgo/ClockDxe/ClockDxe.inf
    INF Binaries/ginkgo/ShmBridgeDxe/ShmBridgeDxe.inf
    INF Binaries/ginkgo/ScmDxe/ScmDxe.inf

    INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    INF Binaries/ginkgo/SdccDxe/SdccDxe.inf
    INF Binaries/ginkgo/UFSDxe/UFSDxe.inf

    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
    INF FatPkg/EnhancedFatDxe/Fat.inf

    INF Binaries/ginkgo/TzDxe/TzDxe.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    INF Binaries/ginkgo/QcomWDogDxe/QcomWDogDxe.inf
    INF Binaries/ginkgo/DALTLMM/DALTLMM.inf
    INF Binaries/ginkgo/SPMI/SPMI.inf
    INF Binaries/ginkgo/ResetRuntimeDxe/ResetRuntimeDxe.inf
    INF Binaries/ginkgo/PmicDxe/PmicDxe.inf
    INF Binaries/ginkgo/PILDxe/PILDxe.inf

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    INF Binaries/ginkgo/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    INF Binaries/ginkgo/GpiDxe/GpiDxe.inf
    INF Binaries/ginkgo/I2C/I2C.inf
    INF Binaries/ginkgo/AdcDxe/AdcDxe.inf
    INF Binaries/ginkgo/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
    INF Binaries/ginkgo/QcomChargerDxeLA/QcomChargerDxeLA.inf
    INF Binaries/ginkgo/ChargerExDxe/ChargerExDxe.inf
    INF Binaries/ginkgo/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

    INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

    INF Binaries/ginkgo/UsbConfigDxe/UsbConfigDxe.inf
    INF Binaries/ginkgo/ButtonsDxe/ButtonsDxe.inf
    INF Binaries/ginkgo/TsensDxe/TsensDxe.inf

!if $(USE_DISPLAYDXE) == TRUE
    INF Binaries/ginkgo/DisplayDxe/DisplayDxe.inf
!else
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    INF Binaries/ginkgo/HashDxe/HashDxe.inf
    INF Binaries/ginkgo/CipherDxe/CipherDxe.inf
    INF Binaries/ginkgo/RngDxe/RngDxe.inf
    INF Binaries/ginkgo/DDRInfoDxe/DDRInfoDxe.inf

    INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

    INF Binaries/ginkgo/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
