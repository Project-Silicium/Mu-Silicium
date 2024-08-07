##
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x20000000             # 512 MB Size

[Components]
  # PrePI
  SamsungPkg/PrePi/PrePi.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  # Custom Exynos Drivers
  SamsungPkg/Drivers/ExynosGpioDxe/ExynosGpioDxe.inf
  SamsungPkg/Drivers/ExynosSpeedyDxe/ExynosSpeedyDxe.inf
  SamsungPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
