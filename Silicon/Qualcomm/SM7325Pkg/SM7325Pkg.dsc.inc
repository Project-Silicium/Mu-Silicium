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
  USE_PHYSICAL_TIMER = 0

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17A60000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17A60000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00007325

  # SmBios
!if $(SOC_TYPE) == 0
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 778G @ 2.40 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM7325"
!elseif $(SOC_TYPE) == 1
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 778G+ @ 2.50 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM7325-AE"
!elseif $(SOC_TYPE) == 2
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 782G @ 2.70 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM7325-AF"
!endif

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3
  gQcomPkgTokenSpaceGuid.PcdHasLevel3Cache|TRUE

  # Power Services
  gQcomPkgTokenSpaceGuid.PcdIsPowerOkImplemented|TRUE

  # CPU Max Freq
  gQcomPkgTokenSpaceGuid.PcdMaxFreqSupported|TRUE

[LibraryClasses]
  PlatformPeiLib|SM7325Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|SM7325Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  RFSProtectionLib|SM7325Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
  AcpiPlatformUpdateLib|SM7325Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|SM7325Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components]
  SM7325Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/QcomPkg.dsc.inc
