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

[LibraryClasses]
  # Custom Libraries
!if $(USE_CUSTOM_TIMER) == 1
  TimerLib|NvidiaPkg/GPLLibrary/TegraTimerLib/TegraTimerLib.inf
!endif

[Components]
  # PrePI
  NvidiaPkg/PrePi/PrePi.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  # Custom Tegra Drivers
  NvidiaPkg/GPLDrivers/TegraGpioDxe/TegraGpioDxe.inf
!if $(USE_CUSTOM_TIMER) == 1
  NvidiaPkg/GPLDrivers/TegraTimerDxe/TegraTimerDxe.inf
!endif

!include SiliciumPkg/SiliciumPkg.dsc.inc
