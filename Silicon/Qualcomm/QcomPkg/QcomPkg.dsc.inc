##
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

!include SiliciumPkg/SiliciumPkg.dsc.inc

[BuildOptions]
  *_CLANGPDB_AARCH64_CC_FLAGS = -D HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA=$(HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA) -D HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA=$(HAS_AMCNTENSET0_EL0_UNIMPLEMENTED_ERRATA) -D HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA=$(HAS_GIC_V3_WITHOUT_IRM_FLAG_SUPPORT_ERRATA) -D HAS_PSCI_MEMPROTECT_FAILING_ERRATA=$(HAS_PSCI_MEMPROTECT_FAILING_ERRATA)

[PcdsFixedAtBuild]
  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorSocket|"Qualcomm"
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorManufacturer|"Qualcomm Technologies, Inc."

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformPeiLib|QcomPkg/Library/PlatformPeiLib/PlatformPeiLib.inf

  #
  # Memory Libraries
  #
  RamManagerLib|QcomPkg/Library/RamManagerLib/RamManagerLib.inf

  #
  # BDS Libraries
  #
  DeviceBootManagerLib|QcomPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  PlatformBootOptionsLib|QcomPkg/Library/PlatformBootOptionsLib/PlatformBootOptionsLib.inf
  ErrataPatchesLib|QcomPkg/Library/ErrataPatchesLib/ErrataPatchesLib.inf

  #
  # Custom Libraries
  #
  ConfigurationMapHelperLib|QcomPkg/Library/ConfigurationMapHelperLib/ConfigurationMapHelperLib.inf
  ArmSmmuDetachLib|QcomPkg/Library/ArmSmmuDetachLib/ArmSmmuDetachLib.inf

[Components]
  #
  # SMBIOS
  #
  QcomPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Custom Drivers
  #
  QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf

  #
  # Custom Apps
  #
  QcomPkg/Applications/MassStorage/MassStorage.inf
