##
#  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.
#  Copyright (c) 2018 - 2019, Bingxing Wang. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

!include SiliciumPkg/SiliciumPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorSocket|"Samsung"
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorManufacturer|"Samsung Electronics Co., Ltd."

[LibraryClasses]
  #
  # Memory Libraries
  #
  RamManagerLib|SamsungPkg/Library/RamManagerLib/RamManagerLib.inf

  #
  # IO Libraries
  #
  GpioLib|SamsungNullPkg/Library/GpioNullLib/GpioNullLib.inf
  SpeedyLib|SamsungNullPkg/Library/SpeedyNullLib/SpeedyNullLib.inf
  HsI2cLib|SamsungNullPkg/Library/HsI2cNullLib/HsI2cNullLib.inf

  #
  # Storage Libraries
  #
  PlatformUfsLib|SamsungNullPkg/Library/PlatformUfsLibNull/PlatformUfsLibNull.inf
  UfsCalAdapterLib|SamsungPkg/Library/UfsCalAdapterLib/UfsCalAdapterLib.inf

  #
  # Samsung Libraries
  #
  UsiLib|SamsungNullPkg/Library/UsiNullLib/UsiNullLib.inf

[Components]
  #
  # Storage Drivers
  #
  SamsungPkg/Drivers/UfsDxe/UfsDxe.inf

  #
  # IO Drivers
  #
  SamsungPkg/Drivers/GpioDxe/GpioDxe.inf
  SamsungPkg/Drivers/SpeedyDxe/SpeedyDxe.inf
  SamsungPkg/Drivers/HsI2cDxe/HsI2cDxe.inf

  #
  # Power Management Drivers
  #
  SamsungPkg/Drivers/PmicDxe/PmicDxe.inf

  #
  # SMBIOS Drivers
  #
  SamsungPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Samsung Drivers
  #
  SamsungPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  SamsungPkg/Drivers/UsiDxe/UsiDxe.inf
