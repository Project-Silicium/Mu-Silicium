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

#ifndef _TYPE_19_H_
#define _TYPE_19_H_

SMBIOS_TABLE_TYPE19 mMemArrMapInfoType19 = {
  {EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS, sizeof(SMBIOS_TABLE_TYPE19), 0},
  0xFFFFFFFF,    // StartingAddress
  0xFFFFFFFF,    // EndingAddress
  0,             // MemoryArrayHandle
  0,             // PartitionWidth
  0x3FFFFFFFC00, // ExtendedStartingAddress
  0x3FFFFFFFFFF  // ExtendedEndingAddress
};

CHAR8 *mMemArrMapInfoType19Strings[] = { NULL };

#endif /* _TYPE_19_H_ */
