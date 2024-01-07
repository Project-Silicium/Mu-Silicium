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
  PLATFORM_NAME                  = ATU-L21
  PLATFORM_GUID                  = 98ebebe8-c68a-46a6-9c8b-4cee7fc0ff0b
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/ATU-L21Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = ATU-L21Pkg/ATU-L21.fdf
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 0

[LibraryClasses.common]
  DeviceMemoryMapLib|ATU-L21Pkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  KeypadDeviceImplLib|ATU-L21Pkg/Library/KeypadDeviceImplLib/KeypadDeviceImplLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000             # Starting address
!if $(RAM_SIZE) == 3
  gArmTokenSpaceGuid.PcdSystemMemorySize|0xC0000000             # 3GB Size
!elseif $(RAM_SIZE) == 2
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x80000000             # 2GB Size
!else
!error "Invaild RAM Size! Use 3 or 2."
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Robotix22" # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x82600000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x82601000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000          # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Huawei Technologies Co., Ltd"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Y6 2018"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"ATU-L21"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Y6_2018_ATU-L21"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Y6 2018"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1440
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|24

[Components.common]
  # Keypad
  QcomPkg/Drivers/KeypadDxe/KeypadDxe.inf
  QcomPkg/Drivers/GenericKeypadDeviceDxe/GenericKeypadDeviceDxe.inf

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1440
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|75
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|75

!include MSM8917Pkg/MSM8917Pkg.dsc.inc
