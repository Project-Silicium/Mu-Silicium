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

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>

#include <IndustryStandard/SmBios.h>

#include <Protocol/Smbios.h>

#include "Tables/Type0.h"
#include "Tables/Type1.h"
#include "Tables/Type2.h"
#include "Tables/Type3.h"
#include "Tables/Type4.h"
#include "Tables/Type7.h"
#include "Tables/Type16.h"
#include "Tables/Type17.h"
#include "Tables/Type19.h"

EFI_STATUS
EFIAPI
LogSmbiosData (
  IN  EFI_SMBIOS_TABLE_HEADER  *Template,
  IN  CHAR8                   **StringPack,
  OUT EFI_SMBIOS_HANDLE        *DataSmbiosHandle)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *mSmbiosProtocol;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER *Record;
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
    for (UINTN Index = 0, StringSize = 0; StringPack[Index] != NULL; Index++) {
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
  for (UINTN Index = 0, StringSize = 0; StringPack[Index] != NULL; Index++) {
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
  if (DataSmbiosHandle != NULL) {
    *DataSmbiosHandle = SmbiosHandle;
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
  AsciiSPrintUnicodeFormat (FirmwareVendor,  sizeof (FirmwareVendor),  FixedPcdGetPtr (PcdFirmwareVendor));
  AsciiSPrintUnicodeFormat (FirmwareVersion, sizeof (FirmwareVersion), FixedPcdGetPtr (PcdFirmwareVersionString));

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
  // Update Max & Current Speed
  mProcessorInfoType4.MaxSpeed     = FixedPcdGet32 (PcdSmbiosMaxCpuFreq);
  mProcessorInfoType4.CurrentSpeed = FixedPcdGet32 (PcdSmbiosMaxCpuFreq);

  // Update Core Count
  mProcessorInfoType4.CoreCount        = FixedPcdGet32 (PcdCoreCount);
  mProcessorInfoType4.EnabledCoreCount = FixedPcdGet32 (PcdCoreCount);
  mProcessorInfoType4.ThreadCount      = FixedPcdGet32 (PcdCoreCount);

  // Update String Table
  mProcessorInfoType4Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmBiosProcessorSocket);
  mProcessorInfoType4Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmBiosProcessorManufacturer);
  mProcessorInfoType4Strings[2] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorModel);
  mProcessorInfoType4Strings[5] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorPartNumber);

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4, mProcessorInfoType4Strings, NULL);
}

VOID
CacheInfoUpdateSmbiosType7 ()
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  // Update Cache Size
  mCacheInfoType7_L1IC.MaximumCacheSize.Size = FixedPcdGet32 (PcdSmbiosLevel1InstCacheSize);
  mCacheInfoType7_L1IC.InstalledSize.Size    = FixedPcdGet32 (PcdSmbiosLevel1InstCacheSize);
  mCacheInfoType7_L1DC.MaximumCacheSize.Size = FixedPcdGet32 (PcdSmbiosLevel1DataCacheSize);
  mCacheInfoType7_L1DC.InstalledSize.Size    = FixedPcdGet32 (PcdSmbiosLevel1DataCacheSize);

  // Update String Table
  mCacheInfoType7_L1ICStrings[0] = "L1 Instruction Cache";
  mCacheInfoType7_L1DCStrings[0] = "L1 Data Cache";

  // Register SmBios Structures
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC, mCacheInfoType7_L1ICStrings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC, mCacheInfoType7_L1DCStrings, &SmbiosHandle);

  // Append SmBios Handle
  mProcessorInfoType4.L1CacheHandle = (UINT16)SmbiosHandle;

  if (FixedPcdGet32 (PcdSmbiosLevel2CacheSize) != 0) {
    // Update Cache Size
    mCacheInfoType7_L2C.MaximumCacheSize.Size = FixedPcdGet32 (PcdSmbiosLevel2CacheSize);
    mCacheInfoType7_L2C.InstalledSize.Size    = FixedPcdGet32 (PcdSmbiosLevel2CacheSize);

    // Update String Table
    mCacheInfoType7_L2CStrings[0] = "L2 Unified Cache";

    // Register SmBios Structure
    LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C, mCacheInfoType7_L2CStrings, &SmbiosHandle);

    // Append SmBios Handle
    mProcessorInfoType4.L2CacheHandle = (UINT16)SmbiosHandle;
  }

  if (FixedPcdGet32 (PcdSmbiosLevel3CacheSize) != 0) {
    // Update Cache Size
    mCacheInfoType7_L3C.MaximumCacheSize.Size = FixedPcdGet32 (PcdSmbiosLevel3CacheSize);
    mCacheInfoType7_L3C.InstalledSize.Size    = FixedPcdGet32 (PcdSmbiosLevel3CacheSize);

    // Update String Table
    mCacheInfoType7_L3CStrings[0] = "L3 Unified Cache";

    // Register SmBios Structure
    LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L3C, mCacheInfoType7_L3CStrings, &SmbiosHandle);

    // Append SmBios Handle
    mProcessorInfoType4.L3CacheHandle = (UINT16)SmbiosHandle;
  }
}

VOID
PhyMemArrayInfoUpdateSmbiosType16 (IN UINT64 SystemMemorySize)
{
  EFI_SMBIOS_HANDLE MemArraySmbiosHandle;

  // Update Memory Size
  mPhyMemArrayInfoType16.ExtendedMaximumCapacity = SystemMemorySize;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mPhyMemArrayInfoType16, mPhyMemArrayInfoType16Strings, &MemArraySmbiosHandle);

  // Append SmBios Handle
  mMemDevInfoType17.MemoryArrayHandle = MemArraySmbiosHandle;
}

VOID
MemDevInfoUpdateSmbiosType17 (IN UINT64 SystemMemorySize)
{
  // Set Memory Speed
  mMemDevInfoType17.Speed                      = FixedPcdGet32 (PcdSmbiosMemorySpeed);
  mMemDevInfoType17.ConfiguredMemoryClockSpeed = FixedPcdGet32 (PcdSmbiosMemorySpeed);

  // Set Memory Speed
  mMemDevInfoType17.MemoryType = FixedPcdGet32 (PcdSmbiosMemoryType);

  // Update Memory Size
  mMemDevInfoType17.Size = SystemMemorySize / 0x100000;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemDevInfoType17, mMemDevInfoType17Strings, NULL);
}

VOID
MemArrMapInfoUpdateSmbiosType19 (
  IN EFI_PHYSICAL_ADDRESS SystemMemoryBase,
  IN UINT64               SystemMemorySize)
{
  // Update Memory Start & End Address
  mMemArrMapInfoType19.StartingAddress = SystemMemoryBase / 1024;
  mMemArrMapInfoType19.EndingAddress   = (SystemMemorySize + SystemMemoryBase - 1) / 1024;

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemArrMapInfoType19, mMemArrMapInfoType19Strings, NULL);
}

EFI_STATUS
GetSystemMemorySize (
  IN  EFI_PHYSICAL_ADDRESS  SystemMemoryBase,
  OUT UINT64               *SystemMemorySize)
{
  EFI_STATUS             Status           = EFI_OUT_OF_RESOURCES;
  EFI_MEMORY_DESCRIPTOR *EfiMemoryMap     = NULL;
  UINTN                  EfiMemoryMapSize = 0;
  UINTN                  DescriptorSize   = 0;
  UINT64                 MemorySize       = 0;

  // Get EFI Memory Map Size
  Status = gBS->GetMemoryMap (&EfiMemoryMapSize, EfiMemoryMap, NULL, NULL, NULL);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((EFI_D_ERROR, "Failed to get EFI Memory Map Size!\n"));
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
    DEBUG ((EFI_D_ERROR, "Failed to get EFI Memory Map!\n"));
    goto cleanup;
  }

  // Go thru each Memory Descriptor
  for (UINT16 i = 0; i < EfiMemoryMapSize / DescriptorSize - 1; i++) {
    // Get new Memory Descriptor
    EFI_MEMORY_DESCRIPTOR *Descriptor = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)EfiMemoryMap + (i * DescriptorSize));

    // Skip Non-DDR Memory
    if (Descriptor->PhysicalStart < SystemMemoryBase) {
      continue;
    }

    // Add Memory Size
    MemorySize += EFI_PAGES_TO_SIZE (Descriptor->NumberOfPages);
  }

  // Pass System Memory Size
  *SystemMemorySize = ((MemorySize + (SIZE_1GB - 1)) / SIZE_1GB) * SIZE_1GB;

cleanup:
  // Free Buffer
  if (EfiMemoryMap != NULL) {
    FreePool (EfiMemoryMap);
  }

  return Status;
}

EFI_STATUS
EFIAPI
RegisterSmBiosTables (
  IN EFI_HANDLE        ImageHandle, 
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  UINT64     SystemMemorySize;

  // Get System Memory Base
  EFI_PHYSICAL_ADDRESS SystemMemoryBase = FixedPcdGet64 (PcdSystemMemoryBase);

  // Get System Memory Size
  Status = GetSystemMemorySize (SystemMemoryBase, &SystemMemorySize);
  ASSERT_EFI_ERROR (Status);

  // Update SmBios Structures
  BIOSInfoUpdateSmbiosType0         ();
  SysInfoUpdateSmbiosType1          ();
  BoardInfoUpdateSmbiosType2        ();
  EnclosureInfoUpdateSmbiosType3    ();
  ProcessorInfoUpdateSmbiosType4    ();
  CacheInfoUpdateSmbiosType7        ();
  PhyMemArrayInfoUpdateSmbiosType16 (SystemMemorySize);
  MemDevInfoUpdateSmbiosType17      (SystemMemorySize);
  MemArrMapInfoUpdateSmbiosType19   (SystemMemoryBase, SystemMemorySize);

  return EFI_SUCCESS;
}
