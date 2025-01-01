##
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[BuildOptions]
  *_CLANG*_*_CC_FLAGS = -Wno-unused-const-variable -Wno-sometimes-uninitialized -Wno-unaligned-access -Wno-deprecated-non-prototype -D CLANG
!if $(TARGET) == DEBUG
  *_CLANG*_*_CC_FLAGS = -D EFI_DEBUG
!endif
  *_CLANGPDB_AARCH64_CC_FLAGS = -D __DEPRECATED_AARCH64_4K_RUNTIME_GRANULARITY -D HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA=$(HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA) -D HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA=$(HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA) -D HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA=$(HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA) -D HAS_PSCI_MEMPROTECT_FAILING_ERRATA=$(HAS_PSCI_MEMPROTECT_FAILING_ERRATA)

[BuildOptions.common.EDKII.SEC,BuildOptions.common.EDKII.MM_CORE_STANDALONE]
  *_CLANGDWARF_*_DLINK_FLAGS = -z common-page-size=0x1000
  *_CLANGPDB_*_DLINK_FLAGS = /ALIGN:0x1000 /FILEALIGN:0x1000

[BuildOptions.common.EDKII.DXE_CORE,BuildOptions.common.EDKII.DXE_DRIVER,BuildOptions.common.EDKII.UEFI_DRIVER,BuildOptions.common.EDKII.UEFI_APPLICATION,BuildOptions.common.EDKII.MM_CORE_STANDALONE,BuildOptions.common.EDKII.MM_STANDALONE]
  *_CLANGDWARF_*_DLINK_FLAGS = -z common-page-size=0x1000
  *_CLANGPDB_*_DLINK_FLAGS = /ALIGN:0x1000

[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  *_CLANGDWARF_*_DLINK_FLAGS = -z common-page-size=0x1000
  *_CLANGPDB_*_DLINK_FLAGS = /ALIGN:0x10000

[BuildOptions.common.EDKII.MM_CORE_STANDALONE,BuildOptions.common.EDKII.MM_STANDALONE]
  *_CLANGPDB_AARCH64_CC_FLAGS = -mstrict-align

[PcdsFixedAtBuild.AArch64]
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize|44

[PcdsFixedAtBuild.Arm]
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize|0

[PcdsFeatureFlag]
  gEfiMdePkgTokenSpaceGuid.PcdComponentNameDisable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnosticsDisable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdComponentName2Disable|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDriverDiagnostics2Disable|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  gEmbeddedTokenSpaceGuid.PcdPrePiProduceMemoryTypeInformationHob|TRUE

[PcdsFixedAtBuild]
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"3.0.3"
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Project Silicium"
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|0x01231229
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|"SILICI"
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x324B44454D4F4351
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorId|0x4D4F4351
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorRevision|0x00000001
  gEfiMdePkgTokenSpaceGuid.PcdMaximumUnicodeStringLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdMaximumAsciiStringLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|1000000
  gEfiMdePkgTokenSpaceGuid.PcdSpinLockTimeout|10000000
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|320
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x00

!if $(TARGET) == DEBUG
  gEfiMdePkgTokenSpaceGuid.PcdDebugClearMemoryValue|0xAF
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2f
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x800fee0f
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|1
  gEfiMdePkgTokenSpaceGuid.PcdPostCodePropertyMask|0
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugClearMemoryValue|0x00
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x00
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x00000000
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0
  gEfiMdePkgTokenSpaceGuid.PcdPostCodePropertyMask|0
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE

  #
  # Make VariableRuntimeDxe work at emulated non-volatile variable mode.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvModeEnable|TRUE

  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiACPIReclaimMemory|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiACPIMemoryNVS|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiReservedMemoryType|0
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiRuntimeServicesData|300
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiRuntimeServicesCode|150
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiBootServicesCode|1000
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiBootServicesData|800
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiLoaderCode|10
  gEmbeddedTokenSpaceGuid.PcdMemoryTypeEfiLoaderData|0

  ## Default Terminal Type
  ## 0-PCANSI, 1-VT100, 2-VT00+, 3-UTF8, 4-TTYTERM
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|4

  # Boot Manager
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|5

  # FrameBuffer Delay
  gSiliciumPkgTokenSpaceGuid.PcdMipiFrameBufferDelay|10000          # In Microseconds

  # a PCD that controls the enumeration and connection of ConIn's. When true, ConIn is only connected once a console input is requests
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerInBootOrder|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPlatformRecoverySupport|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses.Arm]
  ArmSoftFloatLib|ArmPkg/Library/ArmSoftFloatLib/ArmSoftFloatLib.inf

[LibraryClasses]
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  ImagePropertiesRecordLib|MdeModulePkg/Library/ImagePropertiesRecordLib/ImagePropertiesRecordLib.inf
  MemoryBinOverrideLib|MdeModulePkg/Library/MemoryBinOverrideLibNull/MemoryBinOverrideLibNull.inf

  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  FdtLib|EmbeddedPkg/Library/FdtLib/FdtLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf

  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf

  #
  # Allow dynamic PCDs
  #
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

  # use the accelerated BaseMemoryLibOptDxe by default, overrides for SEC/PEI below
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf

  #
  # It is not possible to prevent the ARM compiler from inserting calls to intrinsic functions.
  # This library provides the intrinsic functions such a compiler may generate calls to.
  #
  NULL|MdePkg/Library/CompilerIntrinsicsLib/ArmCompilerIntrinsicsLib.inf  # MU_CHANGE

  # Add support for GCC stack protector
  NULL|MdePkg/Library/BaseStackCheckLib/BaseStackCheckLib.inf

  # ARM Architectural Libraries
  CacheMaintenanceLib|ArmPkg/Library/ArmCacheMaintenanceLib/ArmCacheMaintenanceLib.inf
  DefaultExceptionHandlerLib|ArmPkg/Library/DefaultExceptionHandlerLib/DefaultExceptionHandlerLib.inf
  CpuExceptionHandlerLib|ArmPkg/Library/ArmExceptionLib/ArmExceptionLib.inf
  ArmDisassemblerLib|ArmPkg/Library/ArmDisassemblerLib/ArmDisassemblerLib.inf
!if $(USE_CUSTOM_GIC) == 0
  ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicLib.inf
!endif
  ArmSmcLib|ArmPkg/Library/ArmSmcLib/ArmSmcLib.inf

!if $(USE_CUSTOM_TIMER) == 0
!if $(USE_PHYSICAL_TIMER) == 1
  ArmGenericTimerCounterLib|ArmPkg/Library/ArmGenericTimerPhyCounterLib/ArmGenericTimerPhyCounterLib.inf
!else
  ArmGenericTimerCounterLib|ArmPkg/Library/ArmGenericTimerVirtCounterLib/ArmGenericTimerVirtCounterLib.inf
!endif
!endif

  MemoryInitPeiLib|SiliciumPkg/Library/MemoryInitPeiLib/MemoryInitPeiLib.inf

  # ARM PL031 RTC Driver
  TimeBaseLib|EmbeddedPkg/Library/TimeBaseLib/TimeBaseLib.inf

!if $(TARGET) == RELEASE
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!else
  SerialPortLib|SiliciumPkg/Library/FrameBufferSerialPortLib/FrameBufferSerialPortLib.inf
!endif

  PlatformThemeLib|SiliciumPkg/Library/PlatformThemeLib/PlatformThemeLib.inf
  AslUpdateLib|SiliciumPkg/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf

  #
  # Uncomment (and comment out the next line) For RealView Debugger. The Standard IO window
  # in the debugger will show load and unload commands for symbols. You can cut and paste this
  # into the command window to load symbols. We should be able to use a script to do this, but
  # the version of RVD I have does not support scripts accessing system memory.
  #
  #PeCoffExtraActionLib|ArmPkg/Library/RvdPeCoffExtraActionLib/RvdPeCoffExtraActionLib.inf
  #PeCoffExtraActionLib|ArmPkg/Library/DebugPeCoffExtraActionLib/DebugPeCoffExtraActionLib.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf

  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf

  # USB Libraries
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf

  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
  SecureBootVariableLib|SecurityPkg/Library/SecureBootVariableLib/SecureBootVariableLib.inf

  PasswordPolicyLib|OemPkg/Library/PasswordPolicyLib/PasswordPolicyLib.inf
  SecureBootKeyStoreLib|MsCorePkg/Library/SecureBootKeyStoreLibNull/SecureBootKeyStoreLibNull.inf
  PlatformPKProtectionLib|SecurityPkg/Library/PlatformPKProtectionLibVarPolicy/PlatformPKProtectionLibVarPolicy.inf
  MuSecureBootKeySelectorLib|MsCorePkg/Library/MuSecureBootKeySelectorLib/MuSecureBootKeySelectorLib.inf

  # Variable Libraries
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
  VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf

  # Whea Libraries
  MuTelemetryHelperLib|MsWheaPkg/Library/MuTelemetryHelperLib/MuTelemetryHelperLib.inf

  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf

  # Base ARM libraries
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmMmuLib|ArmPkg/Library/ArmMmuLib/ArmMmuBaseLib.inf

!if $(USE_CUSTOM_TIMER) == 0
  TimerLib|ArmPkg/Library/ArmArchTimerLib/ArmArchTimerLib.inf
!endif

  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  PlatformBootManagerLib|SiliciumPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  FrameBufferBltLib|MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.inf

  MemoryTypeInformationChangeLib|MsCorePkg/Library/MemoryTypeInformationChangeLib/MemoryTypeInformationChangeLib.inf
  DxeMemoryProtectionHobLib|MdeModulePkg/Library/MemoryProtectionHobLib/DxeMemoryProtectionHobLib.inf
  VariableFlashInfoLib|MdeModulePkg/Library/BaseVariableFlashInfoLib/BaseVariableFlashInfoLib.inf
  MemoryTypeInfoSecVarCheckLib|MdeModulePkg/Library/MemoryTypeInfoSecVarCheckLib/MemoryTypeInfoSecVarCheckLib.inf
  SecurityLockAuditLib|MdeModulePkg/Library/SecurityLockAuditDebugMessageLib/SecurityLockAuditDebugMessageLib.inf
  ResetUtilityLib|MdeModulePkg/Library/ResetUtilityLib/ResetUtilityLib.inf
  FltUsedLib|MdePkg/Library/FltUsedLib/FltUsedLib.inf

  # UI and graphics
  FrameBufferMemDrawLib|MsGraphicsPkg/Library/FrameBufferMemDrawLib/FrameBufferMemDrawLibDxe.inf

  # TPM Libraries
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf

  CompilerIntrinsicsLib|MdePkg/Library/CompilerIntrinsicsLib/ArmCompilerIntrinsicsLib.inf  # MU_CHANGE

  PcdDatabaseLoaderLib|MdeModulePkg/Library/PcdDatabaseLoaderLib/Dxe/PcdDatabaseLoaderLibDxe.inf

  ExtractGuidedSectionLib|EmbeddedPkg/Library/PrePiExtractGuidedSectionLib/PrePiExtractGuidedSectionLib.inf
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf

  MemoryMapHelperLib|SiliciumPkg/Library/MemoryMapHelperLib/MemoryMapHelperLib.inf

[LibraryClasses.common.SEC]
  HobLib|EmbeddedPkg/Library/PrePiHobLib/PrePiHobLib.inf
  MemoryAllocationLib|EmbeddedPkg/Library/PrePiMemoryAllocationLib/PrePiMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  PrePiHobListPointerLib|ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf
  ArmMmuLib|ArmPkg/Library/ArmMmuLib/ArmMmuPeiLib.inf

[LibraryClasses.common.DXE_CORE]
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf

[LibraryClasses.common.DXE_DRIVER]
!if $(USE_CUSTOM_GIC) == 0
  ArmGicArchLib|ArmPkg/Library/ArmGicArchLib/ArmGicArchLib.inf
!endif
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf

[LibraryClasses.common.UEFI_APPLICATION]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf

[LibraryClasses.common.UEFI_DRIVER]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  VariablePolicyLib|MdeModulePkg/Library/VariablePolicyLib/VariablePolicyLibRuntimeDxe.inf

[Components]
  # PrePi
  SiliciumPkg/PrePi/PrePi.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  # DXE Core
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  # PCD Database
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  # Processor Core Services
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
!if $(USE_CUSTOM_GIC) == 0
  ArmPkg/Drivers/ArmGic/ArmGicDxe.inf
!endif
!if $(USE_CUSTOM_TIMER) == 0
  ArmPkg/Drivers/TimerDxe/TimerDxe.inf
!endif

  # Pci
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf   # USB 3.*
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf   # USB 2.*

  # USB
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseAbsolutePointerDxe/UsbMouseAbsolutePointerDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  # Runtime Services
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  EmbeddedPkg/Drivers/VirtualKeyboardDxe/VirtualKeyboardDxe.inf
  EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf {
    <LibraryClasses>
!if $(USE_CUSTOM_TIMER) == 0
      RealTimeClockLib|SiliciumPkg/Library/RealTimeClockLibNull/RealTimeClockLibNull.inf
!else
      # TODO: Make a Non Arch Timer Version.
      RealTimeClockLib|EmbeddedPkg/Library/TemplateRealTimeClockLib/TemplateRealTimeClockLib.inf
!endif
  }

  # ACPI
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf

  # Disk
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf

  # BDS
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
!if ($(ARCH) == AARCH64) && ($(ENABLE_KEP) == 1)
      NULL|SiliciumPkg/Library/KernelErrataPatcherLib/KernelErrataPatcherLib.inf
!endif
  }
  MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/DriverHealthManagerDxe/DriverHealthManagerDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf

  # Graphics Driver
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 0
  SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  # Ms Button Service Driver
  SiliciumPkg/Drivers/MsButtonServiceDxe/MsButtonServiceDxe.inf

!include SiliciumPkg/Frontpage.dsc.inc
