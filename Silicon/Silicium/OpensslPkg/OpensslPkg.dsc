## @file
#  Cryptographic Library Package for UEFI Security Implementation.
#  PEIM, DXE Driver, and SMM Driver with all crypto services enabled.
#
#  Copyright (c) 2009 - 2021, Intel Corporation. All rights reserved.<BR>
#  Copyright (c) 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = OpensslPkg
  PLATFORM_GUID                  = 9c998ec7-ea1d-48be-bcb7-04e732869340
  PLATFORM_VERSION               = 0.98
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/OpensslPkg
  SUPPORTED_ARCHITECTURES        = IA32|X64|ARM|AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib|MdeModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  FltUsedLib|MsCorePkg/Library/FltUsedLib/FltUsedLib.inf
  HashApiLib|CryptoPkg/Library/BaseHashApiLib/BaseHashApiLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  MmServicesTableLib|MdePkg/Library/MmServicesTableLib/MmServicesTableLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  OpensslLib|OpensslPkg/Library/OpensslLib/OpensslLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  RngLib|MdePkg/Library/BaseRngLibNull/BaseRngLibNull.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  RealTimeClockLib|MdeModulePkg/Library/RealTimeClockLibNull/RealTimeClockLibNull.inf # MU_CHANGE

[LibraryClasses.common.SEC]
  BaseCryptLib|OpensslPkg/Library/BaseCryptLib/SecCryptLib.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf

[LibraryClasses.common.PEIM]
  BaseCryptLib|OpensslPkg/Library/BaseCryptLib/PeiCryptLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  BaseCryptLib|OpensslPkg/Library/BaseCryptLib/SmmCryptLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  TlsLib|CryptoPkg/Library/TlsLibNull/TlsLibNull.inf

[LibraryClasses.IA32.PEIM, LibraryClasses.X64.PEIM]
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
  BaseCryptLib|OpensslPkg/Library/BaseCryptLib/BaseCryptLib.inf
  DebugLib|MdePkg/Library/UefiDebugLibDebugPortProtocol/UefiDebugLibDebugPortProtocol.inf
  OpensslLib|OpensslPkg/Library/OpensslLib/OpensslLibFull.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  TlsLib|OpensslPkg/Library/TlsLib/TlsLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0f
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x06

[PcdsFeatureFlag]
  gEfiCryptoPkgTokenSpaceGuid.PcdOpensslLibAssemblySourceStyleNasm|TRUE
  gEfiCryptoPkgTokenSpaceGuid.PcdOpensslLibAssemblySourceStylePe|FALSE

###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
###################################################################################################
[Components]
  OpensslPkg/Library/BaseCryptLib/BaseCryptLib.inf
  OpensslPkg/Library/BaseCryptLib/PeiCryptLib.inf
  OpensslPkg/Library/BaseCryptLib/RuntimeCryptLib.inf
  OpensslPkg/Library/BaseCryptLib/SecCryptLib.inf
  OpensslPkg/Library/OpensslLib/OpensslLib.inf
  OpensslPkg/Library/OpensslLib/OpensslLibCrypto.inf
  OpensslPkg/Library/OpensslLib/OpensslLibFull.inf
  OpensslPkg/Library/TlsLib/TlsLib.inf

[Components.X64, Components.IA32]
  OpensslPkg/Library/BaseCryptLib/SmmCryptLib.inf

[Components.IA32, Components.X64]
  #
  # Build verification of IA32/X64 specific libraries
  #
  OpensslPkg/Library/OpensslLib/OpensslLibAccel.inf
  OpensslPkg/Library/OpensslLib/OpensslLibFullAccel.inf

[BuildOptions]
  *_*_*_CC_FLAGS = -D DISABLE_NEW_DEPRECATED_INTERFACES

[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER, BuildOptions.common.EDKII.DXE_SMM_DRIVER, BuildOptions.common.EDKII.SMM_CORE, BuildOptions.common.EDKII.DXE_DRIVER]
  MSFT:*_*_IA32_DLINK_FLAGS = /ALIGN:4096 # enable 4k alignment for MAT and other protections.
  MSFT:*_*_X64_DLINK_FLAGS = /ALIGN:4096 # enable 4k alignment for MAT and other protections.
