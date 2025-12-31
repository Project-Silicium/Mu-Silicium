##
#
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[PcdsFixedAtBuild]
  # RAM Size (512 MB)
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x20000000

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorSocket|"NVIDIA"
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorManufacturer|"NVIDIA Corporation"

[LibraryClasses]
  #
  # ARM Libraries
  #
  ArmSmcLib|MdePkg/Library/ArmSmcLib/ArmSmcLib.inf
!if $(USE_CUSTOM_TIMER) == 1
  TimerLib|NvidiaPkg/GPLLibrary/TegraTimerLib/TegraTimerLib.inf
!endif

  #
  # Silicium Libraries
  #
  PlatformHobLib|SiliciumPkg/Library/PlatformHobLibNull/PlatformHobLibNull.inf
  MemoryProtectionLib|NvidiaPkg/Library/MemoryProtectionLib/MemoryProtectionLib.inf
  ErrataPatchesLib|NvidiaPkg/Library/ErrataPatchesLib/ErrataPatchesLib.inf

[Components]
  #
  # SmBios
  #
  NvidiaPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Custom Drivers
  #
  NvidiaPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  NvidiaPkg/GPLDrivers/TegraGpioDxe/TegraGpioDxe.inf
!if $(USE_CUSTOM_TIMER) == 1
  NvidiaPkg/GPLDrivers/TegraTimerDxe/TegraTimerDxe.inf
!endif
  NvidiaPkg/GPLDrivers/TegraClockDxe/TegraClockDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
