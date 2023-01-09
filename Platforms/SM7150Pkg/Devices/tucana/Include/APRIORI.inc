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

    #INF Binaries/tucana/SmemDxe/SmemDxe.inf
    #INF Binaries/tucana/DALSys/DALSys.inf
    #INF Binaries/tucana/HWIODxeDriver/HWIODxeDriver.inf
    #INF Binaries/tucana/ChipInfo/ChipInfo.inf
    #INF Binaries/tucana/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
    #INF Binaries/tucana/HALIOMMU/HALIOMMU.inf
    #INF Binaries/tucana/ULogDxe/ULogDxe.inf
    #INF Binaries/tucana/CmdDbDxe/CmdDbDxe.inf
    #INF Binaries/tucana/NpaDxe/NpaDxe.inf
    #INF Binaries/tucana/RpmhDxe/RpmhDxe.inf
    #INF Binaries/tucana/PdcDxe/PdcDxe.inf
    #INF Binaries/tucana/ClockDxe/ClockDxe.inf
    
    # Used to speed up booting. must be loaded after ClockDxe
    INF QcomPkg/Drivers/SetCPUFreqDxe/SetCPUFreqDxe.inf

    #INF Binaries/tucana/ShmBridgeDxe/ShmBridgeDxe.inf
    #INF Binaries/tucana/ScmDxe/ScmDxe.inf

    INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    #INF Binaries/tucana/SdccDxe/SdccDxe.inf
    #INF Binaries/tucana/UFSDxe/UFSDxe.inf

    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
    INF FatPkg/EnhancedFatDxe/Fat.inf

    #INF Binaries/tucana/TzDxe/TzDxe.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    #INF Binaries/tucana/QcomWDogDxe/QcomWDogDxe.inf
    #INF Binaries/tucana/DALTLMM/DALTLMM.inf
    #INF Binaries/tucana/SPMI/SPMI.inf
    INF Binaries/tucana/ResetRuntimeDxe/ResetRuntimeDxe.inf
    #INF Binaries/tucana/PmicDxe/PmicDxe.inf

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    #INF Binaries/tucana/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    #INF Binaries/tucana/I2C/I2C.inf
    #INF Binaries/tucana/VibratorDxe/VibratorDxe.inf
    #INF Binaries/tucana/AdcDxe/AdcDxe.inf
    #INF Binaries/tucana/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
    #INF Binaries/tucana/QcomChargerDxeLA/QcomChargerDxeLA.inf
    #INF Binaries/tucana/ChargerExDxe/ChargerExDxe.inf
    #INF Binaries/tucana/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

    INF MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

    #INF Binaries/tucana/UsbConfigDxe/UsbConfigDxe.inf
    #INF Binaries/tucana/TsensDxe/TsensDxe.inf
    #INF Binaries/tucana/ButtonsDxe/ButtonsDxe.inf
    
!if $(USE_DISPLAYDXE) == FALSE
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    #INF Binaries/tucana/GpiDxe/GpiDxe.inf
    #INF Binaries/tucana/HashDxe/HashDxe.inf
    #INF Binaries/tucana/CipherDxe/CipherDxe.inf
    #INF Binaries/tucana/RngDxe/RngDxe.inf

    INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf

    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

    INF Binaries/tucana/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
