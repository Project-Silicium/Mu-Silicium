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

#ifndef _TYPE_2_H_
#define _TYPE_2_H_

SMBIOS_TABLE_TYPE2 mBoardInfoType2 = {
  {EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION, sizeof(SMBIOS_TABLE_TYPE2), 0},
  1, // Manufacturer String
  2, // ProductName String
  3, // Version String
  4, // SerialNumber String
  5, // AssetTag String
  {
    1, // Motherboard
    0, // RequiresDaughterCard
    0, // Removable
    0, // Replaceable
    0, // HotSwappable
    0, // Reserved
  },
  6,                        // LocationInChassis String
  0,                        // ChassisHandle
  BaseBoardTypeMotherBoard, // BoardType
  0,                        // NumberOfContainedObjectHandles
  {0}                       // ContainedObjectHandles
};

CHAR8 *mBoardInfoType2Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

#endif /* _TYPE_2_H_ */
