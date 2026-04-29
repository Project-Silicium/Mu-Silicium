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
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorSocket|"MediaTek"
  gSiliciumPkgTokenSpaceGuid.PcdSmBiosProcessorManufacturer|"MediaTek Inc."

[LibraryClasses]
  #
  # ARM Libraries
  #
  ArmSmcLib|MdePkg/Library/ArmSmcLib/ArmSmcLib.inf

[Components]
  #
  # SmBios
  #
  MediaTekPkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf

  #
  # Custom Drivers
  #
  MediaTekPkg/Drivers/GpioDxe/GpioDxe.inf
  MediaTekPkg/Drivers/PmicWrapperDxe/PmicWrapperDxe.inf
  MediaTekPkg/Drivers/MT6358PmicDxe/MT6358PmicDxe.inf
  MediaTekPkg/Drivers/MsdcDxe/MsdcDxe.inf
