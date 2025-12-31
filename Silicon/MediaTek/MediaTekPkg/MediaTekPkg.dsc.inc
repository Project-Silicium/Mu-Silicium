##
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[PcdsFixedAtBuild]
  # RAM Size (512 MB)
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x20000000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorSocket|"MediaTek"
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorManufacturer|"MediaTek Inc."

[LibraryClasses]
  #
  # ARM Libraries
  #
  ArmSmcLib|MdePkg/Library/ArmSmcLib/ArmSmcLib.inf

  #
  # Silicium Libraries
  #
  PlatformHobLib|SiliciumPkg/Library/PlatformHobLibNull/PlatformHobLibNull.inf
  MemoryProtectionLib|MediaTekPkg/Library/MemoryProtectionLib/MemoryProtectionLib.inf
  ErrataPatchesLib|MediaTekPkg/Library/ErrataPatchesLib/ErrataPatchesLib.inf

[Components]
  #
  # SmBios
  #
  MediaTekPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Custom Drivers
  #
  MediaTekPkg/Drivers/GpioDxe/GpioDxe.inf
  MediaTekPkg/Drivers/PmicWrapperDxe/PmicWrapperDxe.inf
  MediaTekPkg/Drivers/MT6358PmicDxe/MT6358PmicDxe.inf
  MediaTekPkg/Drivers/MsdcDxe/MsdcDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
