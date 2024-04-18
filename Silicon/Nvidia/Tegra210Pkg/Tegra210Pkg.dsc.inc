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
  USE_PHYSICAL_TIMER = 1

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Timer
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|31
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|28
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|27

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x50041000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x50042000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x50042000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00000210

  # SmBios
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"ODNX02-A2"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"TM670D-A1"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"TM670M-A2"
!elseif $(SOC_TYPE) == 3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"TM671D-A2"
!elseif $(SOC_TYPE) == 4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 1.42 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"TM660M-A2"
!elseif $(SOC_TYPE) == 5
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.10 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"ODNX10-A1"
!elseif $(SOC_TYPE) == 6
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.10 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"TM675M-A1"
!endif

  # CPU
!if ($(SOC_TYPE) == 0) || ($(SOC_TYPE) == 1) || ($(SOC_TYPE) == 2) || ($(SOC_TYPE) == 3)
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2
!else
  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1
!endif

[LibraryClasses]
  PlatformPeiLib|Tegra210Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|Tegra210Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|Tegra210Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components]
  Tegra210Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include NVPkg/NVPkg.dsc.inc
