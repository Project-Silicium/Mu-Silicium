  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  FILE DRIVER = 9C1EB71F-DD6C-4ED5-9F6A-5CC0CA789F16 {
    SECTION DXE_DEPEX = Binaries/vili/ShmBridgeDxeLA/ShmBridgeDxeLA.depex
    SECTION PE32 = Binaries/vili/ShmBridgeDxeLA/ShmBridgeDxeLA.efi
    SECTION UI = "ShmBridgeDxeLA"
  }

  FILE DRIVER = 2D7A83E3-E343-4FFB-9109-631F2EE11694 {
    SECTION DXE_DEPEX = Binaries/vili/ScmDxeLA/ScmDxeLA.depex
    SECTION PE32 = Binaries/vili/ScmDxeLA/ScmDxeLA.efi
    SECTION UI = "ScmDxeLA"
  }

  FILE DRIVER = 6925FAD3-E243-4B69-AD61-3F978C8ABC60 {
    SECTION DXE_DEPEX = Binaries/vili/TzDxeLA/TzDxeLA.depex
    SECTION PE32 = Binaries/vili/TzDxeLA/TzDxeLA.efi
    SECTION UI = "TzDxeLA"
  }

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf

  FILE DRIVER = FCABE6A7-7953-4A84-B7EC-D29E89B62E87 {
    SECTION DXE_DEPEX = Binaries/vili/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.depex
    SECTION PE32 = Binaries/vili/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.efi
    SECTION UI = "EmbeddedMonotonicCounter"
  }

  INF MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

  FILE DRIVER = 3AE17DB7-3CC5-4B89-9304-9549211057EF {
    SECTION DXE_DEPEX = Binaries/vili/ResetRuntimeDxe/ResetRuntimeDxe.depex
    SECTION PE32 = Binaries/vili/ResetRuntimeDxe/ResetRuntimeDxe.efi
    SECTION UI = "ResetRuntimeDxe"
  }

  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  FILE DRIVER = ed15ffd5-bc96-4107-9118-95366600188e {
    SECTION DXE_DEPEX = Binaries/vili/FontDxe/FontDxe.depex
    SECTION PE32 = Binaries/vili/FontDxe/FontDxe.efi
    SECTION UI = "FontDxe"
  }

  FILE DRIVER = 040E1E61-0AFB-411B-9EC9-00F3FC59CC13 {
    SECTION DXE_DEPEX = Binaries/vili/QcomWDogDxe/QcomWDogDxe.depex
    SECTION PE32 = Binaries/vili/QcomWDogDxe/QcomWDogDxe.patched.efi
    SECTION UI = "QcomWDogDxe"
  }

  INF ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF ArmPkg/Drivers/ArmPsciMpServicesDxe/ArmPsciMpServicesDxe.inf
    
  FILE DRIVER = 10E193DF-9966-44E7-B17C-59DD831E20FC {
    SECTION DXE_DEPEX = Binaries/vili/ChipInfo/ChipInfo.depex
    SECTION PE32 = Binaries/vili/ChipInfo/ChipInfo.efi
    SECTION UI = "ChipInfo"
  }

  FILE DRIVER = B105211B-BBBD-4ADD-A3B0-D1CF4A52154C {
    SECTION DXE_DEPEX = Binaries/vili/PlatformInfoDxeDriver/PlatformInfoDxeDriver.depex
    SECTION PE32 = Binaries/vili/PlatformInfoDxeDriver/PlatformInfoDxeDriver.efi
    SECTION UI = "PlatformInfoDxeDriver"
  }

  FILE DRIVER = 0A8D6360-7EF3-11EA-B20F-B3A524584400 {
    SECTION DXE_DEPEX = Binaries/vili/ProjectInfoDxeDriver/ProjectInfoDxeDriver.depex
    SECTION PE32 = Binaries/vili/ProjectInfoDxeDriver/ProjectInfoDxeDriver.efi
    SECTION UI = "ProjectInfoDxeDriver"
  }

  FILE DRIVER = 8E9BD160-B184-11DF-94E2-0800200C9A66 {
    SECTION DXE_DEPEX = Binaries/vili/DALSys/DALSys.depex
    SECTION PE32 = Binaries/vili/DALSys/DALSys.efi
    SECTION UI = "DALSys"
  }

  FILE DRIVER = 9A00771F-36D4-4DD5-8916-C48ED9B16B86 {
    SECTION DXE_DEPEX = Binaries/vili/HALIOMMU/HALIOMMU.depex
    SECTION PE32 = Binaries/vili/HALIOMMU/HALIOMMU.efi
    SECTION UI = "HALIOMMU"
  }

  FILE DRIVER = AF9763A2-033B-4109-8E17-56A98D380C92 {
    SECTION DXE_DEPEX = Binaries/vili/HWIODxeDriver/HWIODxeDriver.depex
    SECTION PE32 = Binaries/vili/HWIODxeDriver/HWIODxeDriver.efi
    SECTION UI = "HWIODxeDriver"
  }

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  FILE DRIVER = 6B38F7B4-AD98-40e9-9093-ACA2B5A253C4 {
    SECTION PE32 = Binaries/vili/DiskIoDxe/DiskIoDxe.efi
    SECTION UI = "DiskIoDxe"
  }

  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  INF MdeModulePkg/Universal/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  INF AdvLoggerPkg/AdvancedFileLogger/AdvancedFileLogger.inf

  FILE DRIVER = 0D35CD8E-97EA-4F9A-96AF-0F0D89F76567 {
    SECTION DXE_DEPEX = Binaries/vili/UFSDxe/UFSDxe.depex
    SECTION PE32 = Binaries/vili/UFSDxe/UFSDxe.patched.efi
    SECTION UI = "UFSDxe"
  }

  FILE DRIVER = ABA01FF8-2CCB-4E12-8B2E-CD3F4A742993 {
    SECTION DXE_DEPEX = Binaries/vili/CmdDbDxe/CmdDbDxe.depex
    SECTION PE32 = Binaries/vili/CmdDbDxe/CmdDbDxe.efi
    SECTION UI = "CmdDbDxe"
  }

  FILE DRIVER = 0401B830-BFF9-44A2-A5F6-95734A33C017 {
    SECTION DXE_DEPEX = Binaries/vili/PwrUtilsDxe/PwrUtilsDxe.depex
    SECTION PE32 = Binaries/vili/PwrUtilsDxe/PwrUtilsDxe.efi
    SECTION UI = "PwrUtilsDxe"
  }

  FILE DRIVER = CB29F4D1-7F37-4692-A416-93E82E219766 {
    SECTION DXE_DEPEX = Binaries/vili/RpmhDxe/RpmhDxe.depex
    SECTION PE32 = Binaries/vili/RpmhDxe/RpmhDxe.efi
    SECTION UI = "RpmhDxe"
  }

  FILE DRIVER = CB29F4D1-7F37-4692-A416-93E82E219711 {
    SECTION DXE_DEPEX = Binaries/vili/NpaDxe/NpaDxe.depex
    SECTION PE32 = Binaries/vili/NpaDxe/NpaDxe.efi
    SECTION UI = "NpaDxe"
  }

  FILE DRIVER = E43128A8-8692-42B6-8AFA-676158578D18 {
    SECTION DXE_DEPEX = Binaries/vili/ULogDxe/ULogDxe.depex
    SECTION PE32 = Binaries/vili/ULogDxe/ULogDxe.efi
    SECTION UI = "ULogDxe"
  }

  FILE DRIVER = 8BD3B475-401A-4B0B-9315-EDEE61A1EAE5 {
    SECTION DXE_DEPEX = Binaries/vili/VcsDxe/VcsDxe.depex
    SECTION PE32 = Binaries/vili/VcsDxe/VcsDxe.efi
    SECTION UI = "VcsDxe"
  }

  FILE DRIVER = 4DB5DEA6-5302-4D1A-8A82-677A683B0D29 {
    SECTION DXE_DEPEX = Binaries/vili/ClockDxe/ClockDxe.depex
    SECTION PE32 = Binaries/vili/ClockDxe/ClockDxe.patched.efi
    SECTION UI = "ClockDxe"
  }

  FILE DRIVER = 5824F9DE-17D2-4B1F-A5FE-5EB77FA53093 {
    SECTION DXE_DEPEX = Binaries/vili/ICBDxe/ICBDxe.depex
    SECTION PE32 = Binaries/vili/ICBDxe/ICBDxe.efi
    SECTION UI = "ICBDxe"
  }

  FILE DRIVER = F541D663-4A48-40AA-AABF-FF158CCAE34C {
    SECTION DXE_DEPEX = Binaries/vili/SmemDxe/SmemDxe.depex
    SECTION PE32 = Binaries/vili/SmemDxe/SmemDxe.efi
    SECTION UI = "SmemDxe"
  }

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  FILE DRIVER = 6696936D-3637-467C-87CB-14EA8248948C {
    SECTION DXE_DEPEX = Binaries/vili/SimpleTextInOutSerial/SimpleTextInOutSerial.depex
    SECTION PE32 = Binaries/vili/SimpleTextInOutSerial/SimpleTextInOutSerial.efi
    SECTION UI = "SimpleTextInOutSerial"
  }

  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf

  FILE DRIVER = cccb0c28-4b24-11d5-9a5a-0090273fc14d {
    SECTION PE32 = Binaries/vili/GraphicsConsoleDxe/GraphicsConsoleDxe.efi
    SECTION UI = "GraphicsConsoleDxe"
  }

!if $(USE_DISPLAYDXE) == 1
  FILE DRIVER = 1B52138D-3FA3-4E50-B958-20887353F809 {
    SECTION DXE_DEPEX = Binaries/vili/CPRDxe/CPRDxe.depex
    SECTION PE32 = Binaries/vili/CPRDxe/CPRDxe.efi
    SECTION UI = "CPRDxe"
  }
!endif

  FILE DRIVER = 569ea0de-b557-4043-84cf-01103fe516e5 {
    SECTION DXE_DEPEX = Binaries/vili/GpiDxe/GpiDxe.depex
    SECTION PE32 = Binaries/vili/GpiDxe/GpiDxe.efi
    SECTION UI = "GpiDxe"
  }

  FILE DRIVER = 04DE8591-D2B3-4077-BBBE-B12070094EB6 {
    SECTION DXE_DEPEX = Binaries/vili/I2C/I2C.depex
    SECTION PE32 = Binaries/vili/I2C/I2C.efi
    SECTION UI = "I2C"
  }

  FILE DRIVER = 7A32BD23-F735-4F57-AA1A-447D2FE3BE0D {
    SECTION DXE_DEPEX = Binaries/vili/SPI/SPI.depex
    SECTION PE32 = Binaries/vili/SPI/SPI.efi
    SECTION UI = "SPI"
  }

  FILE DRIVER = 2A7B4BEF-80CD-49E1-B473-374BA4D673FC {
    SECTION DXE_DEPEX = Binaries/vili/SPMI/SPMI.depex
    SECTION PE32 = Binaries/vili/SPMI/SPMI.efi
    SECTION UI = "SPMI"
  }

  FILE DRIVER = 5776232E-082D-4B75-9A0E-FE1D13F7A5D9 {
    SECTION DXE_DEPEX = Binaries/vili/PmicDxe/PmicDxe.depex
    SECTION PE32 = Binaries/vili/PmicDxe/PmicDxe.patched.efi
    SECTION UI = "PmicDxe"
  }

  FILE DRIVER = F10F76DB-42C1-533F-34A8-69BE24653110 {
    SECTION DXE_DEPEX = Binaries/vili/SdccDxe/SdccDxe.depex
    SECTION PE32 = Binaries/vili/SdccDxe/SdccDxe.efi
    SECTION UI = "SdccDxe"
  }

  FILE DRIVER = 8681CC5A-0DF6-441E-B4B8-E915C538F067 {
    SECTION DXE_DEPEX = Binaries/vili/DALTLMM/DALTLMM.depex
    SECTION PE32 = Binaries/vili/DALTLMM/DALTLMM.efi
    SECTION UI = "DALTLMM"
  }

  FILE DRIVER = 1C04F529-0612-4319-9315-B2E75C16810A {
    SECTION PE32 = Binaries/vili/FeatureEnablerDxe/FeatureEnablerDxe.patched.efi
    SECTION UI = "FeatureEnablerDxe"
  }

  FILE DRIVER = 2A0ECDCE-01AE-446E-9FFD-C4217DE0340F {
    SECTION DXE_DEPEX = Binaries/vili/MinidumpTADxe/MinidumpTADxe.depex
    SECTION PE32 = Binaries/vili/MinidumpTADxe/MinidumpTADxe.patched.efi
    SECTION UI = "MinidumpTADxe"
  }

!if $(USE_DISPLAYDXE) == 1
  FILE DRIVER = 4138022F-06C7-4F79-9C94-7E33B511A4E7 {
    SECTION DXE_DEPEX = Binaries/vili/DisplayDxe/DisplayDxe.depex
    SECTION PE32 = Binaries/vili/DisplayDxe/DisplayDxe.efi
    SECTION UI = "DisplayDxe"
  }
!else
  INF QcomPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf
  INF MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  INF CryptoPkg/Driver/CryptoDxe.inf

  FILE DRIVER = 5bd181db-0487-4f1a-ae73-820e165611b3 {
    SECTION DXE_DEPEX = Binaries/vili/ButtonsDxe/ButtonsDxe.depex
    SECTION PE32 = Binaries/vili/ButtonsDxe/ButtonsDxe.patched.efi
    SECTION UI = "ButtonsDxe"
  }

  FILE DRIVER = 2E3042F3-2BB9-463F-8866-56D75B7C4AB0 {
    SECTION DXE_DEPEX = Binaries/vili/ChargerExDxe/ChargerExDxe.depex
    SECTION PE32 = Binaries/vili/ChargerExDxe/ChargerExDxe.efi
    SECTION UI = "ChargerExDxe"
  }

  FILE DRIVER = 42031F68-1F0B-429B-93F1-0EB0CF89DDBF {
    SECTION DXE_DEPEX = Binaries/vili/IPCCDxe/IPCCDxe.depex
    SECTION PE32 = Binaries/vili/IPCCDxe/IPCCDxe.efi
    SECTION UI = "IPCCDxe"
  }

  FILE DRIVER = 7942A7EB-B7D5-4E2E-B908-831E4DE55b58 {
    SECTION DXE_DEPEX = Binaries/vili/GlinkDxe/GlinkDxe.depex
    SECTION PE32 = Binaries/vili/GlinkDxe/GlinkDxe.efi
    SECTION UI = "GlinkDxe"
  }

  FILE DRIVER = 6d4a9cf7-cc0c-42b0-aa89-f6a6a8d99273 {
    SECTION DXE_DEPEX = Binaries/vili/PmicGlinkDxe/PmicGlinkDxe.depex
    SECTION PE32 = Binaries/vili/PmicGlinkDxe/PmicGlinkDxe.efi
    SECTION UI = "PmicGlinkDxe"
  }

  FILE DRIVER = 4BCE7F36-428E-4393-99E3-7E0844404DBA {
    SECTION DXE_DEPEX = Binaries/vili/QcomChargerDxeLA/QcomChargerDxeLA.depex
    SECTION PE32 = Binaries/vili/QcomChargerDxeLA/QcomChargerDxeLA.efi
    SECTION UI = "QcomChargerDxeLA"
  }

  FILE DRIVER = 11faed4c-b21f-4d88-8e48-c4c28a1e50df {
    SECTION DXE_DEPEX = Binaries/vili/UsbPwrCtrlDxe/UsbPwrCtrlDxe.depex
    SECTION PE32 = Binaries/vili/UsbPwrCtrlDxe/UsbPwrCtrlDxe.efi
    SECTION UI = "UsbPwrCtrlDxe"
  }

  FILE DRIVER = f09ad92e-485c-9cdb-3e05-3385c38f94cc {
    SECTION DXE_DEPEX = Binaries/vili/TsensDxe/TsensDxe.depex
    SECTION PE32 = Binaries/vili/TsensDxe/TsensDxe.efi
    SECTION UI = "TsensDxe"
  }

  FILE DRIVER = 7DB0793A-4402-4BE1-906E-D0FABAD2707E {
    SECTION DXE_DEPEX = Binaries/vili/DDRInfoDxe/DDRInfoDxe.depex
    SECTION PE32 = Binaries/vili/DDRInfoDxe/DDRInfoDxe.efi
    SECTION UI = "DDRInfoDxe"
  }

  FILE DRIVER = 94f8a6a7-dc34-4101-88c1-99179cceae83 {
    SECTION DXE_DEPEX = Binaries/vili/UsbfnDwc3Dxe/UsbfnDwc3Dxe.depex
    SECTION PE32 = Binaries/vili/UsbfnDwc3Dxe/UsbfnDwc3Dxe.efi
    SECTION UI = "UsbfnDwc3Dxe"
  }

  FILE DRIVER = BEB12BEE-F6E1-11E1-9FB8-6C626DE4AEB1 {
    SECTION DXE_DEPEX = Binaries/vili/XhciPciEmulation/XhciPciEmulation.depex
    SECTION PE32 = Binaries/vili/XhciPciEmulation/XhciPciEmulation.efi
    SECTION UI = "XhciPciEmulation"
  }

  FILE DRIVER = b7f50e91-a759-412c-ade4-dcd03e7f7c28 {
    SECTION PE32 = Binaries/vili/XhciDxe/XhciDxe.efi
    SECTION UI = "XhciDxe"
  }

  FILE DRIVER = 240612b7-a063-11d4-9a3a-0090273fc14d {
    SECTION PE32 = Binaries/vili/UsbBusDxe/UsbBusDxe.efi
    SECTION UI = "UsbBusDxe"
  }

  FILE DRIVER = 2d2e62cf-9ecf-43b7-8219-94e7fc713dfe {
    SECTION PE32 = Binaries/vili/UsbKbDxe/UsbKbDxe.efi
    SECTION UI = "UsbKbDxe"
  }

  INF MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf

  FILE DRIVER = 9fb4b4a7-42c0-4bcd-8540-9bcc6711f83e {
    SECTION PE32 = Binaries/vili/UsbMassStorageDxe/UsbMassStorageDxe.efi
    SECTION UI = "UsbMassStorageDxe"
  }

  FILE DRIVER = 5af77f10-90df-4e7e-8325-a17ec09d5443 {
    SECTION DXE_DEPEX = Binaries/vili/UsbMsdDxe/UsbMsdDxe.depex
    SECTION PE32 = Binaries/vili/UsbMsdDxe/UsbMsdDxe.efi
    SECTION UI = "UsbMsdDxe"
  }

  FILE DRIVER = 3299a266-15f0-4346-8318-716336736d3e {
    SECTION DXE_DEPEX = Binaries/vili/UsbDeviceDxe/UsbDeviceDxe.depex
    SECTION PE32 = Binaries/vili/UsbDeviceDxe/UsbDeviceDxe.efi
    SECTION UI = "UsbDeviceDxe"
  }

  FILE DRIVER = cd823a4d-7dec-4531-ae5d-4134fa4127b8 {
    SECTION DXE_DEPEX = Binaries/vili/UsbConfigDxe/UsbConfigDxe.depex
    SECTION PE32 = Binaries/vili/UsbConfigDxe/UsbConfigDxe.patched.efi
    SECTION UI = "UsbConfigDxe"
  }

  FILE DRIVER = 0A134F0E-075E-40B3-9C63-3B3906804663 {
    SECTION DXE_DEPEX = Binaries/vili/UsbInitDxe/UsbInitDxe.depex
    SECTION PE32 = Binaries/vili/UsbInitDxe/UsbInitDxe.efi
    SECTION UI = "UsbInitDxe"
  }

  FILE DRIVER = 5f0937cd-73f9-445e-b320-bb6fd5d3cbde {
    SECTION PE32 = Binaries/vili/UCDxe/UCDxe.efi
    SECTION UI = "UCDxe"
  }

  FILE DRIVER = b0d3689e-11f8-43c6-8ece-023a29cec35b {
    SECTION PE32 = Binaries/vili/RngDxe/RngDxe.efi
    SECTION UI = "RngDxe"
  }

  FILE DRIVER = 1D317734-4083-47C6-83EA-E3A42F2A6Bff {
    SECTION DXE_DEPEX = Binaries/vili/PILDxe/PILDxe.depex
    SECTION PE32 = Binaries/vili/PILDxe/PILDxe.efi
    SECTION UI = "PILDxe"
  }

  FILE DRIVER = 8CF9C920-D5E6-4DAC-BEF9-6E6A4EEC7ADD {
    SECTION DXE_DEPEX = Binaries/vili/PILProxyDxe/PILProxyDxe.depex
    SECTION PE32 = Binaries/vili/PILProxyDxe/PILProxyDxe.efi
    SECTION UI = "PILProxyDxe"
  }

  FILE DRIVER = 5D787B6C-6254-43F5-A7A8-CB25329CF97E {
    SECTION DXE_DEPEX = Binaries/vili/SPSSDxe/SPSSDxe.depex
    SECTION PE32 = Binaries/vili/SPSSDxe/SPSSDxe.efi
    SECTION UI = "SPSSDxe"
  }

  FILE DRIVER = E2534A24-1D9A-4A54-AAEC-BE8836D0F45D {
    SECTION DXE_DEPEX = Binaries/vili/FvDxe/FvDxe.depex
    SECTION PE32 = Binaries/vili/FvDxe/FvDxe.efi
    SECTION UI = "FvDxe"
  }

  FILE DRIVER = 4A568B8A-4D9D-4984-9C75-CEE33698E451 {
    SECTION DXE_DEPEX = Binaries/vili/ParserDxe/ParserDxe.depex
    SECTION PE32 = Binaries/vili/ParserDxe/ParserDxe.efi
    SECTION UI = "ParserDxe"
  }

  FILE DRIVER = 84CC57F6-38BE-4ACF-9752-73D1E23F793F {
    SECTION DXE_DEPEX = Binaries/vili/SerialPortDxe/SerialPortDxe.depex
    SECTION PE32 = Binaries/vili/SerialPortDxe/SerialPortDxe.efi
    SECTION UI = "SerialPortDxe"
  }

  FILE APPLICATION = EEE9C2B1-16CA-4F34-87EA-2E6D1E160CC4 {
    SECTION PE32 = Binaries/vili/QcomChargerApp/QcomChargerApp.efi
    SECTION UI = "QcomChargerApp"
  }
