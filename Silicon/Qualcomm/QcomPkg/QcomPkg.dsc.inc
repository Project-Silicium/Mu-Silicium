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
  *_*_*_CC_FLAGS = -DPLATFORM_HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA=$(PLATFORM_HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA) -DPLATFORM_HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA=$(PLATFORM_HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA) -DPLATFORM_HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA=$(PLATFORM_HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA) -DPLATFORM_HAS_PSCI_MEMPROTECT_FAILING_ERRATA=$(PLATFORM_HAS_PSCI_MEMPROTECT_FAILING_ERRATA)

[PcdsFixedAtBuild]
  # RAM Size (512 MB)
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x20000000

[LibraryClasses]
  # Custom Libraries
!if $(AB_SLOT_SUPPORT) == 1
  BootSlotLib|QcomPkg/GPLLibrary/BootSlotLib/BootSlotLib.inf
!endif
  AslUpdateLib|QcomPkg/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
  RamPartitionTableLib|QcomPkg/Library/RamPartitionTableLib/RamPartitionTableLib.inf
  PowerServicesLib|QcomPkg/Library/PowerServicesLib/PowerServicesLib.inf
  BdsExtensionLib|QcomPkg/Library/BdsExtensionLib/BdsExtensionLib.inf

[Components.AArch64]
  # BDS
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
      NULL|QcomPkg/Library/KernelErrataPatcherLib/KernelErrataPatcherLib.inf
  }

[Components.Arm]
  # BDS
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

[Components]
  # Custom Drivers
  QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf
  QcomPkg/Drivers/DynamicRAMDxe/DynamicRAMDxe.inf
  QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!if $(AB_SLOT_SUPPORT) == 1
  QcomPkg/GPLDrivers/BootSlotDxe/BootSlotDxe.inf
  QcomPkg/GPLApplications/SwitchSlotsApp/SwitchSlotsApp.inf
!endif

!include SiliciumPkg/SiliciumPkg.dsc.inc
