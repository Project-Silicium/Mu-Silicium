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
#include <Library/DynamicRamLib.h>

#include <IndustryStandard/SmBios.h>

#include <Protocol/Smbios.h>

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
  // Update Max & Current Speed
#if SOC_TYPE == 4
  mProcessorInfoType4.MaxSpeed     = 1420;
  mProcessorInfoType4.CurrentSpeed = 1420;
#elif SOC_TYPE == 5 || SOC_TYPE == 6
  mProcessorInfoType4.MaxSpeed     = 2100;
  mProcessorInfoType4.CurrentSpeed = 2100;
#else
  mProcessorInfoType4.MaxSpeed     = 2200;
  mProcessorInfoType4.CurrentSpeed = 2200;
#endif

  // Update Core Count
  mProcessorInfoType4.CoreCount        = FixedPcdGet32 (PcdCoreCount);
  mProcessorInfoType4.EnabledCoreCount = FixedPcdGet32 (PcdCoreCount);
  mProcessorInfoType4.ThreadCount      = FixedPcdGet32 (PcdCoreCount);

  // Update String Table
  mProcessorInfoType4Strings[0] = (CHAR8 *)FixedPcdGetPtr (PcdSmBiosProcessorSocket);
  mProcessorInfoType4Strings[1] = (CHAR8 *)FixedPcdGetPtr (PcdSmBiosProcessorManufacturer);
  mProcessorInfoType4Strings[2] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorModel);
  mProcessorInfoType4Strings[5] = (CHAR8 *)FixedPcdGetPtr (PcdSmbiosProcessorCodename);

  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4, mProcessorInfoType4Strings, NULL);
}

VOID
CacheInfoUpdateSmbiosType7 ()
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  // Update Cache Size
  mCacheInfoType7_L1IC.MaximumCacheSize = 0x30;
  mCacheInfoType7_L1IC.InstalledSize    = 0x30;
  mCacheInfoType7_L1DC.MaximumCacheSize = 0x30;
  mCacheInfoType7_L1DC.InstalledSize    = 0x30;
  mCacheInfoType7_L2C.MaximumCacheSize  = 0x200;
  mCacheInfoType7_L2C.InstalledSize     = 0x200;

  // Update String Table
  mCacheInfoType7_L1ICStrings[0] = "L1 Instruction Cache";
  mCacheInfoType7_L1DCStrings[0] = "L1 Data Cache";
  mCacheInfoType7_L2CStrings[0]  = "L2 Unified Cache";

  // Register SmBios Structures
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC, mCacheInfoType7_L1ICStrings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC, mCacheInfoType7_L1DCStrings, &SmbiosHandle);

  // Append SmBios Handle
  mProcessorInfoType4.L1CacheHandle = (UINT16)SmbiosHandle;

#if SOC_TYPE == 0 || SOC_TYPE == 1 || SOC_TYPE == 2 || SOC_TYPE == 3
  // Register SmBios Structure
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C, mCacheInfoType7_L2CStrings, &SmbiosHandle);

  // Append SmBios Handle
  mProcessorInfoType4.L2CacheHandle = (UINT16)SmbiosHandle;
#endif
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
  // Update DDR Freq
  mMemDevInfoType17.Speed                      = 1600 * 2;
  mMemDevInfoType17.ConfiguredMemoryClockSpeed = 1600;

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
  EFI_STATUS Status;
  UINT64     MemorySize;

  // Get Max RAM Size
  Status = GetMemorySize (&MemorySize);
  if (EFI_ERROR (Status)) {
    MemorySize = FixedPcdGet64 (PcdSystemMemorySize);
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
  PhyMemArrayInfoUpdateSmbiosType16  (MemorySize);
  MemDevInfoUpdateSmbiosType17       (MemorySize);
  MemArrMapInfoUpdateSmbiosType19    (MemorySize);

  return EFI_SUCCESS;
}
