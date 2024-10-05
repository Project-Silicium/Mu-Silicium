APRIORI DXE {
    # First, install the PCD driver and call DxeInit
    INF MdeModulePkg/Core/Dxe/DxeMain.inf
    INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

    INF Binaries/ice/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

    INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
    INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
    INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

    # Next, install cpu protocol and enable the interrupt controller
    INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
    INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf

    # Core Drivers
    INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

    INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/SmemDxe/SmemDxe.inf

    INF QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
    INF Binaries/ice/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
    INF Binaries/ice/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

    INF Binaries/ice/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
    INF Binaries/ice/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF Binaries/ice/MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF Binaries/ice/MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

    INF FatPkg/EnhancedFatDxe/Fat.inf

    INF Binaries/ice/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/FvUtilsDxe/FvUtilsDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/PILDxe/PILDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
    INF Binaries/ice/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf

    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf

    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf

    INF Binaries/ice/MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/FontDxe/FontDxe.inf

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/OcdtDxe/OppoProject.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
    INF Binaries/ice/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
    INF Binaries/ice/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/XhciDxe/XhciDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbBusDxe/UsbBusDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbKbDxe/UsbKbDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbMassStorageDxe/UsbMassStorageDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
    INF Binaries/ice/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
    INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    INF Binaries/ice/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/UCDxe/UCDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/RNGDxe/RngDxe.inf
    INF Binaries/ice/QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf

    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
    INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

    INF Binaries/ice/QcomPkg/Drivers/OGaugeAuthDxe/OGaugeAuth.inf
}
