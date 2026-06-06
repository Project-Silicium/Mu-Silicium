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

#ifndef _TYPE_4_TABLE_H_
#define _TYPE_4_TABLE_H_

SMBIOS_TABLE_TYPE4 mProcessorInfoType4 = {
  {EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof(SMBIOS_TABLE_TYPE4), 0},
  1,                               // Socket String
  CentralProcessor,                // ProcessorType
  ProcessorFamilyIndicatorFamily2, // ProcessorFamily
  2,                               // ProcessorManufacturer String
  {
    0, // SocId
    0, // SipId
    0, // SipBankIndex
    0  // SocRevision
  },
  3, // ProcessorVersion String
  {
    0, // ProcessorVoltageCapability5V
    0, // ProcessorVoltageCapability3_3V
    0, // ProcessorVoltageCapability2_9V
    0, // ProcessorVoltageCapabilityReserved
    0, // ProcessorVoltageReserved
    0  // ProcessorVoltageIndicateLegacy
  },
  0,                     // ExternalClock
  0,                     // MaxSpeed
  0,                     // CurrentSpeed
  0x41,                  // Status
  ProcessorUpgradeOther, // ProcessorUpgrade
  0,                     // L1CacheHandle
  0,                     // L2CacheHandle
  0,                     // L3CacheHandle
  4,                     // SerialNumber String
  5,                     // AssetTag String
  6,                     // PartNumber String
  0,                     // CoreCount
  0,                     // EnabledCoreCount
  0,                     // ThreadCount
  0xAC,                  // ProcessorCharacteristics
  ProcessorFamilyARM,    // ProcessorFamily2
  0,                     // CoreCount2
  0,                     // EnabledCoreCount2
  0,                     // ThreadCount2
  0,                     // ThreadEnabled
  7                      // SocketType String
};

CHAR8 *mProcessorInfoType4Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

#endif /* _TYPE_4_TABLE_H_ */
