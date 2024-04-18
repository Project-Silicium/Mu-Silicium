#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/RamPartitionTableLib.h>

#include <Protocol/EFISmem.h>

#include "ExtendedMemoryMap.h"

EFI_STATUS
EFIAPI
AddRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length,
  IN UINT64               ArmAttributes,
  IN EFI_MEMORY_TYPE      EfiMemoryType)
{
  EFI_STATUS          Status;
  EFI_GCD_MEMORY_TYPE EfiGcdMemoryType;

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
    DEBUG ((EFI_D_WARN, "Trying to add Ram Partition: 0x%08llx 0x%08llx\n", Base, Length));
    goto exit;
  }

  // Set New Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, ArmAttributes, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Attributes! Status = %r\n", Status));
    DEBUG ((EFI_D_WARN, "Trying to add Ram Partition: 0x%08llx 0x%08llx\n", Base, Length));
    goto exit;
  }

  // Set Memory Space Attributes
  Status = gDS->SetMemorySpaceAttributes (Base, Length, EFI_MEMORY_WB);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Space Attributes! Status = %r\n", Status));
    DEBUG ((EFI_D_WARN, "Trying to add Ram Partition: 0x%08llx 0x%08llx\n", Base, Length));
    goto exit;
  }

  DEBUG ((EFI_D_WARN, "Successfully Added RAM Parition: 0x%08llx 0x%08llx\n", Base, Length));

exit:
  return Status;
}

EFI_STATUS
EFIAPI
AddRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                       Status              = EFI_SUCCESS;
  UINTN                            Index               = 1;
  UINT32                           NumPartitions       = 0;
  UINT32                           PartitionVersion    = 0;
  PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptor    = gExtendedMemoryDescriptor;
  BOOLEAN                          AddedFirstPartition = FALSE;
  RamPartitionTable               *RamPartitionTable   = NULL;

  // Get RAM Partition Infos
  Status = GetRamPartitions (&RamPartitionTable, &NumPartitions, &PartitionVersion);
  ASSERT_EFI_ERROR (Status);

  // Print RAM Partition Version
  if (PartitionVersion == 1) { 
    DEBUG ((EFI_D_ERROR, "RAM Partition Version 1 is not Supported!\n"));
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
  } else {
    DEBUG ((EFI_D_WARN, "RAM Partition Version: %d\n", PartitionVersion));
  }

  for (INT32 i = 0; i < NumPartitions; i++) {
    if (RamPartitionTable->RamPartitionEntry[i].Type != RAM_PART_SYS_MEMORY || RamPartitionTable->RamPartitionEntry[i].Category != RAM_PART_SDRAM) { continue; }
    if (RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength <= RAM_PARTITION_BASE) { continue; }

    DEBUG ((EFI_D_WARN, "\n"));
    DEBUG ((EFI_D_WARN, "RAM Partition Entry %d: 0x%08llx\n", i, RamPartitionTable->RamPartitionEntry[i].Base));
    DEBUG ((EFI_D_WARN, "Available Length:      0x%08llx\n",     RamPartitionTable->RamPartitionEntry[i].AvailableLength));
    DEBUG ((EFI_D_WARN, "\n"));

    if (RamPartitionTable->RamPartitionEntry[i].Base == GENERIC_RAM_BASE) {
      MemoryDescriptor[0].Address = RAM_PARTITION_BASE;
      MemoryDescriptor[0].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength + GENERIC_RAM_BASE - RAM_PARTITION_BASE;

      // Add New RAM Partition
      AddRamPartition (MemoryDescriptor[0].Address, MemoryDescriptor[0].Length, MemoryDescriptor[0].ArmAttributes, MemoryDescriptor[0].MemoryType);

      AddedFirstPartition = TRUE;

      continue;
    }

    if (!AddedFirstPartition) {
      MemoryDescriptor[Index].Address = RAM_PARTITION_BASE;
      MemoryDescriptor[Index].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength + GENERIC_RAM_BASE - RAM_PARTITION_BASE;

      AddedFirstPartition = TRUE;

      continue;
    }

    MemoryDescriptor[Index].Address = RamPartitionTable->RamPartitionEntry[i].Base;
    MemoryDescriptor[Index].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength;
    Index++;
  }

  Index = 1;

  while (MemoryDescriptor[Index].Length != 0) {
    ASSERT (Index < MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT);

    // Add New RAM Partition
    AddRamPartition (MemoryDescriptor[Index].Address, MemoryDescriptor[Index].Length, MemoryDescriptor[Index].ArmAttributes, MemoryDescriptor[Index].MemoryType);

    Index++;
  }

  return EFI_SUCCESS;
}