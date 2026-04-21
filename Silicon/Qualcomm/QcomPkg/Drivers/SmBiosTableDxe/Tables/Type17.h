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

#ifndef _TYPE_17_H_
#define _TYPE_17_H_

SMBIOS_TABLE_TYPE17 mMemDevInfoType17 = {
  {EFI_SMBIOS_TYPE_MEMORY_DEVICE, sizeof(SMBIOS_TABLE_TYPE17), 0},
  0,                       // MemoryArrayHandle
  0xFFFE,                  // MemoryErrorInformationHandle
  0xFFFF,                  // TotalWidth
  0xFFFF,                  // DataWidth
  0xFFFF,                  // Size
  MemoryFormFactorUnknown, // FormFactor
  0xFF,                    // DeviceSet
  1,                       // DeviceLocator String
  2,                       // BankLocator String
  MemoryTypeUnknown,       // MemoryType
  {
    0, // Reserved
    0, // Other
    1, // Unknown
    0, // FastPaged
    0, // StaticColumn
    0, // PseudoStatic
    0, // Rambus
    0, // Synchronous
    0, // Cmos
    0, // Edo
    0, // WindowDram
    0, // CacheDram
    0, // Nonvolatile
    0, // Registered
    0, // Unbuffered
    0, // Reserved1
  },
  0,                    // Speed
  3,                    // Manufacturer String
  4,                    // SerialNumber String
  5,                    // AssetTag String
  6,                    // PartNumber String
  0,                    // Attributes
  0,                    // ExtendedSize
  0,                    // ConfiguredMemoryClockSpeed
  0,                    // MinimumVoltage
  0,                    // MaximumVoltage
  0,                    // ConfiguredVoltage
  MemoryTechnologyDram, // MemoryTechnology
  {
    {
      0, // Reserved
      0, // Other
      1, // Unknown
      0, // VolatileMemory
      0, // ByteAccessiblePersistentMemory
      0, // BlockAccessiblePersistentMemory
      0  // Reserved2
    }
  },
  7, // FirmwareVersion String
  0, // ModuleManufacturerID
  0, // ModuleProductID
  0, // MemorySubsystemControllerManufacturerID
  0, // MemorySubsystemControllerProductID
  0, // NonVolatileSize
  0, // VolatileSize
  0, // CacheSize
  0, // LogicalSize
  0, // ExtendedSpeed
  0  // ExtendedConfiguredMemorySpeed
};

CHAR8 *mMemDevInfoType17Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

#endif /* _TYPE_17_H_ */
