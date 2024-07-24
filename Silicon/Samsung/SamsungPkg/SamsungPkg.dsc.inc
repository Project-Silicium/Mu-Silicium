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
!if $(DEVICE_PRESERVES_FDT) == 1
  SamsungPkg/PrePi/PrePiUniversalDecon.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }
!else
SamsungPkg/PrePi/PrePi.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }
!endif

  # Custom Exynos Drivers
  SamsungPkg/Drivers/ExynosGpioDxe/ExynosGpioDxe.inf
  SamsungPkg/Drivers/ExynosSpeedyDxe/ExynosSpeedyDxe.inf
!if $(DEVICE_PRESERVES_FDT) == 1 && $(DEVICE_SUPPORTS_EXYNOS_DYNAMIC_RAM_ALLOCATION) == 1
  SamsungPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
!endif

!include SiliciumPkg/SiliciumPkg.dsc.inc
