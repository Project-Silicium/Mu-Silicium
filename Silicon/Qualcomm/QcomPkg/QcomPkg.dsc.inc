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
  # Custom Libraries
  RamPartitionTableLib|QcomPkg/Library/RamPartitionTableLib/RamPartitionTableLib.inf
  ConfigurationMapHelperLib|QcomPkg/Library/ConfigurationMapHelperLib/ConfigurationMapHelperLib.inf

[Components]
  # PrePI
  QcomPkg/PrePi/PrePi.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  # Custom Snapdragon Drivers
  QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf
  QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
  QcomPkg/Drivers/PeripheralInitDxe/PeripheralInitDxe.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
