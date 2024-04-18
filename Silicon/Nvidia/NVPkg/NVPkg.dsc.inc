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
  PowerServicesLib|PcBdsPkg/Library/PowerServicesLibNull/PowerServicesLibNull.inf
  BdsExtensionLib|NVPkg/Library/BdsExtensionLib/BdsExtensionLib.inf
!if $(USE_CUSTOM_TIMER) == 1
  TimerLib|NVPkg/GPLLibrary/TegraTimerLib/TegraTimerLib.inf
!endif

[Components]
  # BDS
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  # Custom Tegra Drivers
  NVPkg/GPLDrivers/TegraGpioDxe/TegraGpioDxe.inf
!if $(USE_CUSTOM_TIMER) == 1
  NVPkg/GPLDrivers/TegraTimerDxe/TegraTimerDxe.inf
!endif

!include SiliciumPkg/SiliciumPkg.dsc.inc
