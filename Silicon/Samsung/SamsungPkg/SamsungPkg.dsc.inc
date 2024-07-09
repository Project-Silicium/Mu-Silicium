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

[Components]
  # PrePI
  SamsungPkg/PrePi/PrePi.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  # Custom Exynos Drivers
  SamsungPkg/Drivers/ExynosGpioDxe/ExynosGpioDxe.inf
  SamsungPkg/Drivers/ExynosSpeedyDxe/ExynosSpeedyDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
