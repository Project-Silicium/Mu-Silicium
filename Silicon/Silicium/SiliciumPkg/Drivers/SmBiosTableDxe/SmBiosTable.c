/**
  Copyright (c) 2017, Andrey Warkentin <andrey.warkentin@gmail.com>
  Copyright (c) 2018, Bingxing Wang <uefi-oss-projects@imbushuo.net>
  Copyright (c) Microsoft Corporation. All rights reserved.
  Copyright (c) 2012, Apple Inc. All rights reserved.
  Copyright (c) 2013 Linaro.org

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/EfiMemoryMapUtilsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmBiosTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>

#include <Protocol/Smbios.h>

#include "SmBiosTable.h"
#include "TableDefinitions.h"

//
// Global Variables
//
STATIC EFI_SMBIOS_PROTOCOL *mSmBiosProtocol = NULL;

VOID
RegisterTable (
  IN  EFI_SMBIOS_TABLE_HEADER  *TableHeader,
  IN  CHAR8                   **StringPack,
  OUT EFI_SMBIOS_HANDLE        *DataSmbiosHandle)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmBiosHandle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  UINTN                    RecordSize;
  CHAR8                   *Strings;

  // Set Inital Record Size
  RecordSize = TableHeader->Length;

  // Verify String Pack
  if (StringPack == NULL || StringPack[0] == NULL) {
    // Append String Terminators
    RecordSize += 2;
  } else {
    // Go thru each String
    for (UINT8 i = 0; StringPack[i] != NULL; i++) {
      // Append String Size
      RecordSize += AsciiStrSize (StringPack[i]);
    }

    // Append String Terminator
    RecordSize += 1;
  }

  // Allocate Memory
  Record = AllocateZeroPool (RecordSize);
  if (Record == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for the SMBIOS Record!\n"));
    return;
  }

  // Copy Table Header
  CopyMem (Record, TableHeader, TableHeader->Length);

  // Save Strings
  Strings = (CHAR8 *)Record + Record->Length;

  // Verify String Pack
  if (StringPack != NULL && StringPack[0] != NULL) {
    // Go thru each String
    for (UINT8 i = 0; StringPack[i] != NULL; i++) {
      // Get String Size
      UINTN StringSize = AsciiStrSize (StringPack[i]);

      // Save String
      CopyMem (Strings, StringPack[i], StringSize);

      // Append String Size
      Strings += StringSize;
    }

    // Append String Terminator
    *Strings = 0;
  } else {
    // Append String Terminators
    *Strings       = 0;
    *(Strings + 1) = 0;
  }

  // Set SMBIOS Handle
  SmBiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  // Add SMBIOS Table
  Status = mSmBiosProtocol->Add (mSmBiosProtocol, gImageHandle, &SmBiosHandle, Record);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Add %u SMBIOS Table! Status = %r", Record->Type, Status));
  }

  // Pass SMBIOS Handle
  if (!EFI_ERROR (Status) && DataSmbiosHandle != NULL) {
    *DataSmbiosHandle = SmBiosHandle;
  }

  // Free Buffer
  FreePool (Record);
}

CHAR8*
ConvertFirmwareString (
  IN CHAR16 *String,
  IN UINTN   StringSize)
{
  // Allocate Memory
  CHAR8 *NewString = AllocateZeroPool (StringSize);
  if (NewString == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Firmware String!\n"));
    return "Not Specified";
  }

  // Convert String
  AsciiSPrintUnicodeFormat (NewString, StringSize, String);

  // Return new String
  return NewString;
}

VOID
UpdateSmBiosType0 ()
{
  // Update BIOS Size
  mSmBiosType0.Table.BiosSize = (UINT8)((FixedPcdGet32 (PcdFdSize) / SIZE_64KB) - 1);

  // Get Firmware Strings
  CHAR16 *FirmwareVendor    = (CHAR16 *)FixedPcdGetPtr (PcdFirmwareVendor);
  CHAR16 *FirmwareVersion   = (CHAR16 *)FixedPcdGetPtr (PcdFirmwareVersionString);
  CHAR16 *FirmwareBuildDate = SMBIOS_BUILD_DATE;

  // Set Firmware Strings
  mSmBiosType0.Strings[0] = ConvertFirmwareString (FirmwareVendor,    StrSize (FirmwareVendor));
  mSmBiosType0.Strings[1] = ConvertFirmwareString (FirmwareVersion,   StrSize (FirmwareVersion));
  mSmBiosType0.Strings[2] = ConvertFirmwareString (FirmwareBuildDate, StrSize (FirmwareBuildDate));
}

VOID
UpdateSmBiosType1 ()
{
  // Update System UUID
  mSmBiosType1.Table.Uuid = gSiliciumPkgTokenSpaceGuid;

  // Update System Strings
  mSmBiosType1.Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemManufacturer);
  mSmBiosType1.Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemModel);
  mSmBiosType1.Strings[2] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemRetailModel);
  mSmBiosType1.Strings[4] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemRetailSku);
}

VOID
UpdateSmBiosType2 ()
{
  // Update System Strings
  mSmBiosType2.Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemManufacturer);
  mSmBiosType2.Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemBoardModel);
}

VOID
UpdateSmBiosType3 ()
{
  // Update System Strings
  mSmBiosType3.Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosSystemManufacturer);
}

VOID
UpdateSmBiosType4 ()
{
  // Get ARMv9 Flag
  BOOLEAN Armv9 = IsArmv9 ();

  // Get MIDR
  UINT64 Midr = (UINT64)GetMidr ();

  // Update Processor MIDR
  CopyMem (&mSmBiosType4.Table.ProcessorId, &Midr, sizeof (PROCESSOR_ID_DATA));

  // Update Processor Core & Thread Count
  mSmBiosType4.Table.CoreCount        = FixedPcdGet32 (PcdCoreCount);
  mSmBiosType4.Table.EnabledCoreCount = FixedPcdGet32 (PcdCoreCount);
  mSmBiosType4.Table.ThreadCount      = FixedPcdGet32 (PcdCoreCount);
  mSmBiosType4.Table.ThreadEnabled    = FixedPcdGet32 (PcdCoreCount);

  // Set Multi Core Flag
  if (mSmBiosType4.Table.CoreCount > 1) {
    mSmBiosType4.Table.ProcessorCharacteristics |= BIT3;
  }

  // Update Processor Family
  mSmBiosType4.Table.ProcessorFamily2 = Armv9 ? ProcessorFamilyARMv9 : ProcessorFamilyARMv8;

  // Update Processor Strings
  mSmBiosType4.Strings[2] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorModel);
  mSmBiosType4.Strings[5] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorPartNumber);
}

VOID
UpdateSmBiosType7 ()
{
  CACHE_ASSOCIATIVITY_DATA L1iAssociativity;
  CACHE_ASSOCIATIVITY_DATA L1dAssociativity;
  CACHE_ASSOCIATIVITY_DATA L2Associativity;
  CACHE_ASSOCIATIVITY_DATA L3Associativity;
  UINT32                   L1iSize;
  UINT32                   L1dSize;
  UINT32                   L2Size;
  UINT32                   L3Size;

  // Get Cache Details
  GetCpuCacheDetails (1, TRUE,  &L1iSize, &L1iAssociativity);
  GetCpuCacheDetails (1, FALSE, &L1dSize, &L1dAssociativity);
  GetCpuCacheDetails (2, FALSE, &L2Size,  &L2Associativity);
  GetCpuCacheDetails (3, FALSE, &L3Size,  &L3Associativity);

  // Update L1I Cache Details
  mSmBiosType7_L1I.Table.InstalledSize.Size    = L1iSize;
  mSmBiosType7_L1I.Table.MaximumCacheSize.Size = L1iSize;
  mSmBiosType7_L1I.Table.Associativity         = L1iAssociativity;

  // Update L1D Cache Details
  mSmBiosType7_L1D.Table.InstalledSize.Size    = L1dSize;
  mSmBiosType7_L1D.Table.MaximumCacheSize.Size = L1dSize;
  mSmBiosType7_L1D.Table.Associativity         = L1dAssociativity;

  // Update L2 Cache Details
  mSmBiosType7_L2.Table.InstalledSize.Size     = L2Size;
  mSmBiosType7_L2.Table.MaximumCacheSize.Size  = L2Size;
  mSmBiosType7_L2.Table.Associativity          = L2Associativity;

  // Update L3 Cache Details
  mSmBiosType7_L3.Table.InstalledSize.Size     = L3Size;
  mSmBiosType7_L3.Table.MaximumCacheSize.Size  = L3Size;
  mSmBiosType7_L3.Table.Associativity          = L3Associativity;

  // Update Cache Strings
  mSmBiosType7_L1I.Strings[0] = "L1 Instruction Cache";
  mSmBiosType7_L1D.Strings[0] = "L1 Data Cache";
  mSmBiosType7_L2.Strings[0]  = "L2 Unified Cache";
  mSmBiosType7_L3.Strings[0]  = "L3 Unified Cache";
}

VOID
UpdateSmBiosType16 (IN UINT64 MemorySize)
{
  // Check Memory Size
  if (MemorySize < SIZE_2TB) {
    // Update Maximum Capacity
    mSmBiosType16.Table.MaximumCapacity = (UINT32)(MemorySize / SIZE_1KB);
  } else {
    // Update Maximum Capacity
    mSmBiosType16.Table.MaximumCapacity = 0x80000000;

    // Update Extended Maximum Capacity
    mSmBiosType16.Table.ExtendedMaximumCapacity = MemorySize;
  }
}

VOID
UpdateSmBiosType17 (IN UINT64 MemorySize)
{
  // Check Memory Size
  if (MemorySize < (SIZE_32GB - SIZE_1MB)) {
    // Update Memory Size
    mSmBiosType17.Table.Size = (UINT16)(MemorySize / SIZE_1MB);
  } else {
    // Update Memory Size
    mSmBiosType17.Table.Size = 0x7FFF;

    // Update Extended Memory Size
    mSmBiosType17.Table.ExtendedSize = MemorySize;
  }

  // Update Volatile Memory Size
  mSmBiosType17.Table.VolatileSize = MemorySize;
}

VOID
UpdateSmBiosType19 (
  IN EFI_PHYSICAL_ADDRESS MemoryBase,
  IN EFI_PHYSICAL_ADDRESS MemoryEnd)
{
  // Check Memory End
  if (MemoryEnd < SIZE_4TB) {
    // Update System Memory Details
    mSmBiosType19.Table.StartingAddress = (UINT32)(MemoryBase / SIZE_1KB);
    mSmBiosType19.Table.EndingAddress   = (UINT32)(MemoryEnd / SIZE_1KB);
  } else {
    // Update System Memory Details
    mSmBiosType19.Table.StartingAddress = 0xFFFFFFFF;
    mSmBiosType19.Table.EndingAddress   = 0xFFFFFFFF;

    // Update Extended System Memory Details
    mSmBiosType19.Table.ExtendedStartingAddress = MemoryBase;
    mSmBiosType19.Table.ExtendedEndingAddress   = MemoryEnd;
  }
}

UINT64
GetMemorySize ()
{
  EFI_STATUS             Status           = EFI_OUT_OF_RESOURCES;
  EFI_MEMORY_DESCRIPTOR *EfiMemoryMap     = NULL;
  UINTN                  EfiMemoryMapSize = 0;
  UINTN                  DescriptorSize   = 0;
  UINT64                 MemorySize       = 0;

  // Get EFI Memory Map Size
  Status = gBS->GetMemoryMap (&EfiMemoryMapSize, EfiMemoryMap, NULL, NULL, NULL);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((EFI_D_ERROR, "Failed to get EFI Memory Map Size! Status = %r\n", Status));
    goto cleanup;
  }

  // Add 1 KiB
  EfiMemoryMapSize += SIZE_1KB;

  // Allocate Memory
  EfiMemoryMap = AllocateZeroPool (EfiMemoryMapSize);
  if (EfiMemoryMap == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for EFI Memory Map!\n"));
    goto cleanup;
  }

  // Get EFI Memory Map
  Status = gBS->GetMemoryMap (&EfiMemoryMapSize, EfiMemoryMap, NULL, &DescriptorSize, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get EFI Memory Map! Status = %r\n", Status));
    goto cleanup;
  }

  // Go thru each Memory Descriptor
  for (UINT16 i = 0; i < MEMORY_DESCRIPTOR_LIST_SIZE (EfiMemoryMapSize, DescriptorSize); i++) {
    // Get new Memory Descriptor
    EFI_MEMORY_DESCRIPTOR *Descriptor = GET_MEMORY_DESCRIPTOR (EfiMemoryMap, i, DescriptorSize);

    // Add Memory Size
    MemorySize += EFI_PAGES_TO_SIZE (Descriptor->NumberOfPages);
  }

  // Round Memory Size
  MemorySize = ALIGN_VALUE (MemorySize, SIZE_1GB);

cleanup:
  // Free Buffer
  if (EfiMemoryMap != NULL) {
    FreePool (EfiMemoryMap);
  }

  return MemorySize;
}

VOID
UpdateSmBiosTables ()
{
  // Get System Memory Details
  EFI_PHYSICAL_ADDRESS MemoryBase = FixedPcdGet64 (PcdSystemMemoryBase);
  UINT64               MemorySize = GetMemorySize ();

  // Update SMBIOS Tables
  UpdateSmBiosType0  ();
  UpdateSmBiosType1  ();
  UpdateSmBiosType2  ();
  UpdateSmBiosType3  ();
  UpdateSmBiosType4  ();
  UpdateSmBiosType7  ();
  UpdateSmBiosType16 (MemorySize);
  UpdateSmBiosType17 (MemorySize);
  UpdateSmBiosType19 (MemoryBase, (EFI_PHYSICAL_ADDRESS)(MemoryBase + MemorySize - 1));
}

VOID
RegisterCacheTables (
  OUT EFI_SMBIOS_HANDLE *L1dHandle,
  OUT EFI_SMBIOS_HANDLE *L2Handle,
  OUT EFI_SMBIOS_HANDLE *L3Handle)
{
  // Set Default Values
  *L1dHandle = 0xFFFF;
  *L2Handle  = 0xFFFF;
  *L3Handle  = 0xFFFF;

  // Register L1I TYPE7 Table
  if (mSmBiosType7_L1I.Table.InstalledSize.Size != 0) {
    RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType7_L1I.Table, mSmBiosType7_L1I.Strings, NULL);
  }

  // Register L1D TYPE7 Table
  if (mSmBiosType7_L1D.Table.InstalledSize.Size != 0) {
    RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType7_L1D.Table, mSmBiosType7_L1D.Strings, L1dHandle);
  }

  // Register L2 TYPE7 Table
  if (mSmBiosType7_L2.Table.InstalledSize.Size != 0) {
    RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType7_L2.Table, mSmBiosType7_L2.Strings, L2Handle);
  }

  // Register L3 TYPE7 Table
  if (mSmBiosType7_L3.Table.InstalledSize.Size != 0) {
    RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType7_L3.Table, mSmBiosType7_L3.Strings, L3Handle);
  }
}

VOID
RegisterSmBiosTables ()
{
  EFI_SMBIOS_HANDLE MemoryArrayHandle;
  EFI_SMBIOS_HANDLE L1dHandle;
  EFI_SMBIOS_HANDLE L2Handle;
  EFI_SMBIOS_HANDLE L3Handle;

  // Register Information Tables
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType0.Table, mSmBiosType0.Strings, NULL);
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType1.Table, mSmBiosType1.Strings, NULL);
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType2.Table, mSmBiosType2.Strings, NULL);
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType3.Table, mSmBiosType3.Strings, NULL);

  // Register Cache Handles
  RegisterCacheTables (&L1dHandle, &L2Handle, &L3Handle);

  // Update Processor Cache Handles
  mSmBiosType4.Table.L1CacheHandle = L1dHandle;
  mSmBiosType4.Table.L2CacheHandle = L2Handle;
  mSmBiosType4.Table.L3CacheHandle = L3Handle;

  // Register TYPE4 & TYPE16 Table
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType4.Table,  mSmBiosType4.Strings,  NULL);
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType16.Table, mSmBiosType16.Strings, &MemoryArrayHandle);

  // Update Memory Array Handle
  mSmBiosType17.Table.MemoryArrayHandle = MemoryArrayHandle;
  mSmBiosType19.Table.MemoryArrayHandle = MemoryArrayHandle;

  // Register Memory Tables
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType17.Table, mSmBiosType17.Strings, NULL);
  RegisterTable ((EFI_SMBIOS_TABLE_HEADER *)&mSmBiosType19.Table, mSmBiosType19.Strings, NULL);
}

VOID
SmBiosType0CleanUp ()
{
  // Get Firmware Strings
  CHAR8 *FirmwareVendor    = mSmBiosType0.Strings[0];
  CHAR8 *FirmwareVersion   = mSmBiosType0.Strings[1];
  CHAR8 *FirmwareBuildDate = mSmBiosType0.Strings[2];

  // Free Firmware Vendor Buffer
  if (FirmwareVendor != NULL && AsciiStrCmp (FirmwareVendor, "Not Specified") != 0) {
    FreePool (FirmwareVendor);
  }

  // Free Firmware Version Buffer
  if (FirmwareVersion != NULL && AsciiStrCmp (FirmwareVersion, "Not Specified") != 0) {
    FreePool (FirmwareVersion);
  }

  // Free Firmware Build Date Buffer
  if (FirmwareBuildDate != NULL && AsciiStrCmp (FirmwareBuildDate, "Not Specified") != 0) {
    FreePool (FirmwareBuildDate);
  }
}

VOID
SmBiosCleanUp (IN OUT EFI_SMBIOS_TABLES *SmBiosTables)
{
  // Do TYPE0 Clean Up
  SmBiosType0CleanUp ();

  // Do Platform Specific Clean Up
  PlatformSmBiosCleanUp (SmBiosTables);
}

EFI_STATUS
EFIAPI
SmBiosTableDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate SMBIOS Protocol
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID *)&mSmBiosProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SMBIOS Protocol!\n"));
    return Status;
  }

  // Set SMBIOS Tables
  EFI_SMBIOS_TABLES SmBiosTables = {
    .Type0     = &mSmBiosType0,
    .Type1     = &mSmBiosType1,
    .Type2     = &mSmBiosType2,
    .Type3     = &mSmBiosType3,
    .Type4     = &mSmBiosType4,
    .Type7_L1I = &mSmBiosType7_L1I,
    .Type7_L1D = &mSmBiosType7_L1D,
    .Type7_L2  = &mSmBiosType7_L2,
    .Type7_L3  = &mSmBiosType7_L3,
    .Type16    = &mSmBiosType16,
    .Type17    = &mSmBiosType17,
    .Type19    = &mSmBiosType19
  };

  // Update SMBIOS Tables
  UpdateSmBiosTables ();

  // Apply Platform Specific Updates
  PlatformUpdateSmBiosTables (&SmBiosTables);

  // Register SMBIOS Tables
  RegisterSmBiosTables ();

  // Do Clean Up
  SmBiosCleanUp (&SmBiosTables);

  return EFI_SUCCESS;
}
