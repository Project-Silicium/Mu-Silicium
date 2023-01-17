## @file
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#  Copyright (c) 2022, Xilin Wu. All rights reserved.
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
  PLATFORM_NAME                  = MSM8998
  PLATFORM_GUID                  = b6325ac2-9f3f-4b1d-b129-ac7b35ddde60
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/MSM8998Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = MSM8998Pkg/MSM8998.fdf
  USE_DISPLAYDXE                 = FALSE
  SWITCHSLOT_SUPPORT             = FALSE
  MASS_STORAGE_SUPPORT           = FALSE

[BuildOptions.common]
  *_*_*_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

!include MSM8998Pkg/Devices/$(TARGET_DEVICE)/$(TARGET_DEVICE).dsc.inc
!include QcomPkg/Qcom.dsc.inc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17B00000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00008998
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0x17B00000
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize|44

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

[LibraryClasses.common]
  PlatformPeiLib|MSM8998Pkg/Library/PlatformPei/PlatformPeiLib.inf
  PlatformPrePiLib|MSM8998Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  MsPlatformDevicesLib|MSM8998Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf
  SOCSmbiosInfoLib|MSM8998Pkg/Library/SOCSmbiosInfoLib/SOCSmbiosInfoLib.inf
  RFSProtectionLib|MSM8998Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf

[Components.common]
  MSM8998Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
