#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Library/SortLib.h>

#include <Protocol/EFISmem.h>

#include "ExtendedMemoryMap.h"

BOOLEAN AnyRamPartitionAdded = FALSE;

VOID
AddRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length,
  IN UINT64               ArmAttributes,
  IN EFI_MEMORY_TYPE      EfiMemoryType)
{
  EFI_STATUS          Status;
  EFI_GCD_MEMORY_TYPE EfiGcdMemoryType;

  if (Length <= 0) { return; }

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

  // Add New Memory Space
  Status = gDS->AddMemorySpace (EfiGcdMemoryType, Base, Length, 0xF);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Add Memory Space for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  // Set New Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, ArmAttributes, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Attributes for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  // Set Memory Space Attributes
  Status = gDS->SetMemorySpaceAttributes (Base, Length, EFI_MEMORY_WB);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Space Attributes for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  DEBUG ((EFI_D_WARN, "Successfully Added RAM Parition: 0x%08llx 0x%08llx\n", Base, Length));

  AnyRamPartitionAdded = TRUE;
}

INTN
EFIAPI
CompareBaseAddress (
  CONST VOID *P1,
  CONST VOID *P2)
{
  CONST RamPartitionEntry *Part1 = (RamPartitionEntry*)P1;
  CONST RamPartitionEntry *Part2 = (RamPartitionEntry*)P2;

  if (Part1->Base < Part2->Base) {
    return -1;
  } else if (Part1->Base > Part2->Base) {
    return 1;
  } else {
    return 0;
  }
}

EFI_STATUS
EFIAPI
AddRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                       Status              = EFI_SUCCESS;
  PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptor    = gExtendedMemoryDescriptor;
  RamPartitionTable               *RamPartitionTable   = NULL;
  UINTN                            Index               = 1;
  UINT32                           NumPartitions       = 0;
  UINT32                           PartitionVersion    = 0;

  // Get RAM Partition Infos
  Status = GetRamPartitions (&RamPartitionTable, &PartitionVersion);
  ASSERT_EFI_ERROR (Status);

  // Print RAM Partition Version
  if (PartitionVersion == 1) { 
    DEBUG ((EFI_D_ERROR, "RAM Partition Version 1 is not Supported!\n"));
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
  } else {
    DEBUG ((EFI_D_WARN, "RAM Partition Version: %d\n", PartitionVersion));
  }

  // Get Number of RAM Partitions
  NumPartitions = RamPartitionTable->NumPartitions;

  // Sort all RAM Partitions
  PerformQuickSort (RamPartitionTable->RamPartitionEntry, NumPartitions, sizeof(RamPartitionEntry), CompareBaseAddress);

  for (INT32 i = 0; i < NumPartitions; i++) {
    // Check if the RAM Partition is Invalid
    if (RamPartitionTable->RamPartitionEntry[i].Type != RAM_PART_SYS_MEMORY || RamPartitionTable->RamPartitionEntry[i].Category != RAM_PART_SDRAM) { continue; }
    if (RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength <= RAM_PARTITION_BASE) { continue; }

    // Add First RAM Partition
    if (!AnyRamPartitionAdded) {
      MemoryDescriptor[0].Address = RAM_PARTITION_BASE;
      MemoryDescriptor[0].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength - RAM_PARTITION_BASE + RamPartitionTable->RamPartitionEntry[i].Base;

      // Add New RAM Partition
      AddRamPartition (MemoryDescriptor[0].Address, MemoryDescriptor[0].Length, MemoryDescriptor[0].ArmAttributes, MemoryDescriptor[0].MemoryType);
      continue;
    }

    // Refresh Variables
    MemoryDescriptor[Index].Address = RamPartitionTable->RamPartitionEntry[i].Base;
    MemoryDescriptor[Index].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength;
    Index++;
  }

  Index = 1;

  // Add the Rest of RAM Partitions
  while (MemoryDescriptor[Index].Length != 0) {
    ASSERT (Index < MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT);

    // Add New RAM Partition
    AddRamPartition (MemoryDescriptor[Index].Address, MemoryDescriptor[Index].Length, MemoryDescriptor[Index].ArmAttributes, MemoryDescriptor[Index].MemoryType);

    Index++;
  }

  if (!AnyRamPartitionAdded) {
    DEBUG ((EFI_D_ERROR, "No RAM Partitions were Added! Stoping UEFI now.\n"));
    ASSERT_EFI_ERROR (EFI_NOT_READY);
  }

  return EFI_SUCCESS;
}
