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
  # ARM Libraries
  #
  ArmSmcLib|SamsungPkg/Library/ArmSmcLib/ArmSmcLib.inf

[Components]
  #
  # SMBIOS
  #
  SamsungPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Custom Drivers
  #
  SamsungPkg/Drivers/ChipDataDxe/ChipDataDxe.inf
  SamsungPkg/Drivers/DdrInfoDxe/DdrInfoDxe.inf
  SamsungPkg/Drivers/GpioDxe/GpioDxe.inf
  SamsungPkg/Drivers/SpeedyDxe/SpeedyDxe.inf
