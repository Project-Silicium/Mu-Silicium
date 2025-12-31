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

[LibraryClasses]
  #
  # ARM Libraries
  #
  ArmSmcLib|SamsungPkg/Library/ArmSmcLib/ArmSmcLib.inf

  #
  # Silicium Libraries
  #
  PlatformHobLib|SiliciumPkg/Library/PlatformHobLibNull/PlatformHobLibNull.inf
  MemoryProtectionLib|SamsungPkg/Library/MemoryProtectionLib/MemoryProtectionLib.inf
  ErrataPatchesLib|SamsungPkg/Library/ErrataPatchesLib/ErrataPatchesLib.inf

[Components]
  #
  # SmBios
  #
  SamsungPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Custom Drivers
  #
  SamsungPkg/Drivers/ChipDataDxe/ChipDataDxe.inf
  SamsungPkg/Drivers/DdrInfoDxe/DdrInfoDxe.inf
  SamsungPkg/Drivers/GpioDxe/GpioDxe.inf
  SamsungPkg/Drivers/SpeedyDxe/SpeedyDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
