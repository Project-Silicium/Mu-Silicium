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
  # ACPI
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|"MTK   "

[LibraryClasses]
  #
  # ARM Libraries
  #
  ArmSmcLib|MdePkg/Library/ArmSmcLib/ArmSmcLib.inf

  #
  # Memory Libraries
  #
  RamManagerLib|MediaTekPkg/Library/RamManagerLib/RamManagerLib.inf

  #
  # Clock Libraries
  #
  ClockImplLib|MediaTekNullPkg/Library/ClockImplNullLib/ClockImplNullLib.inf

  #
  # Storage Libraries
  #
  MsdcImplLib|MediaTekNullPkg/Library/MsdcImplNullLib/MsdcImplNullLib.inf

[Components]
  #
  # Custom Drivers
  #
  MediaTekPkg/Drivers/GpioDxe/GpioDxe.inf
  MediaTekPkg/Drivers/ClockDxe/ClockDxe.inf
  MediaTekPkg/Drivers/PmicWrapperDxe/PmicWrapperDxe.inf
  MediaTekPkg/Drivers/MT6358PmicDxe/MT6358PmicDxe.inf
  MediaTekPkg/Drivers/MT6328PmicDxe/MT6328PmicDxe.inf
  MediaTekPkg/Drivers/MsdcDxe/MsdcDxe.inf
