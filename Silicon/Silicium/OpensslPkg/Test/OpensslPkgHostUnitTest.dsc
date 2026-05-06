## @file
# OpensslPkg DSC file used to build host-based unit tests.
#
# Copyright (c) Microsoft Corporation.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]
  PLATFORM_NAME           = OpensslPkgHostTest
  PLATFORM_GUID           = A7F3A753-9BE8-4BA5-8B1C-1C3E4E5AC1A1
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x00010005
  OUTPUT_DIRECTORY        = Build/OpensslPkg/HostTest
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS           = NOOPT
  SKUID_IDENTIFIER        = DEFAULT

!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  BaseCryptLib|OpensslPkg/Library/BaseCryptLib/UnitTestHostBaseCryptLib.inf
  OpensslLib|OpensslPkg/Library/OpensslLib/OpensslLibFull.inf
  MmServicesTableLib|MdePkg/Library/MmServicesTableLib/MmServicesTableLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf

[LibraryClasses.X64, LibraryClasses.IA32]
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf

[Components]
  #
  # Build HOST_APPLICATION that tests BaseCryptLib (OpenSSL implementation)
  #
  CryptoPkg/Test/UnitTest/Library/BaseCryptLib/TestBaseCryptLibHost.inf

[BuildOptions]
  *_*_*_CC_FLAGS = -D DISABLE_NEW_DEPRECATED_INTERFACES
