## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

# SoC Defines
[Defines]
  USE_PHYSICAL_TIMER          = 1
  PRECOMPILED_CRYPTO_BINARIES = 1

[BuildOptions.common]
  *_*_AARCH64_CC_FLAGS = -march=armv8-a+crypto+rcpc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF8C000
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|1
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|2
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|3
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|0
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xf200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xf300000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0xf200000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00006225
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0xf200000

  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 680 @ 2.40 GHz"
  gQcomPkgTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM6225"

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  # UART
  gQcomPkgTokenSpaceGuid.PcdUartSerialBase|0x4A8C000

[LibraryClasses.common]
  PlatformPeiLib|SM6225Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  PlatformPrePiLib|SM6225Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SM6225Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  HwResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf
  ResetSystemLib|ArmPkg/Library/ArmSmcPsciResetSystemLib/ArmSmcPsciResetSystemLib.inf

[Components.common]
  SM6225Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

!include QcomPkg/QcomPkg.dsc.inc
