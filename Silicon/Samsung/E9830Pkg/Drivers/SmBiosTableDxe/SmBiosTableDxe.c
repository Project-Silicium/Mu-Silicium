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

#include <IndustryStandard/SmBios.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#if DEVICE_PRESERVES_FDT == 1 && DEVICE_SUPPORTS_EXYNOS_DYNAMIC_RAM_ALLOCATION == 1
#include <Library/libfdt.h>
#include <Library/MemoryMapHelperLib.h>
#endif

#include <Protocol/Smbios.h>

#include "DataDefinitions.h"

#if DEVICE_PRESERVES_FDT == 1 && DEVICE_SUPPORTS_EXYNOS_DYNAMIC_RAM_ALLOCATION == 1
typedef struct {
  UINT64 StartAddress;
  UINT64 Size;
} MEMORY_NODE;

MEMORY_NODE*
GetMemoryNodes(const void *fdt, UINTN *NodeCount) {
  INT32 Node;
  INT32 AddrCells, SizeCells;
  CONST UINT32 *Reg;
  INT32 Len;
  UINTN Count = 0;
  MEMORY_NODE *Nodes = NULL;
  UINTN CurrentSize = 0;

  Node = fdt_path_offset(fdt, "/");

  if (Node < 0) {
    DEBUG((EFI_D_ERROR, "Error finding root node: %a\n", fdt_strerror(Node)));
    ASSERT(FALSE);
  }

  AddrCells = fdt_address_cells(fdt, Node);
  SizeCells = fdt_size_cells(fdt, Node);
  if (AddrCells < 0 || SizeCells < 0) {
      DEBUG((EFI_D_ERROR, "Error finding address or size cells\n"));
      ASSERT(FALSE);
  }

  fdt_for_each_subnode(Node, fdt, Node) {
    CONST CHAR8 *DeviceType = fdt_getprop(fdt, Node, "device_type", NULL);

    if (DeviceType && AsciiStrCmp(DeviceType, "memory") == 0) {
      Reg = fdt_getprop(fdt, Node, "reg", &Len);

      if (!Reg) {
        DEBUG((EFI_D_ERROR, "Error reading 'reg' property: %a\n", fdt_strerror(Len)));
        ASSERT(FALSE);
      }

      INT32 RegSize = (AddrCells + SizeCells) * sizeof(UINT32);
      INT32 NumRegs = Len / RegSize;
      UINTN NewSize = (Count + NumRegs) * sizeof(MEMORY_NODE);

      Nodes = ReallocatePool(CurrentSize, NewSize, Nodes);
      CurrentSize = NewSize;

      for (INT32 i = 0; i < NumRegs; i++) {
        UINT64 Start = 0, Size = 0;

        for (INT32 j = 0; j < AddrCells; j++) {
          Start = (Start << 32) | fdt32_to_cpu(Reg[i * (AddrCells + SizeCells) + j]);
        }

        for (INT32 j = 0; j < SizeCells; j++) {
          Size = (Size << 32) | fdt32_to_cpu(Reg[i * (AddrCells + SizeCells) + AddrCells + j]);
        }

        Nodes[Count].StartAddress = Start;
        Nodes[Count].Size = Size;
        Count++;
      }
    }
  }

  *NodeCount = Count;
  return Nodes;
}
#endif

EFI_STATUS
EFIAPI
LogSmbiosData (
  IN  EFI_SMBIOS_TABLE_HEADER *Template,
  IN  CHAR8                  **StringPack,
  OUT EFI_SMBIOS_HANDLE       *DataSmbiosHande)
{
  EFI_STATUS               Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER *Record;
  UINTN                    StringSize;
  UINTN                    Size;
  CHAR8                   *Str;

  // Locate SmBios Protocol
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SmBios Protocol! Status = %r\n", Status));
    return Status;
  }

  Size = Template->Length;

  if (StringPack == NULL) {
    Size += 2;
  } else {
    for (UINTN Index = 0; StringPack[Index] != NULL; Index++) {
      StringSize = AsciiStrSize(StringPack[Index]);
      Size      += StringSize;
    }

    if (StringPack[0] == NULL) {
      Size += 1;
    }

    Size += 1;
  }

  Record = (EFI_SMBIOS_TABLE_HEADER *)AllocateZeroPool (Size);
  if (Record == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem(Record, Template, Template->Length);

  Str = ((CHAR8 *)Record) + Record->Length;

  for (UINTN Index = 0; StringPack[Index] != NULL; Index++) {
    StringSize = AsciiStrSize(StringPack[Index]);
    CopyMem(Str, StringPack[Index], StringSize);
    Str += StringSize;
  }

  *Str         = 0;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status       = Smbios->Add(Smbios, gImageHandle, &SmbiosHandle, Record);
  ASSERT_EFI_ERROR(Status);

  if (DataSmbiosHande != NULL) {
    *DataSmbiosHande = SmbiosHandle;
  }
  
  FreePool(Record);

  return EFI_SUCCESS;
}

VOID
BIOSInfoUpdateSmbiosType0 ()
{
  CHAR8 FirmwareVendor[100] = "";

  // Give Space to FirmwareVendor Variable
  ZeroMem (FirmwareVendor, 100);

  // Append Device Maintainer
  AsciiSPrint (FirmwareVendor, sizeof(FirmwareVendor), "Project Silicium & %a", FixedPcdGetPtr(PcdDeviceMaintainer));

  // Update String Table
  mBIOSInfoType0Strings[0] = FirmwareVendor;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBIOSInfoType0, mBIOSInfoType0Strings, NULL);
}

VOID
SysInfoUpdateSmbiosType1 ()
{
  // Update String Table
  mSysInfoType1Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemVendor);
  mSysInfoType1Strings[1] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemModel);
  mSysInfoType1Strings[2] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemRetailModel);
  mSysInfoType1Strings[4] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemRetailSku);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mSysInfoType1, mSysInfoType1Strings, NULL);
}

VOID
BoardInfoUpdateSmbiosType2 ()
{
  // Update String Table
  mBoardInfoType2Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemVendor);
  mBoardInfoType2Strings[1] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosBoardModel);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mBoardInfoType2, mBoardInfoType2Strings, NULL);
}

VOID
EnclosureInfoUpdateSmbiosType3 ()
{
  // Update String Table
  mEnclosureInfoType3Strings[0] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosSystemVendor);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mEnclosureInfoType3, mEnclosureInfoType3Strings, NULL);
}

VOID
ProcessorInfoUpdateSmbiosType4 ()
{
  // Update String Table
  mProcessorInfoType4Strings[2] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosProcessorModel);
  mProcessorInfoType4Strings[3] = (CHAR8 *)FixedPcdGetPtr(PcdSmbiosProcessorRetailModel);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4_m5,  mProcessorInfoType4Strings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4_a76, mProcessorInfoType4Strings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mProcessorInfoType4_a55, mProcessorInfoType4Strings, NULL);
}

VOID
CacheInfoUpdateSmbiosType7 ()
{
  EFI_SMBIOS_HANDLE SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC_m5,  mCacheInfoType7_L1ICStrings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC_a76, mCacheInfoType7_L1ICStrings, NULL);
  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1IC_a55, mCacheInfoType7_L1ICStrings, NULL);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC_m5, mCacheInfoType7_L1DCStrings, &SmbiosHandle);
  
  mProcessorInfoType4_m5.L1CacheHandle = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC_a76, mCacheInfoType7_L1DCStrings, &SmbiosHandle);

  mProcessorInfoType4_a76.L1CacheHandle = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L1DC_a55, mCacheInfoType7_L1DCStrings, &SmbiosHandle);

  mProcessorInfoType4_a55.L1CacheHandle = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C_m5, mCacheInfoType7_L2CStrings, &SmbiosHandle);

  mProcessorInfoType4_m5.L2CacheHandle = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C_a76, mCacheInfoType7_L2CStrings, &SmbiosHandle);

  mProcessorInfoType4_a76.L2CacheHandle = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L2C_a55, mCacheInfoType7_L2CStrings, &SmbiosHandle);

  mProcessorInfoType4_a55.L2CacheHandle = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L3C_m5, mCacheInfoType7_L3CStrings, &SmbiosHandle);

  mProcessorInfoType4_m5.L3CacheHandle  = (UINT16)SmbiosHandle;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mCacheInfoType7_L3C_a76, mCacheInfoType7_L3CStrings, &SmbiosHandle);

  mProcessorInfoType4_a76.L3CacheHandle = (UINT16)SmbiosHandle;
}

#if DEVICE_PRESERVES_FDT == 1 && DEVICE_SUPPORTS_EXYNOS_DYNAMIC_RAM_ALLOCATION == 1
VOID
PhyMemArrayInfoUpdateSmbiosType16 (UINT64 SystemMemorySize)
{
  EFI_SMBIOS_HANDLE MemArraySmbiosHande;

  mPhyMemArrayInfoType16.ExtendedMaximumCapacity = SystemMemorySize;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mPhyMemArrayInfoType16, mPhyMemArrayInfoType16Strings, &MemArraySmbiosHande);

  // Update the Memory Device Information
  mMemDevInfoType17.MemoryArrayHandle = MemArraySmbiosHande;
}

VOID
MemDevInfoUpdateSmbiosType17 (UINT64 SystemMemorySize)
{
  mMemDevInfoType17.Size = SystemMemorySize / 0x100000;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemDevInfoType17, mMemDevInfoType17Strings, NULL);
}

VOID
MemArrMapInfoUpdateSmbiosType19 (UINT64 SystemMemorySize)
{
  mMemArrMapInfoType19.StartingAddress = FixedPcdGet64(PcdSystemMemoryBase) / 1024;
  mMemArrMapInfoType19.EndingAddress   = (SystemMemorySize + FixedPcdGet64(PcdSystemMemoryBase) - 1) / 1024;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemArrMapInfoType19, mMemArrMapInfoType19Strings, NULL);
}
#else
VOID
PhyMemArrayInfoUpdateSmbiosType16 ()
{
  EFI_SMBIOS_HANDLE MemArraySmbiosHande;

  mPhyMemArrayInfoType16.ExtendedMaximumCapacity = FixedPcdGet64(PcdSystemMemorySize);

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mPhyMemArrayInfoType16, mPhyMemArrayInfoType16Strings, &MemArraySmbiosHande);

  // Update the Memory Device Information
  mMemDevInfoType17.MemoryArrayHandle = MemArraySmbiosHande;
}

VOID
MemDevInfoUpdateSmbiosType17 ()
{
  mMemDevInfoType17.Size = FixedPcdGet64(PcdSystemMemorySize) / 0x100000;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemDevInfoType17, mMemDevInfoType17Strings, NULL);
}

VOID
MemArrMapInfoUpdateSmbiosType19 ()
{
  mMemArrMapInfoType19.StartingAddress = FixedPcdGet64(PcdSystemMemoryBase) / 1024;
  mMemArrMapInfoType19.EndingAddress   = (FixedPcdGet64(PcdSystemMemorySize) + FixedPcdGet64(PcdSystemMemoryBase) - 1) / 1024;

  LogSmbiosData ((EFI_SMBIOS_TABLE_HEADER *)&mMemArrMapInfoType19, mMemArrMapInfoType19Strings, NULL);
}
#endif

EFI_STATUS
EFIAPI
InitializeSmBiosTable (
  IN EFI_HANDLE        ImageHandle, 
  IN EFI_SYSTEM_TABLE *SystemTable)
{
#if DEVICE_PRESERVES_FDT == 1 && DEVICE_SUPPORTS_EXYNOS_DYNAMIC_RAM_ALLOCATION == 1
  EFI_STATUS 			  Status = EFI_SUCCESS;
  UINT64             		  SystemMemorySize = 0;
  UINT64 	     		  DesignMemorySize = 0;
  UINTN 	     		  NodeCount;
  MEMORY_NODE 	     		  *Nodes;
  ARM_MEMORY_REGION_DESCRIPTOR_EX FdtPointer;
#endif

  // Update SmBios Data Definitions
  BIOSInfoUpdateSmbiosType0         ();
  SysInfoUpdateSmbiosType1          ();
  BoardInfoUpdateSmbiosType2        ();
  EnclosureInfoUpdateSmbiosType3    ();
  ProcessorInfoUpdateSmbiosType4    ();
  CacheInfoUpdateSmbiosType7        ();

#if DEVICE_PRESERVES_FDT == 1 && DEVICE_SUPPORTS_EXYNOS_DYNAMIC_RAM_ALLOCATION == 1
  Status = LocateMemoryMapAreaByName ("FDT", &FdtPointer);
  if (!EFI_ERROR (Status)) {
    CONST VOID *FDT = (CONST VOID*)(uintptr_t)MmioRead32(FdtPointer.Address);
    Nodes = GetMemoryNodes(FDT, &NodeCount);

    for (UINTN i = 0; i < NodeCount; i++) {
      SystemMemorySize += Nodes[i].Size;
    }

    while (SystemMemorySize >= DesignMemorySize) {
      DesignMemorySize += 0x40000000; // 1GiB
    }

    SystemMemorySize = DesignMemorySize;
  }

  PhyMemArrayInfoUpdateSmbiosType16 (SystemMemorySize);
  MemDevInfoUpdateSmbiosType17      (SystemMemorySize);
  MemArrMapInfoUpdateSmbiosType19   (SystemMemorySize);
#else
  PhyMemArrayInfoUpdateSmbiosType16 ();
  MemDevInfoUpdateSmbiosType17      ();
  MemArrMapInfoUpdateSmbiosType19   ();
#endif

  return EFI_SUCCESS;
}
