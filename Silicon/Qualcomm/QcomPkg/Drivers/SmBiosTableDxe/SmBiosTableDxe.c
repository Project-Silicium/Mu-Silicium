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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/RamPartitionTableLib.h>

#include <IndustryStandard/SmBios.h>

#include <Protocol/Smbios.h>
#include <Protocol/EFIChipInfo.h>
#include <Protocol/EFIClock.h>
#include <Protocol/EFIDDRGetConfig.h>

#include "DataDefinitions.h"

EFI_STATUS
EFIAPI
LogSmbiosData (
  IN  EFI_SMBIOS_TABLE_HEADER  *Template,
  IN  CHAR8                   **StringPack,
  OUT EFI_SMBIOS_HANDLE        *DataSmbiosHande)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *mSmbiosProtocol;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  UINTN                    StringSize;
  UINTN                    Size;
  CHAR8                   *Str;

  // Locate SmBios Protocol
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID *)&mSmbiosProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SmBios Protocol! Status = %r\n", Status));
    return Status;
  }

  // Set Dummy Size
  Size = Template->Length;

  // Get String Size
  if (StringPack == NULL) {
    Size += 2;
  } else {
    for (UINTN Index = 0; StringPack[Index] != NULL; Index++) {
      StringSize = AsciiStrSize (StringPack[Index]);
      Size      += StringSize;
    }

    if (StringPack[0] == NULL) {
      Size += 1;
    }

    Size += 1;
  }

  // Allocate Memory
  Record = (EFI_SMBIOS_TABLE_HEADER *)AllocateZeroPool (Size);
  if (Record == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for SmBios Record!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Copy Record Memory
  CopyMem (Record, Template, Template->Length);

  // Set Str
  Str = ((CHAR8 *)Record) + Record->Length;

  // Update Size
  for (UINTN Index = 0; StringPack[Index] != NULL; Index++) {
    StringSize = AsciiStrSize (StringPack[Index]);

    CopyMem (Str, StringPack[Index], StringSize);

    Str += StringSize;
  }

  // Reset Variables
  *Str         = 0;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  // Register SmBios Structure
  Status = mSmbiosProtocol->Add (mSmbiosProtocol, gImageHandle, &SmbiosHandle, Record);
  ASSERT_EFI_ERROR (Status);

  // Save SmBios Handle
  if (DataSmbiosHande != NULL) {
    *DataSmbiosHande = SmbiosHandle;
  }

  // Free Memory
  FreePool (Record);

  return EFI_SUCCESS;
}

VOID
BIOSInfoUpdateSmbiosType0 ()
{
  CHAR8 FirmwareVendor[100] = "";
  CHAR8 FirmwareVersion[6]  = "";

  // Allocate Memory
  ZeroMem (FirmwareVendor, 100);
  ZeroMem (FirmwareVersion, 6);

  // Convert Firmware Vendor & Version String
  AsciiSPrintUnicodeFormat (FirmwareVendor,  sizeof(FirmwareVendor),  FixedPcdGetPtr (PcdFirmwareVendor));
  AsciiSPrintUnicodeFormat (FirmwareVersion, sizeof(FirmwareVersion), FixedPcdGetPtr (PcdFirmwareVersionString));

  // Append Device Maintainer
  if (FixedPcdGetPtr (PcdDeviceMaintainer) != "Not Specified") {
    AsciiSPrint (FirmwareVendor, sizeof(FirmwareVendor), "%a & %a", FirmwareVendor, FixedPcdGetPtr (PcdDeviceMaintainer));
  }

  // Update String Table
  mBIOSInfoType0Strings[0] = FirmwareVendor;
  mBIOSInfoType0Strings[1] = FirmwareVersion;
  mBIOSInfoType0Strings[2] = __DATE__;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBIOSInfoType0, mBIOSInfoType0Strings, NULL);
}

VOID
SysInfoUpdateSmbiosType1 ()
{
  // Update String Table
  mSysInfoType1Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemManufacturer);
  mSysInfoType1Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemModel);
  mSysInfoType1Strings[2] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemRetailModel);
  mSysInfoType1Strings[4] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemRetailSku);

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mSysInfoType1, mSysInfoType1Strings, NULL);
}

VOID
BoardInfoUpdateSmbiosType2 ()
{
  // Update String Table
  mBoardInfoType2Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemManufacturer);
  mBoardInfoType2Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosBoardModel);

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBoardInfoType2, mBoardInfoType2Strings, NULL);
}

VOID
EnclosureInfoUpdateSmbiosType3 ()
{
  // Update String Table
  mEnclosureInfoType3Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemManufacturer);

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mEnclosureInfoType3, mEnclosureInfoType3Strings, NULL);
}

VOID
ProcessorInfoUpdateSmbiosType4 ()
{
  EFI_STATUS             Status;
  EFI_CHIPINFO_PROTOCOL *mChipInfoProtocol;
  EFI_CLOCK_PROTOCOL    *mClockProtocol;
  CHAR8                  ProcessorModel[100];
  UINT32                 CpuFreq[2];

  // Locate Chip Info Protocol
  Status = gBS->LocateProtocol (&gEfiChipInfoProtocolGuid, NULL, (VOID *)&mChipInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Chip Info Protocol! Status = %r\n", Status));
  } else {
    UINT32 ChipSerialNumber;

    // Get Chip Serial Number
    Status = mChipInfoProtocol->GetSerialNumber (mChipInfoProtocol, &ChipSerialNumber);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Chip Serial Number! Status = %r\n", Status));
    } else {
      CHAR8 SerialNumber[9];

      // Allocate Memory
      ZeroMem (SerialNumber, 9);

      // Convert Serial Number
      AsciiSPrint (SerialNumber, sizeof(SerialNumber), "%x", ChipSerialNumber);

      // Update Serial Number String
      mProcessorInfoType4Strings[3] = SerialNumber;
    }
  }

  // Locate Clock Protocol
  Status = gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID *)&mClockProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Clock Protocol! Status = %r", Status));
  } else if (mClockProtocol->Version >= 0x10006) {
    UINT32 RequiredVoltage;
    UINT32 CpuPerfLevel[2];

    // Get Last Cluster
    UINT32 LastCluster = FixedPcdGet32 (PcdClusterCount) - 1;

    // Get Max CPU Perf Level
    Status = mClockProtocol->GetMaxPerfLevel (mClockProtocol, LastCluster, &CpuPerfLevel[0]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Max CPU Perf Level! Status = %r\n", Status));
    }

    // Get Current Perf Level
    Status = mClockProtocol->GetCpuPerfLevel (mClockProtocol, LastCluster, &CpuPerfLevel[1]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Current CPU Perf Level! Status = %r\n", Status));
    }

    // Get Max & Current CPU Freq
    for (UINT8 i = 0; i < 2; i++) {
      // Convert Perf Level
      Status = mClockProtocol->GetCpuPerfLevelFrequency (mClockProtocol, LastCluster, CpuPerfLevel[i], &CpuFreq[i], &RequiredVoltage);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Convert Perf Level %u! Status = %r\n", Status));
        continue;
      }

      // Convert Hz to MHz
      CpuFreq[i] = CpuFreq[i] / 1000000;
    }
  }

  if (!CpuFreq[0] || !CpuFreq[1]) {
    // Use PCD Overwrite
    CpuFreq[0] = FixedPcdGet32 (PcdMaxCpuFreq);
    CpuFreq[1] = FixedPcdGet32 (PcdMaxCpuFreq);
  }

  // Allocate Memory
  ZeroMem (ProcessorModel, 100);

  // Update SoC String
  if (CpuFreq[0] < 1000) {
    AsciiSPrint (ProcessorModel, sizeof(ProcessorModel), "%a @ %u MHz", FixedPcdGetPtr (PcdSmbiosProcessorModel), CpuFreq[0]);
  } else {
    UINT8 GhzValue[2];

    // Split GHz Freq
    GhzValue[0] = CpuFreq[0] / 1000;
    GhzValue[1] = (CpuFreq[0] / 10) % 100;

    // Append GHz Freq
    AsciiSPrint (ProcessorModel, sizeof(ProcessorModel), "%a @ %u.%u GHz", FixedPcdGetPtr (PcdSmbiosProcessorModel), GhzValue[0], GhzValue[1]);
  }

  // Update Max & Current Speed
  mProcessorInfoType4.MaxSpeed     = CpuFreq[0];
  mProcessorInfoType4.CurrentSpeed = CpuFreq[1];

  // Update Core Count
  mProcessorInfoType4.CoreCount        = FixedPcdGet32 (PcdCoreCount);
  mProcessorInfoType4.EnabledCoreCount = FixedPcdGet32 (PcdCoreCount);
  mProcessorInfoType4.ThreadCount      = FixedPcdGet32 (PcdCoreCount);

  // Update String Table
  mProcessorInfoType4Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmBiosProcessorSocket);
  mProcessorInfoType4Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmBiosProcessorManufacturer);
  mProcessorInfoType4Strings[2] = ProcessorModel;
  mProcessorInfoType4Strings[5] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorCodename);

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4, mProcessorInfoType4Strings, NULL);
}

VOID
CacheInfoUpdateSmbiosType7 ()
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  // Update Cache Size
  mCacheInfoType7_L1IC.MaximumCacheSize = FixedPcdGet32 (PcdLevel1InstCacheSize);
  mCacheInfoType7_L1IC.InstalledSize    = FixedPcdGet32 (PcdLevel1InstCacheSize);
  mCacheInfoType7_L1DC.MaximumCacheSize = FixedPcdGet32 (PcdLevel1DataCacheSize);
  mCacheInfoType7_L1DC.InstalledSize    = FixedPcdGet32 (PcdLevel1DataCacheSize);

  // Update String Table
  mCacheInfoType7_L1ICStrings[0] = "L1 Instruction Cache";
  mCacheInfoType7_L1DCStrings[0] = "L1 Data Cache";

  // Register SmBios Structures
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC, mCacheInfoType7_L1ICStrings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC, mCacheInfoType7_L1DCStrings, &SmbiosHandle);

  // Append SmBios Handle
  mProcessorInfoType4.L1CacheHandle = (UINT16)SmbiosHandle;

  if (FixedPcdGetBool (PcdHasLevel2Cache)) {
    // Update Cache Size
    mCacheInfoType7_L2C.MaximumCacheSize = FixedPcdGet32 (PcdLevel2CacheSize);
    mCacheInfoType7_L2C.InstalledSize    = FixedPcdGet32 (PcdLevel2CacheSize);

    // Update String Table
    mCacheInfoType7_L2CStrings[0] = "L2 Unified Cache";

    // Register SmBios Structure
    LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C, mCacheInfoType7_L2CStrings, &SmbiosHandle);

    // Append SmBios Handle
    mProcessorInfoType4.L2CacheHandle = (UINT16)SmbiosHandle;
  }

  if (FixedPcdGetBool (PcdHasLevel3Cache)) {
    // Update Cache Size
    mCacheInfoType7_L3C.MaximumCacheSize = FixedPcdGet32 (PcdLevel3CacheSize);
    mCacheInfoType7_L3C.InstalledSize    = FixedPcdGet32 (PcdLevel3CacheSize);

    // Update String Table
    mCacheInfoType7_L3CStrings[0] = "L3 Unified Cache";

    // Register SmBios Structure
    LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L3C, mCacheInfoType7_L3CStrings, &SmbiosHandle);

    // Append SmBios Handle
    mProcessorInfoType4.L3CacheHandle = (UINT16)SmbiosHandle;
  }
}

VOID
OemStringsInfoUpdateSmBiosType11 ()
{
  // Update String Table
  mOemStringsInfoType11Strings[0] = "https://github.com/Project-Silicium/Mu-Silicium";

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mOemStringsInfoType11, mOemStringsInfoType11Strings, NULL);
}

VOID
BiosLanguageInfoUpdateSmBiosType13 ()
{
  // Update String Table
  mBiosLanguageInfoType13Strings[0] = "English";

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBiosLanguageInfoType13, mBiosLanguageInfoType13Strings, NULL);
}

VOID
PhyMemArrayInfoUpdateSmbiosType16 (IN UINT64 SystemMemorySize)
{
  EFI_SMBIOS_HANDLE MemArraySmbiosHande;

  // Update Memory Size
  mPhyMemArrayInfoType16.ExtendedMaximumCapacity = SystemMemorySize;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mPhyMemArrayInfoType16, mPhyMemArrayInfoType16Strings, &MemArraySmbiosHande);

  // Append SmBios Handle
  mMemDevInfoType17.MemoryArrayHandle = MemArraySmbiosHande;
}

VOID
MemDevInfoUpdateSmbiosType17 (IN UINT64 SystemMemorySize)
{
  EFI_STATUS               Status;
  EFI_DDRGETINFO_PROTOCOL *mDdrInfoProtocol;

  // Locate DDR Info Protocol
  Status = gBS->LocateProtocol (&gEfiDDRGetInfoProtocolGuid, NULL, (VOID *)&mDdrInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate DDR Info Protocol! Status = %r\n", Status));
  } else {
    ddr_details_entry DdrInfos;
    UINT32            DdrFreq;

    // Get DDR Infos
    Status = mDdrInfoProtocol->GetDDRDetails (mDdrInfoProtocol, &DdrInfos);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get DDR Infos! Status = %r\n", Status));
    } else {
      // Set Memory Manufacturer
      switch (DdrInfos.manufacturer_id) {
        case 1:
          mMemDevInfoType17Strings[2] = "Samsung";
          break;

        case 6:
          mMemDevInfoType17Strings[2] = "Hynix";
          break;

        default:
          DEBUG ((EFI_D_WARN, "Unknown Memory Manufacturer Found\n"));
          break;
      }

      // Set Memory Type
      switch (DdrInfos.device_type) {
        case DDR_TYPE_PCDDR2:
          mMemDevInfoType17.MemoryType = MemoryTypeDdr2;
          break;

        case DDR_TYPE_PCDDR3:
          mMemDevInfoType17.MemoryType = MemoryTypeDdr3;
          break;

        case DDR_TYPE_LPDDR1:
          mMemDevInfoType17.MemoryType = MemoryTypeLpddr;
          break;

        case DDR_TYPE_LPDDR2:
          mMemDevInfoType17.MemoryType = MemoryTypeLpddr2;
          break;

        case DDR_TYPE_LPDDR3:
          mMemDevInfoType17.MemoryType = MemoryTypeLpddr3;
          break;

        case DDR_TYPE_LPDDR4:
        case DDR_TYPE_LPDDR4X:
          mMemDevInfoType17.MemoryType = MemoryTypeLpddr4;
          break;

        case DDR_TYPE_LPDDR5:
        case DDR_TYPE_LPDDR5X:
          mMemDevInfoType17.MemoryType = MemoryTypeLpddr5;
          break;

        default:
          DEBUG ((EFI_D_WARN, "Unknown Memory Type Found\n"));
          break;
      }

      // Update Manufacturer ID
      mMemDevInfoType17.ModuleManufacturerID                    = DdrInfos.manufacturer_id;
      mMemDevInfoType17.MemorySubsystemControllerManufacturerID = DdrInfos.manufacturer_id;
    }

    // Get Current DDR Freq
    Status = mDdrInfoProtocol->GetDDRFreq (mDdrInfoProtocol, &DdrFreq);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Current DDR Freq! Status = %r\n", Status));

      // Use PCD Overwrite
      mMemDevInfoType17.Speed                      = FixedPcdGet32 (PcdMemorySpeed) * 2;
      mMemDevInfoType17.ConfiguredMemoryClockSpeed = FixedPcdGet32 (PcdMemorySpeed);
    } else {
      // Update DDR Freq
      mMemDevInfoType17.Speed                      = DdrFreq * 2;
      mMemDevInfoType17.ConfiguredMemoryClockSpeed = DdrFreq;
    }
  }

  // Update Memory Size
  mMemDevInfoType17.Size = SystemMemorySize / 0x100000;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemDevInfoType17, mMemDevInfoType17Strings, NULL);
}

VOID
MemArrMapInfoUpdateSmbiosType19 (IN UINT64 SystemMemorySize)
{
  // Update Memory Start & End Address
  mMemArrMapInfoType19.StartingAddress = FixedPcdGet64 (PcdSystemMemoryBase) / 1024;
  mMemArrMapInfoType19.EndingAddress   = (SystemMemorySize + FixedPcdGet64 (PcdSystemMemoryBase) - 1) / 1024;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemArrMapInfoType19, mMemArrMapInfoType19Strings, NULL);
}

EFI_STATUS
EFIAPI
RegisterSmBiosTables (
  IN EFI_HANDLE        ImageHandle, 
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS         Status            = EFI_SUCCESS;
  RamPartitionTable *RamPartitionTable = NULL;
  UINT32             NumPartitions     = 0;
  UINT32             PartitionVersion  = 0;
  UINT64             SystemMemorySize  = 0;

  // Get RAM Partitions
  Status = GetRamPartitions (&RamPartitionTable, &PartitionVersion);
  NumPartitions = RamPartitionTable->NumPartitions;
  if (EFI_ERROR (Status) || (NumPartitions < 1) || (PartitionVersion == 1)) {
    FreePool (RamPartitionTable);

    SystemMemorySize = FixedPcdGet64 (PcdSystemMemorySize);
  } else {
    for (UINTN i = 0; i < NumPartitions; i++) {
      if (RamPartitionTable->RamPartitionEntry[i].Type != RAM_PART_SYS_MEMORY || RamPartitionTable->RamPartitionEntry[i].Category != RAM_PART_SDRAM) { continue; }

      SystemMemorySize += RamPartitionTable->RamPartitionEntry[i].AvailableLength;
    }

    UINTN DesignMemorySize = 0;

    while (SystemMemorySize >= DesignMemorySize) {
      DesignMemorySize += 0x40000000; // 1GB
    }

    SystemMemorySize = DesignMemorySize;
  }

  // Update SmBios Structures
  BIOSInfoUpdateSmbiosType0          ();
  SysInfoUpdateSmbiosType1           ();
  BoardInfoUpdateSmbiosType2         ();
  EnclosureInfoUpdateSmbiosType3     ();
  ProcessorInfoUpdateSmbiosType4     ();
  CacheInfoUpdateSmbiosType7         ();
  OemStringsInfoUpdateSmBiosType11   ();
  BiosLanguageInfoUpdateSmBiosType13 ();
  PhyMemArrayInfoUpdateSmbiosType16  (SystemMemorySize);
  MemDevInfoUpdateSmbiosType17       (SystemMemorySize);
  MemArrMapInfoUpdateSmbiosType19    (SystemMemorySize);

  return EFI_SUCCESS;
}
