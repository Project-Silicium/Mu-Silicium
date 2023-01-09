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
  PLATFORM_NAME                  = SM7150
  PLATFORM_GUID                  = b6325ac2-9f3f-4b1d-b129-ac7b35ddde60
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/SM7150Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = SM7150Pkg/SM7150.fdf
  USE_DISPLAYDXE                 = FALSE
  SWITCHSLOT_SUPPORT             = FALSE
  MASS_STORAGE_SUPPORT           = FALSE

[BuildOptions.common]
  *_*_*_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

!include SM7150Pkg/Devices/$(TARGET_DEVICE)/$(TARGET_DEVICE).dsc.inc
!include QcomPkg/Qcom.dsc.inc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|17
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|18
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17a00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17a60000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00007150
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0x17A60000
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize|44

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

[LibraryClasses.common]
  PlatformMemoryMapLib|SM7150Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|SM7150Pkg/Library/PlatformPei/PlatformPeiLib.inf
  PlatformPrePiLib|SM7150Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  MsPlatformDevicesLib|SM7150Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf
  SOCSmbiosInfoLib|SM7150Pkg/Library/SOCSmbiosInfoLib/SOCSmbiosInfoLib.inf
  RFSProtectionLib|SM7150Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
