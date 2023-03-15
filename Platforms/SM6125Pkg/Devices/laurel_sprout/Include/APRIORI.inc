APRIORI DXE {

    # First, install the PCD driver and call DxeInit
    INF MdeModulePkg/Core/Dxe/DxeMain.inf
    INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

    FILE DRIVER = 90A49AFD-422F-08AE-9611-E788D3804845 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/EnvDxe/EnvDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/EnvDxe/EnvDxe.efi
      SECTION UI = "EnvDxe"
    }

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

    FILE DRIVER = F541D663-4A48-40AA-AABF-FF158CCAE34C {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/SmemDxe/SmemDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/SmemDxe/SmemDxe.efi
      SECTION UI = "SmemDxe"
    }

    FILE DRIVER = 8E9BD160-B184-11DF-94E2-0800200C9A66 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/DALSys/DALSys.depex
      SECTION PE32 = Binaries/laurel_sprout/DALSys/DALSys.efi
      SECTION UI = "DALSys"
    }

    FILE DRIVER = AF9763A2-033B-4109-8E17-56A98D380C92 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/HWIODxeDriver/HWIODxeDriver.depex
      SECTION PE32 = Binaries/laurel_sprout/HWIODxeDriver/HWIODxeDriver.efi
      SECTION UI = "HWIODxeDriver"
    }

    FILE DRIVER = 10E193DF-9966-44E7-B17C-59DD831E20FC {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ChipInfo/ChipInfo.depex
      SECTION PE32 = Binaries/laurel_sprout/ChipInfo/ChipInfo.efi
      SECTION UI = "ChipInfo"
    }

    FILE DRIVER = B105211B-BBBD-4ADD-A3B0-D1CF4A52154C {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/PlatformInfoDxeDriver/PlatformInfoDxeDriver.depex
      SECTION PE32 = Binaries/laurel_sprout/PlatformInfoDxeDriver/PlatformInfoDxeDriver.efi
      SECTION UI = "PlatformInfoDxeDriver"
    }

    FILE DRIVER = 9A00771F-36D4-4DD5-8916-C48ED9B16B86 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/HALIOMMU/HALIOMMU.depex
      SECTION PE32 = Binaries/laurel_sprout/HALIOMMU/HALIOMMU.efi
      SECTION UI = "HALIOMMU"
    }

    FILE DRIVER = 7942A7EB-B7D5-4E2E-B908-831E4DE55b58 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/GlinkDxe/GlinkDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/GlinkDxe/GlinkDxe.efi
      SECTION UI = "GlinkDxe"
    }

    FILE DRIVER = E43128A8-8692-42B6-8AFA-676158578D18 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ULogDxe/ULogDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ULogDxe/ULogDxe.efi
      SECTION UI = "ULogDxe"
    }

    FILE DRIVER = CB29F4D1-7F37-4692-A416-93E82E219711 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/NpaDxe/NpaDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/NpaDxe/NpaDxe.efi
      SECTION UI = "NpaDxe"
    }

    FILE DRIVER = 4DB5DEA6-5302-4D1A-8A82-677A683B0D29 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ClockDxe/ClockDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ClockDxe/ClockDxe.efi
      SECTION UI = "ClockDxe"
    }

    FILE DRIVER = 9C1EB71F-DD6C-4ED5-9F6A-5CC0CA789F16 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ShmBridgeDxe/ShmBridgeDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ShmBridgeDxe/ShmBridgeDxe.efi
      SECTION UI = "ShmBridgeDxe"
    }

    FILE DRIVER = 2D7A83E3-E343-4FFB-9109-631F2EE11694 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ScmDxe/ScmDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ScmDxe/ScmDxe.efi
      SECTION UI = "ScmDxe"
    }

    FILE DRIVER = 6B38F7B4-AD98-40e9-9093-ACA2B5A253C4 {
      SECTION PE32 = Binaries/laurel_sprout/DiskIoDxe/DiskIoDxe.efi
      SECTION UI = "DiskIoDxe"
    }

    INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
    INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
    INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf

    FILE DRIVER = F10F76DB-42C1-533F-34A8-69BE24653110 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/SdccDxe/SdccDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/SdccDxe/SdccDxe.efi
      SECTION UI = "SdccDxe"
    }

    FILE DRIVER = 0D35CD8E-97EA-4F9A-96AF-0F0D89F76567 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/UFSDxe/UFSDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/UFSDxe/UFSDxe.efi
      SECTION UI = "UFSDxe"
    }

    INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
    INF FatPkg/EnhancedFatDxe/Fat.inf

    FILE DRIVER = 6925FAD3-E243-4B69-AD61-3F978C8ABC60 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/TzDxe/TzDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/TzDxe/TzDxe.efi
      SECTION UI = "TzDxe"
    }

    INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

    FILE DRIVER = 040E1E61-0AFB-411B-9EC9-00F3FC59CC13 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/QcomWDogDxe/QcomWDogDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/QcomWDogDxe/QcomWDogDxe.efi
      SECTION UI = "QcomWDogDxe"
    }

    FILE DRIVER = 8681CC5A-0DF6-441E-B4B8-E915C538F067 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/DALTLMM/DALTLMM.depex
      SECTION PE32 = Binaries/laurel_sprout/DALTLMM/DALTLMM.efi
      SECTION UI = "DALTLMM"
    }

    FILE DRIVER = 2A7B4BEF-80CD-49E1-B473-374BA4D673FC {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/SPMI/SPMI.depex
      SECTION PE32 = Binaries/laurel_sprout/SPMI/SPMI.efi
      SECTION UI = "SPMI"
    }

    FILE DRIVER = 3AE17DB7-3CC5-4B89-9304-9549211057EF {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ResetRuntimeDxe/ResetRuntimeDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ResetRuntimeDxe/ResetRuntimeDxe.efi
      SECTION UI = "ResetRuntimeDxe"
    }

    FILE DRIVER = 5776232E-082D-4B75-9A0E-FE1D13F7A5D9 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/PmicDxe/PmicDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/PmicDxe/PmicDxe.efi
      SECTION UI = "PmicDxe"
    }

    FILE DRIVER = 1D317734-4083-47C6-83EA-E3A42F2A6Bff {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/PILDxe/PILDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/PILDxe/PILDxe.efi
      SECTION UI = "PILDxe"
    }

    INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
    INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

    FILE DRIVER = FCABE6A7-7953-4A84-B7EC-D29E89B62E87 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.depex
      SECTION PE32 = Binaries/laurel_sprout/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.efi
      SECTION UI = "EmbeddedMonotonicCounter"
    }

    INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
    INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
    INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
    INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
    INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

    FILE DRIVER = ed15ffd5-bc96-4107-9118-95366600188e {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/FontDxe/FontDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/FontDxe/FontDxe.efi
      SECTION UI = "FontDxe"
    }

    INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

    FILE DRIVER = 569ea0de-b557-4043-84cf-01103fe516e5 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/GpiDxe/GpiDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/GpiDxe/GpiDxe.efi
      SECTION UI = "GpiDxe"
    }

    FILE DRIVER = 04DE8591-D2B3-4077-BBBE-B12070094EB6 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/I2C/I2C.depex
      SECTION PE32 = Binaries/laurel_sprout/I2C/I2C.efi
      SECTION UI = "I2C"
    }

    FILE DRIVER = 5a5cb8ca-9382-4e0c-b383-77fb517cd9eb {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/AdcDxe/AdcDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/AdcDxe/AdcDxe.efi
      SECTION UI = "AdcDxe"
    }

    FILE DRIVER = 11faed4c-b21f-4d88-8e48-c4c28a1e50df {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/UsbPwrCtrlDxe/UsbPwrCtrlDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/UsbPwrCtrlDxe/UsbPwrCtrlDxe.efi
      SECTION UI = "UsbPwrCtrlDxe"
    }

    FILE DRIVER = 4BCE7F36-428E-4393-99E3-7E0844404DBA {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/QcomChargerDxeLA/QcomChargerDxeLA.depex
      SECTION PE32 = Binaries/laurel_sprout/QcomChargerDxeLA/QcomChargerDxeLA.efi
      SECTION UI = "QcomChargerDxeLA"
    }

    FILE DRIVER = 2E3042F3-2BB9-463F-8866-56D75B7C4AB0 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ChargerExDxe/ChargerExDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ChargerExDxe/ChargerExDxe.efi
      SECTION UI = "ChargerExDxe"
    }

    FILE DRIVER = 94f8a6a7-dc34-4101-88c1-99179cceae83 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/UsbfnDwc3Dxe/UsbfnDwc3Dxe.depex
      SECTION PE32 = Binaries/laurel_sprout/UsbfnDwc3Dxe/UsbfnDwc3Dxe.efi
      SECTION UI = "UsbfnDwc3Dxe"
    }

    INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
    INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

    FILE DRIVER = cd823a4d-7dec-4531-ae5d-4134fa4127b8 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/UsbConfigDxe/UsbConfigDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/UsbConfigDxe/UsbConfigDxe.efi
      SECTION UI = "UsbConfigDxe"
    }

    FILE DRIVER = 5bd181db-0487-4f1a-ae73-820e165611b3 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/ButtonsDxe/ButtonsDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/ButtonsDxe/ButtonsDxe.patched.efi
      SECTION UI = "ButtonsDxe"
    }

    FILE DRIVER = f09ad92e-485c-9cdb-3e05-3385c38f94cc {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/TsensDxe/TsensDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/TsensDxe/TsensDxe.efi
      SECTION UI = "TsensDxe"
    }

!if $(USE_DISPLAYDXE) == 1
    FILE DRIVER = 4138022F-06C7-4F79-9C94-7E33B511A4E7 {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/DisplayDxe/DisplayDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/DisplayDxe/DisplayDxe.efi
      SECTION UI = "DisplayDxe"
    }
!else
    INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

    FILE DRIVER = 3adf8dda-1850-44c5-8c63-bb991849bc6f {
      SECTION PE32 = Binaries/laurel_sprout/HashDxe/HashDxe.efi
      SECTION UI = "HashDxe"
    }

    FILE DRIVER = 5f0937cd-73f9-445e-b320-bb6fd5d3cbde {
      SECTION PE32 = Binaries/laurel_sprout/CipherDxe/CipherDxe.efi
      SECTION UI = "CipherDxe"
    }

    FILE DRIVER = b0d3689e-11f8-43c6-8ece-023a29cec35b {
      SECTION PE32 = Binaries/laurel_sprout/RngDxe/RngDxe.efi
      SECTION UI = "RngDxe"
    }
    
    FILE DRIVER = 7DB0793A-4402-4BE1-906E-D0FABAD2707E {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/DDRInfoDxe/DDRInfoDxe.depex
      SECTION PE32 = Binaries/laurel_sprout/DDRInfoDxe/DDRInfoDxe.efi
      SECTION UI = "DDRInfoDxe"
    }

    FILE DRIVER = 6696936D-3637-467C-87CB-14EA8248948C {
      SECTION DXE_DEPEX = Binaries/laurel_sprout/SimpleTextInOutSerial/SimpleTextInOutSerial.depex
      SECTION PE32 = Binaries/laurel_sprout/SimpleTextInOutSerial/SimpleTextInOutSerial.efi
      SECTION UI = "SimpleTextInOutSerial"
    }

    INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
    INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

    FILE DRIVER = cccb0c28-4b24-11d5-9a5a-0090273fc14d {
      SECTION PE32 = Binaries/laurel_sprout/GraphicsConsoleDxe/GraphicsConsoleDxe.efi
      SECTION UI = "GraphicsConsoleDxe"
    }
}
