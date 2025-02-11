##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  # GIC
  USE_CUSTOM_GIC                             = 0

  # Timer
  USE_CUSTOM_TIMER                           = 0
  USE_PHYSICAL_TIMER                         = 1

  # KEP
  ENABLE_KEP                                 = 0
  HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA         = 0
  HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA   = 0
  HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA = 0
  HAS_PSCI_MEMPROTECT_FAILING_ERRATA         = 0

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
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x50041000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x50042000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x50042000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00000210

  # SmBios
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"ODNX02-A2"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"TM670D-A1"
!elseif $(SOC_TYPE) == 2
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"TM670M-A2"
!elseif $(SOC_TYPE) == 3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.20 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"TM671D-A2"
!elseif $(SOC_TYPE) == 4
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 1.42 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"TM660M-A2"
!elseif $(SOC_TYPE) == 5
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.10 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"ODNX10-A1"
!elseif $(SOC_TYPE) == 6
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Tegra (TM) X1 @ 2.10 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"TM675M-A1"
!endif

  # CPU
!if ($(SOC_TYPE) == 0) || ($(SOC_TYPE) == 1) || ($(SOC_TYPE) == 2) || ($(SOC_TYPE) == 3)
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2
!else
  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1
!endif

  # Clock IDs
  gNvidiaPkgTokenSpaceGuid.PcdCGeneralID|0
  gNvidiaPkgTokenSpaceGuid.PcdMemoryID|1
  gNvidiaPkgTokenSpaceGuid.PcdPeriphID|2
  gNvidiaPkgTokenSpaceGuid.PcdAudioID|3
  gNvidiaPkgTokenSpaceGuid.PcdUsbID|4
  gNvidiaPkgTokenSpaceGuid.PcdDisplayID|5
  gNvidiaPkgTokenSpaceGuid.PcdDisplay2ID|0xFF
  gNvidiaPkgTokenSpaceGuid.PcdXCpuID|6
  gNvidiaPkgTokenSpaceGuid.PcdEpciID|7
  gNvidiaPkgTokenSpaceGuid.PcdSfrom32KhzID|8
  gNvidiaPkgTokenSpaceGuid.PcdDpID|9
  gNvidiaPkgTokenSpaceGuid.Pcd32KhzID|10
  gNvidiaPkgTokenSpaceGuid.PcdOscID|11
  gNvidiaPkgTokenSpaceGuid.PcdClkmID|12
  gNvidiaPkgTokenSpaceGuid.PcdIDCount|13

[LibraryClasses]
  PlatformPrePiLib|Tegra210Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  SoCPlatformLib|Tegra210Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf
  DynamicRamLib|Tegra210Pkg/GPLLibrary/DynamicRamLib/DynamicRamLib.inf
  TegraClockLib|Tegra210Pkg/GPLLibrary/TegraClockLib/TegraClockLib.inf

[Components]
  Tegra210Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include NvidiaPkg/NvidiaPkg.dsc.inc
