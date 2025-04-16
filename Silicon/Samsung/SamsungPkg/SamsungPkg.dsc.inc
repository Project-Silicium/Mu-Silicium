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
  # Pei Library
  PlatformPeiLib|SamsungPkg/Library/PlatformPeiLib/PlatformPeiLib.inf

  # Memory Pritection Library
  MemoryProtectionLib|SamsungPkg/Library/MemoryProtectionLib/MemoryProtectionLib.inf

[Components]
  # SmBios
  SamsungPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  # Custom Exynos Drivers
  SamsungPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  SamsungPkg/Drivers/ChipDataDxe/ChipDataDxe.inf
  SamsungPkg/Drivers/DdrInfoDxe/DdrInfoDxe.inf
  SamsungPkg/Drivers/GpioDxe/GpioDxe.inf
  SamsungPkg/Drivers/SpeedyDxe/SpeedyDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
