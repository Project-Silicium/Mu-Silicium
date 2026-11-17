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
  #
  # Timer
  #
  USE_PHYSICAL_TIMER                         = 1

!include MediaTekPkg/MediaTekPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc -Wno-unused-but-set-variable

[PcdsFixedAtBuild]
  #
  # ACPI
  #
!if $(SOC_TYPE) == 0
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x202035333736544D
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x6735
!elseif $(SOC_TYPE) == 1
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x202035333738544D
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x8735
!endif
  #
  # CPU
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|4
  gArmPlatformTokenSpaceGuid.PcdClusterCount|1

  #
  # GIC
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10221000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x10222000

  #
  # Timer
  #
  gSiliciumPkgTokenSpaceGuid.PcdTimerFreqOverwrite|13000000

  #
  # SMBIOS
  #
!if $(SOC_TYPE) == 0
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"MediaTek (TM) MT6735 @ 1.30 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"MT6735"
!elseif $(SOC_TYPE) == 1
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"MediaTek (TM) MT8735 @ 1.30 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"MT8735"
!endif

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|MT6735Pkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # UEFI Libraries
  #
  HwResetSystemLib|MT6735Pkg/Library/ResetSystemLib/ResetSystemLib.inf
  ResetSystemLib|MT6735Pkg/Library/ResetSystemLib/ResetSystemLib.inf

  #
  # Custom Libraries
  #
  GpioImplLib|MT6735Pkg/Library/GpioImplLib/GpioImplLib.inf
  PmicWrapperImplLib|MT6735Pkg/Library/PmicWrapperImplLib/PmicWrapperImplLib.inf
