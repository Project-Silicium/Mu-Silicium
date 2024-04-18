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
  USE_CUSTOM_GIC     = 0
  USE_CUSTOM_TIMER   = 0
  USE_PHYSICAL_TIMER = 0

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|26000000
!if $(BROKEN_CNTFRQ_EL0) == 1
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|13
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|14
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|11
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|10
!else
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|31
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|28
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|27
!endif

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x12301000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x12302000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x12302000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00007885

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 7885 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"E7885"

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

[LibraryClasses]
  PlatformPeiLib|E7885Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|E7885Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|E7885Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components]
  E7885Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include SamPkg/SamPkg.dsc.inc
