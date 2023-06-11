## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = SM6125
  PLATFORM_GUID                  = 13916838-2ca7-4152-bcfa-74fddf2b6823
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/SM6125Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = SM6125Pkg/SM6125.fdf
  USE_PHYSICAL_TIMER             = 1
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0

!include SM6125Pkg/Devices/$(TARGET_DEVICE)/$(TARGET_DEVICE).dsc.inc
!include QcomPkg/Qcom.dsc.inc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdGicDistributorBase|0xf200000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0xf300000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00006125
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0xf300000

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  gQcomTokenSpaceGuid.PcdSmbiosProcessorModel|"Snapdragon (TM) 665 @ 2.00 GHz"
  gQcomTokenSpaceGuid.PcdSmbiosProcessorRetailModel|"SM6125"

[LibraryClasses.common]
  PowerServicesLib|SM6125Pkg/Library/PowerServicesLib/PowerServicesLib.inf
  PlatformPeiLib|SM6125Pkg/Library/PlatformPei/PlatformPeiLib.inf
  PlatformPrePiLib|SM6125Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  AcpiPlatformUpdateLib|SM6125Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf
  MsPlatformDevicesLib|SM6125Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf
  ConsoleMsgLib|SM6125Pkg/Library/ConsoleMsgLib/ConsoleMsgLib.inf

[Components.common]
  SM6125Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
