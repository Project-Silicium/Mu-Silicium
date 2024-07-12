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

[BuildOptions]
  *_*_*_CC_FLAGS = -DHAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA=$(HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA) -DHAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA=$(HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA) -DHAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA=$(HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA) -DHAS_PSCI_MEMPROTECT_FAILING_ERRATA=$(HAS_PSCI_MEMPROTECT_FAILING_ERRATA)

[PcdsFixedAtBuild]
  # RAM Size (512 MB)
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x20000000

!if $(AB_SLOT_SUPPORT) == 1
  # Slot Switch App GUID
  gSiliciumPkgTokenSpaceGuid.PcdSpecificApp|{ 0xB1, 0x0F, 0xBC, 0xD5, 0x33, 0xA8, 0x07, 0x46, 0xB7, 0xB6, 0x5E, 0xF9, 0xD1, 0x0B, 0xEE, 0xB7 }
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
