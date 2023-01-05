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
  PLATFORM_NAME                  = SM8350
  PLATFORM_GUID                  = b6325ac2-9f3f-4b1d-b129-ac7b35ddde60
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/SM8350Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = SM8350Pkg/SM8350.fdf
  USE_DISPLAYDXE                 = FALSE
  SWITCHSLOT_SUPPORT             = FALSE
  MASS_STORAGE_SUPPORT           = FALSE

[BuildOptions.common]
  *_*_*_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

!include SM8350Pkg/Devices/$(TARGET_DEVICE)/$(TARGET_DEVICE).dsc.inc
!include QcomPkg/Qcom.dsc.inc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17A60000

  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemRevision|0x00008350
  gEmbeddedTokenSpaceGuid.PcdInterruptBaseAddress|0x17A60000
  gEmbeddedTokenSpaceGuid.PcdPrePiCpuIoSize|44

  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

[LibraryClasses.common]
  PlatformMemoryMapLib|SM8350Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|SM8350Pkg/Library/PlatformPei/PlatformPeiLib.inf
  PlatformPrePiLib|SM8350Pkg/Library/PlatformPrePiLib/PlatformPrePiLib.inf
  MsPlatformDevicesLib|SM8350Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf
  SOCSmbiosInfoLib|SM8350Pkg/Library/SOCSmbiosInfoLib/SOCSmbiosInfoLib.inf
  RFSProtectionLib|SM8350Pkg/Library/RFSProtectionLib/RFSProtectionLib.inf
