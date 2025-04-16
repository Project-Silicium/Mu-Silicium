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
  USE_PHYSICAL_TIMER                         = 0

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
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|26000000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  # GIC
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10101000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10102000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10102000

  # ACPI
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00009830

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Exynos (TM) 9830 @ 2.73 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorCodename|"E9830"

  # CPU
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|2730

  # CPU Caches
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x1000

  # Memory Speed & Type
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|2750    # Unoffical Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

  # DDR Info
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x2AC00
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x848

[LibraryClasses]
  PlatformPrePiLib|E9830Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SiliciumPkg/Library/AcpiPlatformUpdateLibNull/AcpiPlatformUpdateLibNull.inf
  HwResetSystemLib|E9830Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  ResetSystemLib|E9830Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  SoCPlatformLib|E9830Pkg/Library/SoCPlatformLib/SoCPlatformLib.inf

[Components]
  E9830Pkg/Drivers/S2MPS_19_22Dxe/S2MPS_19_22Dxe.inf

!include SamsungPkg/SamsungPkg.dsc.inc
