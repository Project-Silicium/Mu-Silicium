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

#ifndef _TYPE_16_H_
#define _TYPE_16_H_

SMBIOS_TABLE_TYPE16 mPhyMemArrayInfoType16 = {
  {EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, sizeof(SMBIOS_TABLE_TYPE16), 0},
  MemoryArrayLocationSystemBoard, // Location
  MemoryArrayUseSystemMemory,     // Use
  MemoryErrorCorrectionUnknown,   // MemoryErrorCorrection
  0xFFFFFFFF,                     // MaximumCapacity
  0xFFFE,                         // MemoryErrorInformationHandle
  1,                              // NumberOfMemoryDevices
  0xFFFFFFFF                      // ExtendedMaximumCapacity
};

CHAR8 *mPhyMemArrayInfoType16Strings[] = { NULL };

#endif /* _TYPE_16_H_ */
