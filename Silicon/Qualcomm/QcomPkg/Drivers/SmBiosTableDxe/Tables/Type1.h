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

#ifndef _TYPE_1_H_
#define _TYPE_1_H_

SMBIOS_TABLE_TYPE1 mSysInfoType1 = {
  {EFI_SMBIOS_TYPE_SYSTEM_INFORMATION, sizeof(SMBIOS_TABLE_TYPE1), 0},
  1, // Manufacturer String
  2, // ProductName String
  3, // Version String
  4, // SerialNumber String
  {
    0x00000000,
    0x0000,
    0x0000,
    {
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
    }
  },
  SystemWakeupTypePowerSwitch, // WakeUpType
  5,                           // SKUNumber String
  6,                           // Family String
};

CHAR8 *mSysInfoType1Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

#endif /* _TYPE_1_H_ */
