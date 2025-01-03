/**
  Derived from EmulatorPkg package

  Note SMBIOS 2.7.1 Required structures:
  BIOS Information (Type 0)
  System Information (Type 1)
  Board Information (Type 2)
  System Enclosure (Type 3)
  Processor Information (Type 4) - CPU Driver
  Cache Information (Type 7) - For cache that is external to processor
  System Slots (Type 9) - If system has slots
  Physical Memory Array (Type 16)
  Memory Device (Type 17) - For each socketed system-memory Device
  Memory Array Mapped Address (Type 19) - One per contiguous block per
  Physical Memroy Array System Boot Information (Type 32)


  Copyright (c), 2017, Andrey Warkentin <andrey.warkentin@gmail.com>
  Copyright (c), 2018, Bingxing Wang <uefi-oss-projects@imbushuo.net>
  Copyright (c), Microsoft Corporation. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


  Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013 Linaro.org
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _DATA_DEFINITIONS_H_
#define _DATA_DEFINITIONS_H_

/***********************************************************************
    SMBIOS data definition  TYPE0  BIOS Information
************************************************************************/
SMBIOS_TABLE_TYPE0 mBIOSInfoType0 = {
  {EFI_SMBIOS_TYPE_BIOS_INFORMATION, sizeof(SMBIOS_TABLE_TYPE0), 0},
  1,      // Vendor String
  2,      // BiosVersion String
  0xE000, // BiosSegment
  3,      // BiosReleaseDate String
  0x7F,   // BiosSize
  {
    0, // Reserved
    0, // Unknown
    0, // BiosCharacteristicsNotSupported
    0, // IsaIsSupported
    0, // McaIsSupported
    0, // EisaIsSupported
    1, // PciIsSupported
    0, // PcmciaIsSupported
    0, // PlugAndPlayIsSupported
    0, // ApmIsSupported
    1, // BiosIsUpgradable
    1, // BiosShadowingAllowed
    0, // VlVesaIsSupported
    0, // EscdSupportIsAvailable
    0, // BootFromCdIsSupported
    1, // SelectableBootIsSupported
    0, // RomBiosIsSocketed
    0, // BootFromPcmciaIsSupported
    1, // EDDSpecificationIsSupported
    0, // JapaneseNecFloppyIsSupported
    0, // JapaneseToshibaFloppyIsSupported
    0, // Floppy525_360IsSupported
    0, // Floppy525_12IsSupported
    0, // Floppy35_720IsSupported
    0, // Floppy35_288IsSupported
    1, // PrintScreenIsSupported
    1, // Keyboard8042IsSupported
    1, // SerialIsSupported
    0, // PrinterIsSupported
    0, // CgaMonoIsSupported
    0, // NecPc98
    0  // ReservedForVendor
  },
  {
    0x03, // AcpiIsSupported
          // UsbLegacyIsSupported
          // AgpIsSupported
          // I2OBootIsSupported
          // Ls120BootIsSupported
          // AtapiZipDriveBootIsSupported
          // Boot1394IsSupported
          // SmartBatteryIsSupported

    0x05, // BiosBootSpecIsSupported
          // FunctionKeyNetworkBootIsSupported
          // TargetContentDistributionEnabled
          // UefiSpecificationSupported
          // VirtualMachineSupported
          // ExtensionByte2Reserved
  },
  0x01, // SystemBiosMajorRelease
  0x05, // SystemBiosMinorRelease
  0xFF, // EmbeddedControllerFirmwareMajorRelease
  0xFF, // EmbeddedControllerFirmwareMinorRelease
};

CHAR8 *mBIOSInfoType0Strings[] = {
  "Not Specified", // Vendor String
  "3.0.3",         // BiosVersion String
  __DATE__,        // BiosReleaseDate String
  NULL
};

/***********************************************************************
    SMBIOS data definition  TYPE1  System Information
************************************************************************/
SMBIOS_TABLE_TYPE1 mSysInfoType1 = {
  {EFI_SMBIOS_TYPE_SYSTEM_INFORMATION, sizeof(SMBIOS_TABLE_TYPE1), 0},
  1, // Manufacturer String
  2, // ProductName String
  3, // Version String
  4, // SerialNumber String
  {
    0xc08ac5fb,
    0x2a54,
    0x4369,
    {
      0x8c,
      0x51,
      0xaf,
      0x7d,
      0x06,
      0x0a,
      0x93,
      0xcc,
    },
  },
  SystemWakeupTypeUnknown,
  5, // SKUNumber String
  6, // Family String
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

/***********************************************************************
    SMBIOS data definition  TYPE2  Board Information
************************************************************************/
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
  {0}                       // ContainedObjectHandles[1]
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

/***********************************************************************
    SMBIOS data definition  TYPE3  Enclosure Information
************************************************************************/
SMBIOS_TABLE_TYPE3 mEnclosureInfoType3 = {
  {EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE, sizeof(SMBIOS_TABLE_TYPE3), 0},
  1,                      // Manufacturer String
  MiscChassisTypeUnknown, // Type
  2,                      // Version String
  3,                      // SerialNumber String
  4,                      // AssetTag String
  ChassisStateUnknown,    // BootupState
  ChassisStateUnknown,    // PowerSupplyState
  ChassisStateUnknown,    // ThermalState
  ChassisStateUnknown,    // SecurityStatus
  {0, 0, 0, 0},           // OemDefined[4]
  0,                      // Height
  0,                      // NumberofPowerCords
  0,                      // ContainedElementCount
  0,                      // ContainedElementRecordLength
  {{0}},                  // ContainedElements[1]
};

CHAR8 *mEnclosureInfoType3Strings[] = {
  "Not Specified",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

/***********************************************************************
    SMBIOS data definition  TYPE4  Processor Information
************************************************************************/
SMBIOS_TABLE_TYPE4 mProcessorInfoType4_m4 = {
  {EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof(SMBIOS_TABLE_TYPE4), 0},
  1,                               // Socket String
  CentralProcessor,                // ProcessorType
  ProcessorFamilyIndicatorFamily2, // ProcessorFamily
  2,                               // ProcessorManufacture String
  {
   {
     0, // ProcessorSteppingId
     0, // ProcessorModel
     0, // ProcessorFamily
     0, // ProcessorType
     0, // ProcessorReserved1
     0, // ProcessorXModel
     0, // ProcessorXFamily
     0, // ProcessorReserved2
   },
   {
     0, // ProcessorFpu
     0, // ProcessorVme
     0, // ProcessorDe
     0, // ProcessorPse
     0, // ProcessorTsc
     0, // ProcessorMsr
     0, // ProcessorPae
     0, // ProcessorMce
     0, // ProcessorCx8
     0, // ProcessorApic
     0, // ProcessorReserved1
     0, // ProcessorSep
     0, // ProcessorMtrr
     0, // ProcessorPge
     0, // ProcessorMca
     0, // ProcessorCmov
     0, // ProcessorPat
     0, // ProcessorPse36
     0, // ProcessorPsn
     0, // ProcessorClfsh
     0, // ProcessorReserved2
     0, // ProcessorDs
     0, // ProcessorAcpi
     0, // ProcessorMmx
     0, // ProcessorFxsr
     0, // ProcessorSse
     0, // ProcessorSse2
     0, // ProcessorSs
     0, // ProcessorReserved3
     0, // ProcessorTm
     0, // ProcessorReserved4
    }
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
  2730,                  // MaxSpeed
  2730,                  // CurrentSpeed
  0x41,                  // Status
  ProcessorUpgradeOther, // ProcessorUpgrade
  0,                     // L1CacheHandle
  0,                     // L2CacheHandle
  0,                     // L3CacheHandle
  0,                     // SerialNumber
  0,                     // AssetTag
  4,                     // PartNumber
  2,                     // CoreCount
  2,                     // EnabledCoreCount
  2,                     // ThreadCount
  0xAC,                  // ProcessorCharacteristics
  ProcessorFamilyARM,    // ARM Processor Family
};

SMBIOS_TABLE_TYPE4 mProcessorInfoType4_a75 = {
  {EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof(SMBIOS_TABLE_TYPE4), 0},
  1,                               // Socket String
  CentralProcessor,                // ProcessorType
  ProcessorFamilyIndicatorFamily2, // ProcessorFamily
  2,                               // ProcessorManufacture String
  {
   {
     0, // ProcessorSteppingId
     0, // ProcessorModel
     0, // ProcessorFamily
     0, // ProcessorType
     0, // ProcessorReserved1
     0, // ProcessorXModel
     0, // ProcessorXFamily
     0, // ProcessorReserved2
   },
   {
     0, // ProcessorFpu
     0, // ProcessorVme
     0, // ProcessorDe
     0, // ProcessorPse
     0, // ProcessorTsc
     0, // ProcessorMsr
     0, // ProcessorPae
     0, // ProcessorMce
     0, // ProcessorCx8
     0, // ProcessorApic
     0, // ProcessorReserved1
     0, // ProcessorSep
     0, // ProcessorMtrr
     0, // ProcessorPge
     0, // ProcessorMca
     0, // ProcessorCmov
     0, // ProcessorPat
     0, // ProcessorPse36
     0, // ProcessorPsn
     0, // ProcessorClfsh
     0, // ProcessorReserved2
     0, // ProcessorDs
     0, // ProcessorAcpi
     0, // ProcessorMmx
     0, // ProcessorFxsr
     0, // ProcessorSse
     0, // ProcessorSse2
     0, // ProcessorSs
     0, // ProcessorReserved3
     0, // ProcessorTm
     0, // ProcessorReserved4
    }
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
  2310,                  // MaxSpeed
  2310,                  // CurrentSpeed
  0x41,                  // Status
  ProcessorUpgradeOther, // ProcessorUpgrade
  0,                     // L1CacheHandle
  0,                     // L2CacheHandle
  0,                     // L3CacheHandle
  0,                     // SerialNumber
  0,                     // AssetTag
  4,                     // PartNumber
  2,                     // CoreCount
  2,                     // EnabledCoreCount
  2,                     // ThreadCount
  0xAC,                  // ProcessorCharacteristics
  ProcessorFamilyARM,    // ARM Processor Family
};

SMBIOS_TABLE_TYPE4 mProcessorInfoType4_a55 = {
  {EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof(SMBIOS_TABLE_TYPE4), 0},
  1,                               // Socket String
  CentralProcessor,                // ProcessorType
  ProcessorFamilyIndicatorFamily2, // ProcessorFamily
  2,                               // ProcessorManufacture String
  {
   {
     0, // ProcessorSteppingId
     0, // ProcessorModel
     0, // ProcessorFamily
     0, // ProcessorType
     0, // ProcessorReserved1
     0, // ProcessorXModel
     0, // ProcessorXFamily
     0, // ProcessorReserved2
   },
   {
     0, // ProcessorFpu
     0, // ProcessorVme
     0, // ProcessorDe
     0, // ProcessorPse
     0, // ProcessorTsc
     0, // ProcessorMsr
     0, // ProcessorPae
     0, // ProcessorMce
     0, // ProcessorCx8
     0, // ProcessorApic
     0, // ProcessorReserved1
     0, // ProcessorSep
     0, // ProcessorMtrr
     0, // ProcessorPge
     0, // ProcessorMca
     0, // ProcessorCmov
     0, // ProcessorPat
     0, // ProcessorPse36
     0, // ProcessorPsn
     0, // ProcessorClfsh
     0, // ProcessorReserved2
     0, // ProcessorDs
     0, // ProcessorAcpi
     0, // ProcessorMmx
     0, // ProcessorFxsr
     0, // ProcessorSse
     0, // ProcessorSse2
     0, // ProcessorSs
     0, // ProcessorReserved3
     0, // ProcessorTm
     0, // ProcessorReserved4
    }
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
  1950,                  // MaxSpeed
  1950,                  // CurrentSpeed
  0x41,                  // Status
  ProcessorUpgradeOther, // ProcessorUpgrade
  0,                     // L1CacheHandle
  0,                     // L2CacheHandle
  0,                     // L3CacheHandle
  0,                     // SerialNumber
  0,                     // AssetTag
  4,                     // PartNumber
  4,                     // CoreCount
  4,                     // EnabledCoreCount
  4,                     // ThreadCount
  0xAC,                  // ProcessorCharacteristics
  ProcessorFamilyARM,    // ARM Processor Family
};

CHAR8 *mProcessorInfoType4Strings[] = {
  "Samsung",
  "Samsung Electronics Co., Ltd.",
  "Not Specified",
  "Not Specified",
  "Not Specified",
  NULL
};

/***********************************************************************
    SMBIOS data definition  TYPE7  Cache Information
************************************************************************/
SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1IC_m4 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // Cache Configuration
  0x0020, // Maximum Size
  0x0020, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // Cache Speed unknown
  CacheErrorParity,       // Error Correction Multi
  CacheTypeInstruction,   // System Cache Type
  CacheAssociativity16Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1IC_a75 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // Cache Configuration
  0x0040, // Maximum Size
  0x0040, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // Cache Speed unknown
  CacheErrorParity,       // Error Correction Multi
  CacheTypeInstruction,   // System Cache Type
  CacheAssociativity16Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1IC_a55 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // Cache Configuration
  0x0020, // Maximum Size
  0x0020, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // Cache Speed unknown
  CacheErrorParity,       // Error Correction Multi
  CacheTypeInstruction,   // System Cache Type
  CacheAssociativity16Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1DC_m4 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // Cache Configuration
  0x0020, // Maximum Size
  0x0020, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // Cache Speed unknown
  CacheErrorParity,       // Error Correction Multi
  CacheTypeInstruction,   // System Cache Type
  CacheAssociativity16Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1DC_a75 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // Cache Configuration
  0x0040, // Maximum Size
  0x0040, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // Cache Speed unknown
  CacheErrorParity,       // Error Correction Multi
  CacheTypeInstruction,   // System Cache Type
  CacheAssociativity16Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1DC_a55 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // Cache Configuration
  0x0020, // Maximum Size
  0x0020, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // Cache Speed unknown
  CacheErrorParity,       // Error Correction Multi
  CacheTypeInstruction,   // System Cache Type
  CacheAssociativity16Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L2C_m4 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0281, // Cache Configuration
  0x0200, // Maximum Size
  0x0200, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                     // Cache Speed unknown
  CacheErrorParity,      // Error Correction Multi
  CacheTypeUnified,      // System Cache Type
  CacheAssociativity8Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L2C_a75 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0281, // Cache Configuration
  0x0100, // Maximum Size
  0x0100, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                     // Cache Speed unknown
  CacheErrorParity,      // Error Correction Multi
  CacheTypeUnified,      // System Cache Type
  CacheAssociativity8Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L2C_a55 = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0281, // Cache Configuration
  0x0080, // Maximum Size
  0x0080, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                     // Cache Speed unknown
  CacheErrorParity,      // Error Correction Multi
  CacheTypeUnified,      // System Cache Type
  CacheAssociativity8Way // Associativity
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L3C = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0282, // Cache Configuration
  0x0400, // Maximum Size
  0x0400, // Install Size
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    0, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PiplelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                     // Cache Speed unknown
  CacheErrorParity,      // Error Correction Multi
  CacheTypeUnified,      // System Cache Type
  CacheAssociativity8Way // Associativity
};

CHAR8 *mCacheInfoType7_L1ICStrings[] = {
  "L1 Instruction Cache",
  NULL
};

CHAR8 *mCacheInfoType7_L1DCStrings[] = {
  "L1 Data Cache",
  NULL
};

CHAR8 *mCacheInfoType7_L2CStrings[] = {
  "L2 Cache",
  NULL
};

CHAR8 *mCacheInfoType7_L3CStrings[] = {
  "L3 Cache",
  NULL
};

/***********************************************************************
    SMBIOS data definition  TYPE16  Physical Memory ArrayInformation
************************************************************************/
SMBIOS_TABLE_TYPE16 mPhyMemArrayInfoType16 = {
  {EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, sizeof(SMBIOS_TABLE_TYPE16), 0},
  MemoryArrayLocationSystemBoard,    // Location
  MemoryArrayUseSystemMemory,        // Use
  MemoryErrorCorrectionNone,         // MemoryErrorCorrection
  0xFFFFFFFF,                        // MaximumCapacity
  0xFFFE,                            // MemoryErrorInformationHandle
  1,                                 // NumberOfMemoryDevices
  0xFFFFFFFF                         // ExtendedMaximumCapacity
};

CHAR8 *mPhyMemArrayInfoType16Strings[] = {NULL};

/***********************************************************************
    SMBIOS data definition  TYPE17  Memory Device Information
************************************************************************/
SMBIOS_TABLE_TYPE17 mMemDevInfoType17 = {
  {EFI_SMBIOS_TYPE_MEMORY_DEVICE, sizeof(SMBIOS_TABLE_TYPE17), 0},
  0,                          // MemoryArrayHandle
  0xFFFE,                     // MemoryErrorInformationHandle
  0xFFFF,                     // TotalWidth
  0xFFFF,                     // DataWidth
  0xFFFF,                     // Size
  MemoryFormFactorRowOfChips, // FormFactor
  0xff,                       // DeviceSet
  0,                          // DeviceLocator String
  1,                          // BankLocator String
  MemoryTypeLpddr4,           // MemoryType
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
  2133, // Speed
  2,    // Manufacturer String
  3,    // SerialNumber String
  4,    // AssetTag String
  5,    // PartNumber String
  0,    // Attributes
  0,    // ExtendedSize
  0,    // ConfiguredMemoryClockSpeed
};

CHAR8 *mMemDevInfoType17Strings[] = {
  "Builtin",
  "BANK 0",
  NULL
};

/***********************************************************************
    SMBIOS data definition  TYPE19  Memory Array Mapped Address Information
************************************************************************/
SMBIOS_TABLE_TYPE19 mMemArrMapInfoType19 = {
  {
    EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS,
    sizeof(SMBIOS_TABLE_TYPE19),
    0,
  },
  0xFFFFFFFF,    // StartingAddress
  0xFFFFFFFF,    // EndingAddress
  0,             // MemoryArrayHandle
  0,             // PartitionWidth
  0x3FFFFFFFC00, // ExtendedStartingAddress
  0x3FFFFFFFFFF, // ExtendedEndingAddress
};

CHAR8 *mMemArrMapInfoType19Strings[] = {NULL};

#endif /* _DATA_DEFINITIONS_H_ */