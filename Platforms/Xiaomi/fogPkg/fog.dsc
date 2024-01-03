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
  PLATFORM_NAME                  = fog
  PLATFORM_GUID                  = c62242f8-e275-4150-b6f3-1cc4fac6425c
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/fogPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = fogPkg/fog.fdf
  DISPLAY_USES_RGBA              = 0
  USE_DISPLAYDXE                 = 0
  AB_SLOT_SUPPORT                = 1

[BuildOptions.common]
  *_*_*_CC_FLAGS = -DDEVICE_RAM=$(RAM_SIZE)

[LibraryClasses.common]
  DeviceMemoryMapLib|fogPkg/Library/DeviceMemoryMapLib/DeviceMemoryMapLib.inf
  DeviceConfigurationMapLib|fogPkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf

[PcdsFixedAtBuild.common]
  # Device Specific
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000            # Starting Address
!if $(RAM_SIZE) == 4
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000           # 4GB Size
!elseif $(RAM_SIZE) == 3
  gArmTokenSpaceGuid.PcdSystemMemorySize|0xC0000000            # 3GB Size
!else
!error "Invaild Mem Size! Use 4 or 3."
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Statzar"  # Device Maintainer

  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x5FF8C000

  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x5FF90000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000         # 256K stack

  # SmBios
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemVendor|"Xiaomi Inc"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi 10C"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"fog"
  gQcomPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Redmi_10C_fog"
  gQcomPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Redmi 10C"

  # Simple FrameBuffer
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|1650
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

[PcdsDynamicDefault.common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|1650
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|720
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|1650
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|86
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|90
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|86

!include SM6225Pkg/SM6225Pkg.dsc.inc