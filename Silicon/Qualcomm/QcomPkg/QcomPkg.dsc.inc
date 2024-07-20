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

!if $(AB_SLOT_SUPPORT) == 1
  # Slot Switch App
  gSiliciumPkgTokenSpaceGuid.PcdSpecialApp|{ 0xB1, 0x0F, 0xBC, 0xD5, 0x33, 0xA8, 0x07, 0x46, 0xB7, 0xB6, 0x5E, 0xF9, 0xD1, 0x0B, 0xEE, 0xB7 }
  gSiliciumPkgTokenSpaceGuid.PcdSpecialAppName|"Slot Switch"
!endif

[LibraryClasses]
  # Custom Libraries
!if $(AB_SLOT_SUPPORT) == 1
  BootSlotLib|QcomPkg/GPLLibrary/BootSlotLib/BootSlotLib.inf
!endif
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
!if $(AB_SLOT_SUPPORT) == 1
  QcomPkg/GPLDrivers/BootSlotDxe/BootSlotDxe.inf
  QcomPkg/GPLApplications/SwitchSlotsApp/SwitchSlotsApp.inf
!endif

!include SiliciumPkg/SiliciumPkg.dsc.inc
