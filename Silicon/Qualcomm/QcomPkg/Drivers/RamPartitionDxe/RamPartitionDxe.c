#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DeviceMemoryMapLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/IoLib.h>
#include <Pi/PiDxeCis.h>

#include <Protocol/EFISmem.h>

#include "ExtendedMemoryMap.h"

#define SMEM_USABLE_RAM_PARTITION_TABLE 402

#define RAM_NUM_PART_ENTRIES 32

#define RAM_PART_MAGIC1 0x9DA5E0A8
#define RAM_PART_MAGIC2 0xAF9EC4E2

#define RAM_PART_SYS_MEMORY 1
#define RAM_PART_SDRAM 14

#define GENERIC_RAM_BASE FixedPcdGet64(PcdSystemMemoryBase)
#define RAM_PARTITION_BASE FixedPcdGet64(PcdRamPartitionBase)

typedef struct _RamPartitionEntry {
  char Name[16]; 
  UINT64 Base;
  UINT64 Length;
  UINT32 Attribute;
  UINT32 Category;
  UINT32 Domain;
  UINT32 Type;
  UINT32 NumPartitions;
  UINT32 HwInfo;
  UINT8 HighestBankBit;
  UINT8 reserved0;
  UINT8 reserved1;
  UINT8 reserved2;
  UINT32 MinPasrSize;
  UINT64 AvailableLength;
} RamPartitionEntry;

typedef struct _ram_partition_table
{
  UINT32 Magic1;
  UINT32 Magic2;
  UINT32 Version;
  UINT32 reserved1;
  UINT32 NumPartitions;
  UINT32 reserved2;
  RamPartitionEntry RamPartitionEntry[RAM_NUM_PART_ENTRIES];
} ram_partition_table;

EFI_STATUS
GetRamPartitionVersion(IN VOID *pRamPartitionTable, IN OUT UINT32 *Version) {
  ram_partition_table* ramTable;

  ramTable = (ram_partition_table*)pRamPartitionTable;

  if(ramTable->Magic1 == RAM_PART_MAGIC1 && ramTable->Magic2 == RAM_PART_MAGIC2) {
    *Version = ramTable->Version;
    return EFI_SUCCESS;
  } 
  return EFI_NOT_FOUND;
}

STATIC
EFI_STATUS
GetRamPartitions (
  OUT ram_partition_table **RamPartitionTable,
  OUT UINT32 *NumPartitions,
  OUT UINT32 *Version
) {
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_SMEM_PROTOCOL *pEfiSmemProtocol = NULL;

  Status = gBS->LocateProtocol(
      &gEfiSMEMProtocolGuid, NULL, (VOID **)&pEfiSmemProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Smem Protocol! Status = %r\n", Status));
    return Status;
  }
  Status = pEfiSmemProtocol->GetFunc(SMEM_USABLE_RAM_PARTITION_TABLE, NumPartitions, (VOID **)RamPartitionTable);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM table! Status = %r\n", Status));
    return Status;
  }

  Status = GetRamPartitionVersion(*RamPartitionTable, Version);
  if(EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM partition version! Status = %r\n", Status));
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
AddRamPartition(
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64 Length,
  IN UINT64 ArmAttributes,
  IN EFI_MEMORY_TYPE EfiMemoryType
) {
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_GCD_MEMORY_TYPE EfiGcdMemoryType = EfiGcdMemoryTypeReserved;

  switch(EfiMemoryType){
    case EfiReservedMemoryType:
        EfiGcdMemoryType = EfiGcdMemoryTypeReserved;
        DEBUG((EFI_D_ERROR, "Default Type: EfiGcdMemoryTypeReserved\n"));
        break;
    case EfiConventionalMemory:
        EfiGcdMemoryType = EfiGcdMemoryTypeSystemMemory;
        DEBUG((EFI_D_ERROR, "Default Type: EfiGcdMemoryTypeSystemMemory\n"));
        break;
    default:
        EfiGcdMemoryType = EfiGcdMemoryTypeReserved;
        DEBUG((EFI_D_ERROR, "Choose Type: EfiGcdMemoryTypeReserved\n"));
        break;
  }

  if(Length == 0) return EFI_INVALID_PARAMETER;

  DEBUG((EFI_D_ERROR, "Adding Memory Space: 0x%08llx 0x%08llx\n", Base, Length));
  Status = gDS->AddMemorySpace(EfiGcdMemoryType, Base, Length, 0xF);
  if(EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "AddMemorySpace Failed ! %r\n", Status));
    return Status;
  }

  Status = ArmSetMemoryAttributes(Base, Length, ArmAttributes);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "ArmSetMemoryAttributes Failed ! %r\n", Status));
    return Status;
  }

  Status = ArmClearMemoryRegionNoExec(Base, Length);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "ArmClearMemoryRegionNoExec Failed ! %r\n", Status));
    return Status;
  }

  Status = ArmClearMemoryRegionReadOnly(Base, Length);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "ArmClearMemoryRegionReadOnly Failed ! %r\n", Status));
    return Status;
  }

  Status = gDS->SetMemorySpaceAttributes(Base, Length, EFI_MEMORY_WB);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "ArmClearMemoryRegionReadOnly Failed ! %r\n", Status));
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
RamPartitionDxeInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN Index = 1;
  UINT32 NumPartitions, PartitionVersion = 0;
  ram_partition_table *RamPartitionTable = NULL;
  PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptor = gExtendedMemoryDescriptor;

  if (FixedPcdGetBool(PcdEnableRamPartition) == TRUE) {
    Status = GetRamPartitions(&RamPartitionTable, &NumPartitions, &PartitionVersion);
    DEBUG ((EFI_D_ERROR, "RAM Partition version: %d\n", PartitionVersion));
    if(PartitionVersion == 1) { 
      DEBUG ((EFI_D_ERROR, "RAM Partition version 1 is not supported\n"));
      goto exit;
    }

    DEBUG ((EFI_D_ERROR, "RAM Partitions\n"));
    for(int i = 0; i < NumPartitions; i++) {
      if(RamPartitionTable->RamPartitionEntry[i].Type != RAM_PART_SYS_MEMORY || RamPartitionTable->RamPartitionEntry[i].Category != RAM_PART_SDRAM)
        continue;
      
      DEBUG((EFI_D_ERROR, "RAM Entry %d: Base:  0x%016lx ", i, RamPartitionTable->RamPartitionEntry[i].Base));
      DEBUG((EFI_D_ERROR, "AvailableLength: 0x%016lx\n", RamPartitionTable->RamPartitionEntry[i].AvailableLength));

      if(RamPartitionTable->RamPartitionEntry[i].Base == GENERIC_RAM_BASE) {
        MemoryDescriptor[0].Address = RAM_PARTITION_BASE;
        MemoryDescriptor[0].Length = RamPartitionTable->RamPartitionEntry[i].AvailableLength + GENERIC_RAM_BASE - RAM_PARTITION_BASE;
        AddRamPartition(MemoryDescriptor[0].Address, MemoryDescriptor[0].Length, MemoryDescriptor[0].ArmAttributes, MemoryDescriptor[0].MemoryType);
        continue;
      }

      MemoryDescriptor[Index].Address = RamPartitionTable->RamPartitionEntry[i].Base;
      MemoryDescriptor[Index].Length = RamPartitionTable->RamPartitionEntry[i].AvailableLength;
      Index++;
    }

    Index = 1;
    while(MemoryDescriptor[Index].Length != 0) {
      ASSERT(Index < MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT);
      AddRamPartition(MemoryDescriptor[Index].Address, MemoryDescriptor[Index].Length, MemoryDescriptor[Index].ArmAttributes, MemoryDescriptor[Index].MemoryType);
      Index++;
    }
  }

exit:
  return Status;
}