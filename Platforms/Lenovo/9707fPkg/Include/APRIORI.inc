APRIORI DXE {
    # First, install the PCD driver and call DxeInit
    INF MdeModulePkg/Core/Dxe/DxeMain.inf
    INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

    INF Binaries/9707f/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

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

    INF Binaries/9707f/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

    INF QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf

    INF Binaries/9707f/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
    INF Binaries/9707f/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
    INF Binaries/9707f/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

    INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    INF Binaries/9707f/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

    INF FatPkg/EnhancedFatDxe/Fat.inf
    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

    INF Binaries/9707f/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    # FILE DRIVER = 1C04F529-0612-4319-9315-B2E75C16810A {
    #        SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.efi
    #        SECTION UI = "FeatureEnablerDxe"
    # }

    INF Binaries/9707f/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/I2CDxe/I2CDxe.inf

!if $(USE_DISPLAYDXE) == 1
    INF Binaries/9707f/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    INF Binaries/9707f/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/PILDxe/PILDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf

!if $(USE_DISPLAYDXE) == 1
    INF Binaries/9707f/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

    INF Binaries/9707f/QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf

    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    INF Binaries/9707f/QcomPkg/Drivers/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    INF Binaries/9707f/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
    INF Binaries/9707f/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf

    FILE DRIVER = B7F50E91-A759-412C-ADE4-DCD03E7F7C28 {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/XhciDxe/XhciDxe.efi
            SECTION UI = "XhciDxe"
    }

    FILE DRIVER = 240612B7-A063-11D4-9A3A-0090273FC14D {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/UsbBusDxe/UsbBusDxe.efi
            SECTION UI = "UsbBusDxe"
    }

    FILE DRIVER = 2D2E62CF-9ECF-43B7-8219-94E7FC713DFE {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/UsbKbDxe/UsbKbDxe.efi
            SECTION UI = "UsbKbDxe"
    }

    FILE DRIVER = 9FB4B4A7-42C0-4BCD-8540-9BCC6711F83E {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/UsbMassStorageDxe/UsbMassStorageDxe.efi
            SECTION UI = "UsbMassStorageDxe"
    }

    INF Binaries/9707f/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
    INF Binaries/9707f/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

    FILE DRIVER = F09AD92E-4E91-490F-AB99-E69939B840B2 {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/LimitsDxe/LimitsDxe.efi
            SECTION UI = "LimitsDxe"
    }

    INF Binaries/9707f/QcomPkg/Drivers/GpiDxe/GpiDxe.inf

    FILE DRIVER = 3ADF8DDA-1850-44C5-8C63-BB991849BC6F {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/HashDxe/HashDxe.efi
            SECTION UI = "HashDxe"
    }

    FILE DRIVER = 5F0937CD-73F9-445E-B320-BB6FD5D3CBDE {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/CipherDxe/CipherDxe.efi
            SECTION UI = "CipherDxe"
    }

    FILE DRIVER = 20583A66-3C26-4D9E-9A8C-E2DFA2914015 {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/MacDxe/MacDxe.efi
            SECTION UI = "MacDxe"
    }

    FILE DRIVER = B0D3689E-11F8-43C6-8ECE-023A29CEC35B {
            SECTION PE32 = Binaries/9707f/QcomPkg/Drivers/RNGDxe/RngDxe.efi
            SECTION UI = "RngDxe"
    }

    INF Binaries/9707f/QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf

    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
    INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
