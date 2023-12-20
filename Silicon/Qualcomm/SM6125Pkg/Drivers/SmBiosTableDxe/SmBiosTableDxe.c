/** @file
  SMBIOS Table for Snapdragon 665
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

#include <Base.h>

#include <Guid/SmBios.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>

/* Used to read chip serial number */
#include <Protocol/EFIChipInfo.h>

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
        // BiosCharacteristics
        0, //  Reserved                          :2;  ///< Bits 0-1.
        0, //  Unknown                           :1;
        0, //  BiosCharacteristicsNotSupported   :1;
        0, //  IsaIsSupported                    :1;
        0, //  McaIsSupported                    :1;
        0, //  EisaIsSupported                   :1;
        1, //  PciIsSupported                    :1;
        0, //  PcmciaIsSupported                 :1;
        0, //  PlugAndPlayIsSupported            :1;
        0, //  ApmIsSupported                    :1;
        1, //  BiosIsUpgradable                  :1;
        1, //  BiosShadowingAllowed              :1;
        0, //  VlVesaIsSupported                 :1;
        0, //  EscdSupportIsAvailable            :1;
        0, //  BootFromCdIsSupported             :1;
        1, //  SelectableBootIsSupported         :1;
        0, //  RomBiosIsSocketed                 :1;
        0, //  BootFromPcmciaIsSupported         :1;
        1, //  EDDSpecificationIsSupported       :1;
        1, //  JapaneseNecFloppyIsSupported      :1;
        0, //  JapaneseToshibaFloppyIsSupported  :1;
        0, //  Floppy525_360IsSupported          :1;
        0, //  Floppy525_12IsSupported           :1;
        0, //  Floppy35_720IsSupported           :1;
        0, //  Floppy35_288IsSupported           :1;
        1, //  PrintScreenIsSupported            :1;
        1, //  Keyboard8042IsSupported           :1;
        0, //  SerialIsSupported                 :1;
        0, //  PrinterIsSupported                :1;
        0, //  CgaMonoIsSupported                :1;
        0, //  NecPc98                           :1;
        0 //  ReservedForVendor                 :32; ///< Bits 32-63. Bits 32-47
          //  reserved for BIOS vendor
          ///< and bits 48-63 reserved for System Vendor.
    },
    {
        // BIOSCharacteristicsExtensionBytes[]
        0x03, //  AcpiIsSupported                   :1;
              //  UsbLegacyIsSupported              :1;
              //  AgpIsSupported                    :1;
              //  I2OBootIsSupported                :1;
              //  Ls120BootIsSupported              :1;
              //  AtapiZipDriveBootIsSupported      :1;
              //  Boot1394IsSupported               :1;
              //  SmartBatteryIsSupported           :1;
              //  BIOSCharacteristicsExtensionBytes[1]
        0x05, //  BiosBootSpecIsSupported              :1;
              //  FunctionKeyNetworkBootIsSupported    :1;
              //  TargetContentDistributionEnabled     :1;
              //  UefiSpecificationSupported           :1;
              //  VirtualMachineSupported              :1;
              //  ExtensionByte2Reserved               :3;
    },
    0x01, // SystemBiosMajorRelease
    0x05, // SystemBiosMinorRelease
    0xFF, // EmbeddedControllerFirmwareMajorRelease
    0xFF, // EmbeddedControllerFirmwareMinorRelease
};

CHAR8 *mBIOSInfoType0Strings[] = {
    "Not Specified", // Vendor String
    "Not Specified", // BiosVersion String
    __DATE__,        // BiosReleaseDate String
    NULL};

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
    NULL};

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
        // FeatureFlag
        1, //  Motherboard           :1;
        0, //  RequiresDaughterCard  :1;
        0, //  Removable             :1;
        0, //  Replaceable           :1;
        0, //  HotSwappable          :1;
        0, //  Reserved              :3;
    },
    6,                        // LocationInChassis String
    0,                        // ChassisHandle;
    BaseBoardTypeMotherBoard, // BoardType;
    0,                        // NumberOfContainedObjectHandles;
    {0}                       // ContainedObjectHandles[1];
};
CHAR8 *mBoardInfoType2Strings[] = {
    "Not Specified",
    "Not Specified",
    "Not Specified",
    "Not Specified",
    "Not Specified",
    "Not Specified",
    NULL};

/***********************************************************************
        SMBIOS data definition  TYPE3  Enclosure Information
************************************************************************/
SMBIOS_TABLE_TYPE3 mEnclosureInfoType3 = {
    {EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE, sizeof(SMBIOS_TABLE_TYPE3), 0},
    1,                      // Manufacturer String
    MiscChassisTypeUnknown, // Type;
    2,                      // Version String
    3,                      // SerialNumber String
    4,                      // AssetTag String
    ChassisStateUnknown,    // BootupState;
    ChassisStateUnknown,    // PowerSupplyState;
    ChassisStateUnknown,    // ThermalState;
    ChassisStateUnknown,    // SecurityStatus;
    {0, 0, 0, 0},           // OemDefined[4];
    0,                      // Height;
    0,                      // NumberofPowerCords;
    0,                      // ContainedElementCount;
    0,                      // ContainedElementRecordLength;
    {{0}},                  // ContainedElements[1];
};
CHAR8 *mEnclosureInfoType3Strings[] = {
    "Not Specified", "Not Specified", "Not Specified", "Not Specified",
    NULL};

/***********************************************************************
        SMBIOS data definition  TYPE4  Processor Information
************************************************************************/
SMBIOS_TABLE_TYPE4 mProcessorInfoType4 = {
    {EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION, sizeof(SMBIOS_TABLE_TYPE4), 0},
    1,                // Socket String
    CentralProcessor, // ProcessorType;				      ///< The
                      // enumeration value from PROCESSOR_TYPE_DATA.
    ProcessorFamilyIndicatorFamily2, // ProcessorFamily;        ///< The
                                     // enumeration value from
                                     // PROCESSOR_FAMILY2_DATA.
    2,                               // ProcessorManufacture String;
    {                                // ProcessorId;
     {
         // PROCESSOR_SIGNATURE
         0, //  ProcessorSteppingId:4;
         0, //  ProcessorModel:     4;
         0, //  ProcessorFamily:    4;
         0, //  ProcessorType:      2;
         0, //  ProcessorReserved1: 2;
         0, //  ProcessorXModel:    4;
         0, //  ProcessorXFamily:   8;
         0, //  ProcessorReserved2: 4;
     },

     {
         // PROCESSOR_FEATURE_FLAGS
         0, //  ProcessorFpu       :1;
         0, //  ProcessorVme       :1;
         0, //  ProcessorDe        :1;
         0, //  ProcessorPse       :1;
         0, //  ProcessorTsc       :1;
         0, //  ProcessorMsr       :1;
         0, //  ProcessorPae       :1;
         0, //  ProcessorMce       :1;
         0, //  ProcessorCx8       :1;
         0, //  ProcessorApic      :1;
         0, //  ProcessorReserved1 :1;
         0, //  ProcessorSep       :1;
         0, //  ProcessorMtrr      :1;
         0, //  ProcessorPge       :1;
         0, //  ProcessorMca       :1;
         0, //  ProcessorCmov      :1;
         0, //  ProcessorPat       :1;
         0, //  ProcessorPse36     :1;
         0, //  ProcessorPsn       :1;
         0, //  ProcessorClfsh     :1;
         0, //  ProcessorReserved2 :1;
         0, //  ProcessorDs        :1;
         0, //  ProcessorAcpi      :1;
         0, //  ProcessorMmx       :1;
         0, //  ProcessorFxsr      :1;
         0, //  ProcessorSse       :1;
         0, //  ProcessorSse2      :1;
         0, //  ProcessorSs        :1;
         0, //  ProcessorReserved3 :1;
         0, //  ProcessorTm        :1;
         0, //  ProcessorReserved4 :2;
     }},
    3, // ProcessorVersion String;
    {
        // Voltage;
        0, // ProcessorVoltageCapability5V        :1;
        0, // ProcessorVoltageCapability3_3V      :1;
        0, // ProcessorVoltageCapability2_9V      :1;
        0, // ProcessorVoltageCapabilityReserved  :1; ///< Bit 3, must be zero.
        0, // ProcessorVoltageReserved            :3; ///< Bits 4-6, must be
           // zero.
        0  // ProcessorVoltageIndicateLegacy      :1;
    },
    0,                     // ExternalClock;
    2000,                  // MaxSpeed;
    2000,                  // CurrentSpeed;
    0x41,                  // Status;
    ProcessorUpgradeOther, // ProcessorUpgrade;      ///< The enumeration value
                           // from PROCESSOR_UPGRADE.
    0,                     // L1CacheHandle;
    0,                     // L2CacheHandle;
    0,                     // L3CacheHandle;
    0,                     // SerialNumber;
    0,                     // AssetTag;
    4,                     // PartNumber;
    8,                     // CoreCount;
    8,                     // EnabledCoreCount;
    8,                     // ThreadCount;
    0xAC,                        // ProcessorCharacteristics;
    ProcessorFamilyARM,          // ARM Processor Family;
};

CHAR8 *mProcessorInfoType4Strings[] = {
    "Qualcomm", "Qualcomm Technologies Inc", "Not Specified", "Not Specified",
    NULL};

/***********************************************************************
        SMBIOS data definition  TYPE7  Cache Information
************************************************************************/
SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1IC = {
    {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
    1,      // SocketDesignation String
    0x0280, // Cache Configuration
    0x0040, // Maximum Size
    0x0040, // Install Size
    {
        // Supported SRAM Type
        0, // Other             :1
        0, // Unknown           :1
        0, // NonBurst          :1
        0, // Burst             :1
        0, // PiplelineBurst    :1
        0, // Synchronous       :1
        0, // Asynchronous      :1
        0  // Reserved          :9
    },
    {
        // Current SRAM Type
        0, // Other             :1
        0, // Unknown           :1
        0, // NonBurst          :1
        0, // Burst             :1
        0, // PiplelineBurst    :1
        0, // Synchronous       :1
        0, // Asynchronous      :1
        0  // Reserved          :9
    },
    0,                      // Cache Speed unknown
    CacheErrorParity,       // Error Correction Multi
    CacheTypeInstruction,   // System Cache Type
    CacheAssociativity16Way // Associativity
};
CHAR8 *mCacheInfoType7_L1ICStrings[] = {"L1 Instruction Cache", NULL};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1DC = {
    {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
    1,      // SocketDesignation String
    0x0280, // Cache Configuration
    0x0040, // Maximum Size
    0x0040, // Install Size
    {
        // Supported SRAM Type
        0, // Other             :1
        0, // Unknown           :1
        0, // NonBurst          :1
        0, // Burst             :1
        0, // PiplelineBurst    :1
        0, // Synchronous       :1
        0, // Asynchronous      :1
        0  // Reserved          :9
    },
    {
        // Current SRAM Type
        0, // Other             :1
        0, // Unknown           :1
        0, // NonBurst          :1
        0, // Burst             :1
        0, // PiplelineBurst    :1
        0, // Synchronous       :1
        0, // Asynchronous      :1
        0  // Reserved          :9
    },
    0,                      // Cache Speed unknown
    CacheErrorParity,       // Error Correction Multi
    CacheTypeData,          // System Cache Type
    CacheAssociativity16Way // Associativity
};
CHAR8 *mCacheInfoType7_L1DCStrings[] = {"L1 Data Cache", NULL};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L2C = {
    {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
    1,      // SocketDesignation String
    0x0281, // Cache Configuration
    0x0400, // Maximum Size
    0x0400, // Install Size
    {
        // Supported SRAM Type
        0, // Other             :1
        0, // Unknown           :1
        0, // NonBurst          :1
        0, // Burst             :1
        0, // PiplelineBurst    :1
        0, // Synchronous       :1
        0, // Asynchronous      :1
        0  // Reserved          :9
    },
    {
        // Current SRAM Type
        0, // Other             :1
        0, // Unknown           :1
        0, // NonBurst          :1
        0, // Burst             :1
        0, // PiplelineBurst    :1
        0, // Synchronous       :1
        0, // Asynchronous      :1
        0  // Reserved          :9
    },
    0,                     // Cache Speed unknown
    CacheErrorParity,      // Error Correction Multi
    CacheTypeUnified,      // System Cache Type
    CacheAssociativity8Way // Associativity
};
CHAR8 *mCacheInfoType7_L2CStrings[] = {"L2 Cache", NULL};

/***********************************************************************
        SMBIOS data definition  TYPE16  Physical Memory ArrayInformation
************************************************************************/
SMBIOS_TABLE_TYPE16 mPhyMemArrayInfoType16 = {
    {EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY, sizeof(SMBIOS_TABLE_TYPE16), 0},
    MemoryArrayLocationSystemBoard, // Location;                       ///< The
                                    // enumeration value from
                                    // MEMORY_ARRAY_LOCATION.
    MemoryArrayUseSystemMemory,     // Use;                            ///< The
                                    // enumeration value from MEMORY_ARRAY_USE.
    MemoryErrorCorrectionNone,      // MemoryErrorCorrection;          ///< The
                                    // enumeration value from
                                    // MEMORY_ERROR_CORRECTION.
    0xFFFFFFFF,                     // MaximumCapacity;
    0xFFFE,                         // MemoryErrorInformationHandle;
    1,                              // NumberOfMemoryDevices;
    FixedPcdGet64(PcdSystemMemorySize) // ExtendedMaximumCapacity;
};
CHAR8 *mPhyMemArrayInfoType16Strings[] = {NULL};

/***********************************************************************
        SMBIOS data definition  TYPE17  Memory Device Information
************************************************************************/
SMBIOS_TABLE_TYPE17 mMemDevInfoType17 = {
    {EFI_SMBIOS_TYPE_MEMORY_DEVICE, sizeof(SMBIOS_TABLE_TYPE17), 0},
    0, // MemoryArrayHandle; // Should match SMBIOS_TABLE_TYPE16.Handle,
       // initialized at runtime, refer to PhyMemArrayInfoUpdateSmbiosType16()
    0xFFFE, // MemoryErrorInformationHandle;
    0xFFFF, // TotalWidth;
    0xFFFF, // DataWidth;
    0xFFFF, // Size; // When bit 15 is 0: Size in MB
            // When bit 15 is 1: Size in KB, and continues in ExtendedSize
    MemoryFormFactorRowOfChips, // FormFactor;                     ///< The
                                // enumeration value from MEMORY_FORM_FACTOR.
    0xff,                       // DeviceSet;
    0,                          // DeviceLocator String
    1,                          // BankLocator String
    MemoryTypeLpddr4, // MemoryType;                     ///< The enumeration
                      // value from MEMORY_DEVICE_TYPE.
    {
        // TypeDetail;
        0, // Reserved        :1;
        0, // Other           :1;
        1, // Unknown         :1;
        0, // FastPaged       :1;
        0, // StaticColumn    :1;
        0, // PseudoStatic    :1;
        0, // Rambus          :1;
        0, // Synchronous     :1;
        0, // Cmos            :1;
        0, // Edo             :1;
        0, // WindowDram      :1;
        0, // CacheDram       :1;
        0, // Nonvolatile     :1;
        0, // Registered      :1;
        0, // Unbuffered      :1;
        0, // Reserved1       :1;
    },
    1866, // Speed;
    2,    // Manufacturer String
    3,    // SerialNumber String
    4,    // AssetTag String
    5,    // PartNumber String
    0,    // Attributes;
    0,    // ExtendedSize;
    0,    // ConfiguredMemoryClockSpeed;
};

CHAR8 *mMemDevInfoType17Strings[] = {"Builtin", "BANK 0", NULL};

/***********************************************************************
        SMBIOS data definition  TYPE19  Memory Array Mapped Address Information
************************************************************************/
SMBIOS_TABLE_TYPE19 mMemArrMapInfoType19 = {
    {
        EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS,
        sizeof(SMBIOS_TABLE_TYPE19),
        0,
    },
    0xFFFFFFFF,    // StartingAddress;
    0xFFFFFFFF,    // EndingAddress;
    0,             // MemoryArrayHandle;
    0,             // PartitionWidth;
    0x3FFFFFFFC00, // ExtendedStartingAddress;
    0x3FFFFFFFFFF, // ExtendedEndingAddress;
};
CHAR8 *mMemArrMapInfoType19Strings[] = {NULL};

/**

  Create SMBIOS record.

  Converts a fixed SMBIOS structure and an array of pointers to strings into
  an SMBIOS record where the strings are cat'ed on the end of the fixed record
  and terminated via a double NULL and add to SMBIOS table.

  SMBIOS_TABLE_TYPE32 gSmbiosType12 = {
    { EFI_SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS, sizeof
(SMBIOS_TABLE_TYPE12), 0 }, 1 // StringCount
  };

  CHAR8 *gSmbiosType12Strings[] = {
    "Not Found",
    NULL
  };

  ...

  LogSmbiosData (
    (EFI_SMBIOS_TABLE_HEADER*)&gSmbiosType12,
    gSmbiosType12Strings
    );

  @param  Template    Fixed SMBIOS structure, required.
  @param  StringPack  Array of strings to convert to an SMBIOS string pack.
                      NULL is OK.
  @param  DataSmbiosHande  The new SMBIOS record handle .
                      NULL is OK.
**/

EFI_STATUS
EFIAPI
LogSmbiosData(
    IN EFI_SMBIOS_TABLE_HEADER *Template, IN CHAR8 **StringPack,
    OUT EFI_SMBIOS_HANDLE *DataSmbiosHande)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  UINTN                    Index;
  UINTN                    StringSize;
  UINTN                    Size;
  CHAR8                   *Str;

  //
  // Locate Smbios protocol.
  //
  Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);

  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Calculate the size of the fixed record and optional string pack

  Size = Template->Length;
  if (StringPack == NULL) {
    // At least a double null is required
    Size += 2;
  }
  else {
    for (Index = 0; StringPack[Index] != NULL; Index++) {
      StringSize = AsciiStrSize(StringPack[Index]);
      Size += StringSize;
    }
    if (StringPack[0] == NULL) {
      // At least a double null is required
      Size += 1;
    }

    // Don't forget the terminating double null
    Size += 1;
  }

  // Copy over Template
  Record = (EFI_SMBIOS_TABLE_HEADER *)AllocateZeroPool(Size);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem(Record, Template, Template->Length);

  // Append string pack
  Str = ((CHAR8 *)Record) + Record->Length;

  for (Index = 0; StringPack[Index] != NULL; Index++) {
    StringSize = AsciiStrSize(StringPack[Index]);
    CopyMem(Str, StringPack[Index], StringSize);
    Str += StringSize;
  }

  *Str         = 0;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status       = Smbios->Add(Smbios, gImageHandle, &SmbiosHandle, Record);

  if ((Status == EFI_SUCCESS) && (DataSmbiosHande != NULL)) {
    *DataSmbiosHande = SmbiosHandle;
  }

  ASSERT_EFI_ERROR(Status);
  FreePool(Record);
  return Status;
}

/***********************************************************************
        SMBIOS data update  TYPE0  BIOS Information
************************************************************************/
VOID BIOSInfoUpdateSmbiosType0(VOID)
{
  // Update string table before proceeds
  mBIOSInfoType0Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdFirmwareVendor);
  mBIOSInfoType0Strings[1] = (CHAR8 *)FixedPcdGetPtr(PcdFirmwareVersionString);

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mBIOSInfoType0, mBIOSInfoType0Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE1  System Information
************************************************************************/

VOID SysInfoUpdateSmbiosType1(CHAR8 *serialNo, EFIChipInfoSerialNumType serial)
{
  // Update string table before proceeds
  mSysInfoType1Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemVendor);
  mSysInfoType1Strings[1] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemModel);
  mSysInfoType1Strings[2] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemRetailModel);
  mSysInfoType1Strings[4] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemRetailSku);

  // Update serial number from Board DXE
  mSysInfoType1Strings[3]  = serialNo;
  mSysInfoType1.Uuid.Data1 = serial;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mSysInfoType1, mSysInfoType1Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE2  Board Information
************************************************************************/
VOID BoardInfoUpdateSmbiosType2(CHAR8 *serialNo)
{
  // Update string table before proceeds
  mBoardInfoType2Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemVendor);
  mBoardInfoType2Strings[1] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosBoardModel);

  // Update serial number from Board DXE
  mBoardInfoType2Strings[3] = serialNo;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mBoardInfoType2, mBoardInfoType2Strings,
      NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE3  Enclosure Information
************************************************************************/
VOID EnclosureInfoUpdateSmbiosType3(CHAR8 *serialNo)
{
  // Update string table before proceeds
  mEnclosureInfoType3Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemVendor);

  // Update serial number from Board DXE
  mEnclosureInfoType3Strings[2] = serialNo;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mEnclosureInfoType3,
      mEnclosureInfoType3Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE4  Processor Information
************************************************************************/
VOID ProcessorInfoUpdateSmbiosType4(VOID)
{
  // Update string table before proceeds
  mProcessorInfoType4Strings[2] =
      (CHAR8 *)FixedPcdGetPtr(PcdSmbiosProcessorModel);
  mProcessorInfoType4Strings[3] =
      (CHAR8 *)FixedPcdGetPtr(PcdSmbiosProcessorRetailModel);

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4,
      mProcessorInfoType4Strings, NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE7  Cache Information
************************************************************************/
VOID CacheInfoUpdateSmbiosType7(VOID)
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC,
      mCacheInfoType7_L1ICStrings, NULL);

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC,
      mCacheInfoType7_L1DCStrings, &SmbiosHandle);
  mProcessorInfoType4.L1CacheHandle = (UINT16)SmbiosHandle;
  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C,
      mCacheInfoType7_L2CStrings, &SmbiosHandle);
  mProcessorInfoType4.L2CacheHandle = (UINT16)SmbiosHandle;
}

/***********************************************************************
        SMBIOS data update  TYPE16  Physical Memory Array Information
************************************************************************/
VOID PhyMemArrayInfoUpdateSmbiosType16(VOID)
{
  EFI_SMBIOS_HANDLE MemArraySmbiosHande;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mPhyMemArrayInfoType16,
      mPhyMemArrayInfoType16Strings, &MemArraySmbiosHande);

  //
  // Update the memory device information
  //
  mMemDevInfoType17.MemoryArrayHandle = MemArraySmbiosHande;
}

/***********************************************************************
        SMBIOS data update  TYPE17  Memory Device Information
************************************************************************/
VOID MemDevInfoUpdateSmbiosType17(VOID)
{
  mMemDevInfoType17.Size = FixedPcdGet64(PcdSystemMemorySize) / 0x100000;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mMemDevInfoType17, mMemDevInfoType17Strings,
      NULL);
}

/***********************************************************************
        SMBIOS data update  TYPE19  Memory Array Map Information
************************************************************************/
VOID MemArrMapInfoUpdateSmbiosType19(VOID)
{
  mMemArrMapInfoType19.StartingAddress =
      FixedPcdGet64(PcdSystemMemoryBase) / 1024;
  mMemArrMapInfoType19.EndingAddress =
      (FixedPcdGet64(PcdSystemMemorySize) + FixedPcdGet64(PcdSystemMemoryBase) -
       1) /
      1024;

  LogSmbiosData(
      (EFI_SMBIOS_TABLE_HEADER *)&mMemArrMapInfoType19,
      mMemArrMapInfoType19Strings, NULL);
}

/***********************************************************************
        Driver Entry
************************************************************************/
EFI_STATUS
EFIAPI
SmBiosTableDxeInitialize(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS               Status;
  CHAR8                    serialNo[EFICHIPINFO_MAX_ID_LENGTH];
  UINTN                    serialNoLength = EFICHIPINFO_MAX_ID_LENGTH;
  EFIChipInfoSerialNumType serial;
  EFI_CHIPINFO_PROTOCOL   *mBoardProtocol  = NULL;

  // Locate Qualcomm Board Protocol
  Status = gBS->LocateProtocol(
      &gEfiChipInfoProtocolGuid, NULL, (VOID *)&mBoardProtocol);

  if (mBoardProtocol != NULL) {
    mBoardProtocol->GetSerialNumber(mBoardProtocol, &serial);
    ZeroMem(serialNo, serialNoLength);
    AsciiSPrint(serialNo, serialNoLength, "%lld", serial);
  }

  BIOSInfoUpdateSmbiosType0();
  SysInfoUpdateSmbiosType1(serialNo, serial);
  BoardInfoUpdateSmbiosType2(serialNo);
  EnclosureInfoUpdateSmbiosType3(serialNo);
  ProcessorInfoUpdateSmbiosType4();
  CacheInfoUpdateSmbiosType7();
  PhyMemArrayInfoUpdateSmbiosType16();
  MemDevInfoUpdateSmbiosType17();
  MemArrMapInfoUpdateSmbiosType19();

  return EFI_SUCCESS;
}