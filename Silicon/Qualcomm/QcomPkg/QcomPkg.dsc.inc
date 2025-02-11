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

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorSocket|"Qualcomm"
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorManufacturer|"Qualcomm Technologies, Inc."

  # Mass Storage App
  gSiliciumPkgTokenSpaceGuid.PcdSpecialApp|{ 0xD8, 0xEC, 0x14, 0xFB, 0x32, 0x94, 0xB8, 0x48, 0xA6, 0x92, 0xAE, 0x4B, 0xC1, 0x9D, 0x56, 0x8B }
  gSiliciumPkgTokenSpaceGuid.PcdSpecialAppName|"Mass Storage"

[LibraryClasses]
  # Custom Libraries
  RamPartitionTableLib|QcomPkg/Library/RamPartitionTableLib/RamPartitionTableLib.inf
  ConfigurationMapHelperLib|QcomPkg/Library/ConfigurationMapHelperLib/ConfigurationMapHelperLib.inf

  # Pei Library
  PlatformPeiLib|QcomPkg/Library/PlatformPeiLib/PlatformPeiLib.inf

  # Memory Protection Library
  MemoryProtectionLib|QcomPkg/Library/MemoryProtectionLib/MemoryProtectionLib.inf

[Components]
  # SmBios
  QcomPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  # Custom Snapdragon Drivers
  QcomPkg/Drivers/ClockSpeedUpDxe/ClockSpeedUpDxe.inf
  QcomPkg/Drivers/DynamicRamDxe/DynamicRamDxe.inf
  QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
  QcomPkg/Drivers/PeripheralInitDxe/PeripheralInitDxe.inf

  # Custom Snapdragon Apps
  QcomPkg/Applications/MassStorage/MassStorage.inf

!include SiliciumPkg/SiliciumPkg.dsc.inc
