#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DeviceMemoryMapLib.h>
#include <Library/ArmMmuLib.h>

#include <Protocol/EFISmem.h>

#include "ExtendedMemoryMap.h"

//
// SMEM RAM Partition Defines
//
#define SMEM_USABLE_RAM_PARTITION_TABLE 402

#define RAM_NUM_PART_ENTRIES            32

#define RAM_PART_MAGIC1                 0x9DA5E0A8
#define RAM_PART_MAGIC2                 0xAF9EC4E2

#define RAM_PART_SYS_MEMORY             1
#define RAM_PART_SDRAM                  14

#define GENERIC_RAM_BASE                FixedPcdGet64(PcdSystemMemoryBase)
#define RAM_PARTITION_BASE              FixedPcdGet64(PcdRamPartitionBase)

//
// RAM Partition Entry Structure
//
struct RAMPartitionEntry {
  char   Name[16]; 
  UINT64 Base;
  UINT64 Length;
  UINT32 Attribute;
  UINT32 Category;
  UINT32 Domain;
  UINT32 Type;
  UINT32 NumPartitions;
  UINT32 HWInfo;
  UINT8  HighestBankBit;
  UINT8  Reserved0;
  UINT8  Reserved1;
  UINT8  Reserved2;
  UINT32 MinPasrSize;
  UINT64 AvailableLength;
};

//
// RAM Partition Table Structure
//
struct RAMPartitionTable{
  UINT32                   Magic1;
  UINT32                   Magic2;
  UINT32                   Reserved1;
  UINT32                   Reserved2;
  UINT32                   Version;
  UINT32                   NumPartitions;
  struct RAMPartitionEntry RAMPartitionEntry[RAM_NUM_PART_ENTRIES];
};

EFI_STATUS
GetRamPartitionVersion (
  IN  struct RAMPartitionTable *RAMPartitionTable,
  OUT UINT32                   *Version)
{
  // Check if RAM Partition Magics are Valid
  if (RAMPartitionTable->Magic1 == RAM_PART_MAGIC1 && RAMPartitionTable->Magic2 == RAM_PART_MAGIC2) {
    *Version = RAMPartitionTable->Version;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

STATIC
EFI_STATUS
GetRamPartitions (
  OUT struct RAMPartitionTable **RAMPartitionTable,
  OUT UINT32                    *NumPartitions,
  OUT UINT32                    *Version)
{
  EFI_STATUS         Status        = EFI_SUCCESS;
  EFI_SMEM_PROTOCOL *mSmemProtocol = NULL;

  // Locate SMEM Protocol
  Status = gBS->LocateProtocol (&gEfiSMEMProtocolGuid, NULL, (VOID **)&mSmemProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SMEM Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Get the RAM Partition Table
  Status = mSmemProtocol->GetFunc (SMEM_USABLE_RAM_PARTITION_TABLE, NumPartitions, (VOID **)RAMPartitionTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partition Table! Status = %r\n", Status));
    goto exit;
  }

  // Get RAM Partition Version
  Status = GetRamPartitionVersion (*RAMPartitionTable, Version);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partition Version! Status = %r\n", Status));
    goto exit;
  }

exit:
  return Status;
}

EFI_STATUS
EFIAPI
AddRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length,
  IN UINT64               ArmAttributes,
  IN EFI_MEMORY_TYPE      EfiMemoryType)
{
  EFI_STATUS          Status           = EFI_SUCCESS;
  EFI_GCD_MEMORY_TYPE EfiGcdMemoryType = EfiGcdMemoryTypeReserved;

  switch (EfiMemoryType) {
    case EfiReservedMemoryType:
        EfiGcdMemoryType = EfiGcdMemoryTypeReserved;
        break;

    case EfiConventionalMemory:
        EfiGcdMemoryType = EfiGcdMemoryTypeSystemMemory;
        break;

    default:
        EfiGcdMemoryType = EfiGcdMemoryTypeReserved;
        break;
  }

  if (Length == 0) { return EFI_INVALID_PARAMETER; }

  // Add New Memory Space
  Status = gDS->AddMemorySpace (EfiGcdMemoryType, Base, Length, 0xF);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Add Memory Space! Status = %r\n", Status));
    goto exit;
  }

  // Set New Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, ArmAttributes);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Attributes! Status = %r\n", Status));
    goto exit;
  }

  // Clear NoExec and ReadOnly Memory Regions
  Status  = ArmClearMemoryRegionNoExec   (Base, Length);
  Status |= ArmClearMemoryRegionReadOnly (Base, Length);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Clear Memory Region! Status = %r\n", Status));
    goto exit;
  }

  // Set Memory Space Attributes
  Status = gDS->SetMemorySpaceAttributes (Base, Length, EFI_MEMORY_WB);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Space Attributes! Status = %r\n", Status));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "Successfully Added RAM Parition: 0x%08llx 0x%08llx\n", Base, Length));

exit:
  return Status;
}

EFI_STATUS
EFIAPI
AddRAMPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                       Status            = EFI_SUCCESS;
  UINTN                            Index             = 1;
  UINT32                           NumPartitions     = 0;
  UINT32                           PartitionVersion  = 0;
  PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptor  = gExtendedMemoryDescriptor;

  // Struct the RAMPartitionTable
  struct RAMPartitionTable        *RAMPartitionTable = NULL;

  // Get RAM Partition Infos
  Status = GetRamPartitions (&RAMPartitionTable, &NumPartitions, &PartitionVersion);
  if (EFI_ERROR (Status)) { goto exit; }

  // Print RAM Partition Version
  if (PartitionVersion == 1) { 
    DEBUG ((EFI_D_ERROR, "RAM Partition Version 1 is not Supported!\n"));
    Status = EFI_UNSUPPORTED;
    goto exit;
  } else {
    DEBUG ((EFI_D_WARN, "RAM Partition version: %d\n", PartitionVersion));
  }

  for (INT32 i = 0; i < NumPartitions; i++) {
    if (RAMPartitionTable->RAMPartitionEntry[i].Type != RAM_PART_SYS_MEMORY || RAMPartitionTable->RAMPartitionEntry[i].Category != RAM_PART_SDRAM) { continue; }

    DEBUG ((EFI_D_WARN, "\n"));
    DEBUG ((EFI_D_WARN, "RAM Partition Entry %d: 0x%08llx\n", i, RAMPartitionTable->RAMPartitionEntry[i].Base));
    DEBUG ((EFI_D_WARN, "AvailableLength:       0x%08llx\n",     RAMPartitionTable->RAMPartitionEntry[i].AvailableLength));
    DEBUG ((EFI_D_WARN, "\n"));

    if (RAMPartitionTable->RAMPartitionEntry[i].Base == GENERIC_RAM_BASE) {
      MemoryDescriptor[0].Address = RAM_PARTITION_BASE;
      MemoryDescriptor[0].Length  = RAMPartitionTable->RAMPartitionEntry[i].AvailableLength + GENERIC_RAM_BASE - RAM_PARTITION_BASE;

      // Add New RAM Partition
      AddRamPartition (MemoryDescriptor[0].Address, MemoryDescriptor[0].Length, MemoryDescriptor[0].ArmAttributes, MemoryDescriptor[0].MemoryType);

      continue;
    }

    MemoryDescriptor[Index].Address = RAMPartitionTable->RAMPartitionEntry[i].Base;
    MemoryDescriptor[Index].Length  = RAMPartitionTable->RAMPartitionEntry[i].AvailableLength;
    Index++;
  }

  Index = 1;

  while (MemoryDescriptor[Index].Length != 0) {
    ASSERT (Index < MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT);

    // Add New RAM Partition
    AddRamPartition (MemoryDescriptor[Index].Address, MemoryDescriptor[Index].Length, MemoryDescriptor[Index].ArmAttributes, MemoryDescriptor[Index].MemoryType);

    Index++;
  }

exit:
  ASSERT_EFI_ERROR (Status);

  return Status;
}