/**
  Copyright (c) 2015-2018, 2020, The Linux Foundation. All rights reserved.
  Copyright (c) 2022. Sunflower2333. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

  - Neither the name of The Linux Foundation nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
  ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Library/MemoryMapHelperLib.h>
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
  // Populate RAM Partition Entry
  CONST EFI_RAM_PARTITION_ENTRY *Part1 = (EFI_RAM_PARTITION_ENTRY *)P1;
  CONST EFI_RAM_PARTITION_ENTRY *Part2 = (EFI_RAM_PARTITION_ENTRY *)P2;

  // Compare Base Addresses
  if (Part1->Base < Part2->Base) {
    return -1;
  }

  // Compare Base Addresses
  if (Part1->Base > Part2->Base) {
    return 1;
  }

  return 0;
}

EFI_STATUS
EFIAPI
AddRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_RAM_PARTITION_TABLE          *RamPartitionTable   = NULL;
  EFI_PMEMORY_REGION_DESCRIPTOR_EX  MemoryDescriptor    = gExtendedMemoryDescriptor;
  EFI_MEMORY_REGION_DESCRIPTOR_EX   HypReservedRegion   = {0};
  UINT32                            PartitionVersion    = 0;
  UINTN                             Index               = 1;

  // Get RAM Partition Infos
  ASSERT_EFI_ERROR (GetRamPartitions (&RamPartitionTable, &PartitionVersion));

  // Verify RAM Partition Version
  if (PartitionVersion <= 1) { 
    DEBUG ((EFI_D_ERROR, "RAM Partition Version %u is not Supported!\n", PartitionVersion));
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
  }

  // Get Number of RAM Partitions
  UINT32 NumPartitions = RamPartitionTable->NumPartitions;

  // Sort all RAM Partitions
  PerformQuickSort (RamPartitionTable->RamPartitionEntry, NumPartitions, sizeof (EFI_RAM_PARTITION_ENTRY), CompareBaseAddress);

  // Get "HYP Reserved" Memory Region
  LocateMemoryMapAreaByName ("HYP Reserved", &HypReservedRegion);
  LocateMemoryMapAreaByName ("HYP_Reserved", &HypReservedRegion);

  for (INT32 i = 0; i < NumPartitions; i++) {
    // Check if the RAM Partition is Valid
    if (RamPartitionTable->RamPartitionEntry[i].Type != RAM_PART_SYS_MEMORY || RamPartitionTable->RamPartitionEntry[i].Category != RAM_PART_SDRAM) { continue; }
    if (RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength <= RAM_PARTITION_BASE) { continue; }

    // Add First RAM Partition
    if (!AnyRamPartitionAdded) {
      MemoryDescriptor[0].Address = RAM_PARTITION_BASE;
      MemoryDescriptor[0].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength - RAM_PARTITION_BASE + RamPartitionTable->RamPartitionEntry[i].Base;

      // Checks for "HYP Reserved"
      if (HypReservedRegion.Address) {
        // Check if the RAM Partition is Valid
        if (MemoryDescriptor[0].Address >= HypReservedRegion.Address && MemoryDescriptor[0].Address + MemoryDescriptor[0].Length < HypReservedRegion.Address + HypReservedRegion.Length) { continue; }

        // Adjust RAM Partition Length
        if (MemoryDescriptor[0].Address < HypReservedRegion.Address && MemoryDescriptor[0].Address + MemoryDescriptor[0].Length > HypReservedRegion.Address && MemoryDescriptor[0].Address + MemoryDescriptor[0].Length <= HypReservedRegion.Address + HypReservedRegion.Length) {
          MemoryDescriptor[0].Length = HypReservedRegion.Address - MemoryDescriptor[0].Address;
        }
      }

      // Add new RAM Partition
      AddRamPartition (MemoryDescriptor[0].Address, MemoryDescriptor[0].Length, MemoryDescriptor[0].ArmAttributes, MemoryDescriptor[0].MemoryType);
      continue;
    }

    // Checks for "HYP Reserved"
    if (HypReservedRegion.Address) {
      // Check if the RAM Partition is Valid
      if (RamPartitionTable->RamPartitionEntry[i].Base >= HypReservedRegion.Address && RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength < HypReservedRegion.Address + HypReservedRegion.Length) { continue; }

      // Adjust RAM partition Base & Length
      if (RamPartitionTable->RamPartitionEntry[i].Base < HypReservedRegion.Address && RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength > HypReservedRegion.Address + HypReservedRegion.Length) {
        // Calculate new Length
        MemoryDescriptor[Index].Address = RamPartitionTable->RamPartitionEntry[i].Base;
        MemoryDescriptor[Index].Length  = HypReservedRegion.Address - RamPartitionTable->RamPartitionEntry[i].Base;

        // Increase Index Value
        Index++;

        // Calculate new Base & Length
        MemoryDescriptor[Index].Address = HypReservedRegion.Address + HypReservedRegion.Length;
        MemoryDescriptor[Index].Length  = RamPartitionTable->RamPartitionEntry[i].AvailableLength - RamPartitionTable->RamPartitionEntry[i].Base + HypReservedRegion.Address + HypReservedRegion.Length;

        // Increase Index Value
        Index++;

        // Move to next Entry
        continue;
      }

      // Adjust RAM Partition Length
      if (RamPartitionTable->RamPartitionEntry[i].Base < HypReservedRegion.Address && RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength > HypReservedRegion.Address && RamPartitionTable->RamPartitionEntry[i].Base + RamPartitionTable->RamPartitionEntry[i].AvailableLength <= HypReservedRegion.Address + HypReservedRegion.Length) {
        RamPartitionTable->RamPartitionEntry[i].AvailableLength = HypReservedRegion.Address - RamPartitionTable->RamPartitionEntry[i].Base;
      }
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

  // Check for RAM Partitions
  if (!AnyRamPartitionAdded) {
    DEBUG ((EFI_D_ERROR, "No RAM Partitions were Added! Stoping UEFI now.\n"));
    ASSERT_EFI_ERROR (EFI_NOT_READY);
  }

  return EFI_SUCCESS;
}
