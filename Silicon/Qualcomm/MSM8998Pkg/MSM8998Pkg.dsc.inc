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
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17B00000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17A00000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00008998

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 835 @ 2.45 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"MSM8998"

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2
  gQcomPkgTokenSpaceGuid.PcdHasLevel3Cache|FALSE

  # Power Services
  gQcomPkgTokenSpaceGuid.PcdIsPowerOkImplemented|FALSE

[LibraryClasses]
  PlatformPeiLib|MSM8998Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|MSM8998Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|MSM8998Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|MSM8998Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components]
  MSM8998Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/QcomPkg.dsc.inc
