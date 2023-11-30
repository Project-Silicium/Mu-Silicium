APRIORI DXE {
	# First, install the PCD driver and call DxeInit
	INF MdeModulePkg/Core/Dxe/DxeMain.inf
	INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

	INF Binaries/alioth/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

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

	INF Binaries/alioth/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/HALIOMMUDxe/HALIOMMUDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/PdcDxe/PdcDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxeLA.inf
	INF Binaries/alioth/QcomPkg/Drivers/TzDxe/ScmDxeLA.inf
	INF Binaries/alioth/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/ResetRuntimeDxe/ResetRuntimeDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf

	FILE DRIVER = 6B38F7B4-AD98-40E9-9093-ACA2B5A253C4 {
		SECTION PE32 = Binaries/alioth/MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.efi
		SECTION UI = "DiskIoDxe"
	}

	FILE DRIVER = 1FA1F39E-FEFF-4AAE-BD7B-38A070A3B609 {
		SECTION PE32 = Binaries/alioth/MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.efi
		SECTION UI = "PartitionDxe"
	}

	INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
	INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

	INF Binaries/alioth/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/UFSDxe/UFSDxe.inf

	INF FatPkg/EnhancedFatDxe/Fat.inf
	INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf

	INF Binaries/alioth/QcomPkg/Drivers/TzDxe/TzDxeLA.inf

	INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

	FILE DRIVER = 1C04F529-0612-4319-9315-B2E75C16810A {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.efi
		SECTION UI = "FeatureEnablerDxe"
	}

	INF Binaries/alioth/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/SPIDxe/SPIDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/I2CDxe/I2CDxe.inf

	!if $(USE_DISPLAYDXE) == 1
		INF Binaries/alioth/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
	!else
		INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
	!endif

	INF Binaries/alioth/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/PILDxe/PILDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf

	!if $(USE_DISPLAYDXE) == 1
		INF Binaries/alioth/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
	!endif

	INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf

	INF Binaries/alioth/QcomPkg/Drivers/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf

	INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
	INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
	INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
	INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
	INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

	INF Binaries/alioth/QcomPkg/Drivers/FontDxe/FontDxe.inf

	INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

	INF Binaries/alioth/QcomPkg/Drivers/VibratorDxe/VibratorDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
	INF Binaries/alioth/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf

	FILE DRIVER = B7F50E91-A759-412C-ADE4-DCD03E7F7C28 {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/XhciDxe/XhciDxe.efi
		SECTION UI = "XhciDxe"
	}

	FILE DRIVER = 240612B7-A063-11D4-9A3A-0090273FC14D {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/UsbBusDxe/UsbBusDxe.efi
		SECTION UI = "UsbBusDxe"
	}

	FILE DRIVER = 2D2E62CF-9ECF-43B7-8219-94E7FC713DFE {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/UsbKbDxe/UsbKbDxe.efi
		SECTION UI = "UsbKbDxe"
	}

	FILE DRIVER = 9FB4B4A7-42C0-4BCD-8540-9BCC6711F83E {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/UsbMassStorageDxe/UsbMassStorageDxe.efi
		SECTION UI = "UsbMassStorageDxe"
	}

	INF Binaries/alioth/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
	INF Binaries/alioth/QcomPkg/Drivers/TsensDxe/TsensDxe.inf

	FILE DRIVER = F09AD92E-4E91-490F-AB99-E69939B840B2 {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/LimitsDxe/LimitsDxe.efi
		SECTION UI = "LimitsDxe"
	}

	INF Binaries/alioth/QcomPkg/Drivers/GpiDxe/GpiDxe.inf

	FILE DRIVER = 3ADF8DDA-1850-44C5-8C63-BB991849BC6F {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/HashDxe/HashDxe.efi
		SECTION UI = "HashDxe"
	}

	FILE DRIVER = 5F0937CD-73F9-445E-B320-BB6FD5D3CBDE {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/CipherDxe/CipherDxe.efi
		SECTION UI = "CipherDxe"
	}

	FILE DRIVER = 20583A66-3C26-4D9E-9A8C-E2DFA2914015 {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/MacDxe/MacDxe.efi
		SECTION UI = "MacDxe"
	}

	FILE DRIVER = B0D3689E-11F8-43C6-8ECE-023A29CEC35B {
		SECTION PE32 = Binaries/alioth/QcomPkg/Drivers/RNGDxe/RngDxe.efi
		SECTION UI = "RngDxe"
	}

	INF Binaries/alioth/QcomPkg/Drivers/SimpleTextInOutSerialDxe/SimpleTextInOutSerial.inf

	INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
	INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
	INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
}
