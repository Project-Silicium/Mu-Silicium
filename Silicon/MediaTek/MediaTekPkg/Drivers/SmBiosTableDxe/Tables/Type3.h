/**
  Copyright (c), 2017, Andrey Warkentin <andrey.warkentin@gmail.com>
  Copyright (c), 2018, Bingxing Wang <uefi-oss-projects@imbushuo.net>
  Copyright (c), Microsoft Corporation. All rights reserved.
  Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013 Linaro.org

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _TYPE_3_H_
#define _TYPE_3_H_

SMBIOS_TABLE_TYPE3 mEnclosureInfoType3 = {
  {EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE, sizeof(SMBIOS_TABLE_TYPE3), 0},
  1,                         // Manufacturer String
  MiscChassisTypeUnknown,    // Type
  2,                         // Version String
  3,                         // SerialNumber String
  4,                         // AssetTag String
  ChassisStateSafe,          // BootupState
  ChassisStateSafe,          // PowerSupplyState
  ChassisStateSafe,          // ThermalState
  ChassisSecurityStatusNone, // SecurityStatus
  {0, 0, 0, 0},              // OemDefined
  0,                         // Height
  0,                         // NumberofPowerCords
  0,                         // ContainedElementCount
  0,                         // ContainedElementRecordLength
  {{0}},                     // ContainedElements
};

CHAR8 *mEnclosureInfoType3Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

#endif /* _TYPE_3_H_ */
