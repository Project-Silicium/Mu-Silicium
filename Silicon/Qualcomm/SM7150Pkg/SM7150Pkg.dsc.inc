##
#
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]
  USE_PHYSICAL_TIMER = 1

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17A60000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17A60000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00007150

  # SmBios
!if $(SOC_TYPE) == 0
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 730 @ 2.21 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM7150-AA"
!elseif $(SOC_TYPE) == 1
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 730G @ 2.21 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM7150-AB"
!elseif $(SOC_TYPE) == 2
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 732G @ 2.30 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM7150-AC"
!endif

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2
  gQcomPkgTokenSpaceGuid.PcdHasLevel3Cache|TRUE

  # Power Services
  gQcomPkgTokenSpaceGuid.PcdIsPowerOkImplemented|TRUE

  # CPU Max Freq
  gQcomPkgTokenSpaceGuid.PcdMaxFreqSupported|FALSE

[LibraryClasses]
  PlatformPeiLib|SM7150Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|SM7150Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  RFSProtectionLib|SM7150Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
  AcpiPlatformUpdateLib|SM7150Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|SM7150Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components]
  SM7150Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/QcomPkg.dsc.inc
