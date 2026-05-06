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
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapLib.h>
#include <Library/ArmMmuLib.h>

#include <Protocol/EFIRamPartition.h>

VOID
MapRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length)
{
  EFI_STATUS Status;

  // Add new Memory Space
  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeSystemMemory, Base, Length, 0xF);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to add new Memory Space! Status = %r\n", Status));
    DEBUG ((EFI_D_ERROR, "Affected RAM Partition: 0x%016llx - 0x%016llx\n", Base, Length));
    return;
  }

  // Set Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, WRITE_BACK_XN, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Attributes! Status = %r\n", Status));
    DEBUG ((EFI_D_ERROR, "Affected RAM Partition: 0x%016llx - 0x%016llx\n", Base, Length));
    return;
  }

  // Set Memory Space Attributes
  Status = gDS->SetMemorySpaceAttributes (Base, Length, EFI_MEMORY_WB);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Space Attributes! Status = %r\n", Status));
    DEBUG ((EFI_D_ERROR, "Affected RAM Partition: 0x%016llx - 0x%016llx\n", Base, Length));
    return;
  }

  // Show Progress
  DEBUG ((EFI_D_WARN, "Successfully Added RAM Partition: 0x%016llx - 0x%016llx\n", Base, Length));
}

VOID
ParseRamPartition (
  IN EFI_MEMORY_REGION_DESCRIPTOR *MemoryDescriptor,
  IN UINT8                         MemoryDescriptorCount,
  IN EFI_PHYSICAL_ADDRESS          RamBase,
  IN UINT64                        RamLength)
{
  // Save RAM Partition Details
  EFI_PHYSICAL_ADDRESS CurrentRamBase = RamBase;
  EFI_PHYSICAL_ADDRESS RamEnd         = RamBase + RamLength;

  // Go thru each Memory Map Region
  for (UINT8 i = 0; i < MemoryDescriptorCount; i++) {
    // Save Memory Region Details
    EFI_PHYSICAL_ADDRESS RegionBase = MemoryDescriptor[i].Address;
    EFI_PHYSICAL_ADDRESS RegionEnd  = MemoryDescriptor[i].Address + MemoryDescriptor[i].Length;

    // Verify RAM Partition End
    if (RegionBase >= RamEnd) {
      break;
    }

    // Skip Non-DDR Memory Regions
    if (RegionBase < FixedPcdGet64 (PcdSystemMemoryBase)) {
      continue;
    }

    // Verify Memory Region End
    if (RegionEnd <= CurrentRamBase) {
      continue;
    }

    // Check Memory Region Base
    if (RegionBase > CurrentRamBase) {
      // Set new RAM Partition Length
      UINT64 RamChunkLength = RegionBase - CurrentRamBase;

      // Map RAM Partition
      MapRamPartition (CurrentRamBase, RamChunkLength);
    }

    // Set new RAM Base
    if (RegionEnd > CurrentRamBase) {
      CurrentRamBase = RegionEnd;
    }
  }

  // Check Current RAM Partition Base
  if (CurrentRamBase < RamEnd) {
    // Set new RAM Partition Length
    UINT64 RamChunkLength = RamEnd - CurrentRamBase;

    // Map RAM Partition
    MapRamPartition (CurrentRamBase, RamChunkLength);
  }
}

EFI_STATUS
GetRamPartitions (
  OUT RamPartitionEntry **RamPartitions,
  OUT UINT32             *RamPartitionCount)
{
  EFI_STATUS                 Status;
  EFI_RAMPARTITION_PROTOCOL *mRamPartitionProtocol;

  // Locate RAM Partition Protocol
  Status = gBS->LocateProtocol (&gEfiRamPartitionProtocolGuid, NULL, (VOID *)&mRamPartitionProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate RAM Partition Protocol! Status = %r\n", Status));
    return Status;
  }

  // Get RAM Partition Count
  mRamPartitionProtocol->GetRamPartitions (mRamPartitionProtocol, NULL, RamPartitionCount);

  // Allocate Memory
  *RamPartitions = AllocateZeroPool (sizeof (RamPartitionEntry) * (*RamPartitionCount));
  if (*RamPartitions == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for RAM Partitions!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Get RAM Partitions
  Status = mRamPartitionProtocol->GetRamPartitions (mRamPartitionProtocol, *RamPartitions, RamPartitionCount);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partitions! Status = %r\n", Status));

    // Free Buffer
    FreePool (*RamPartitions);

    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ManageRam (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                    Status;
  EFI_MEMORY_REGION_DESCRIPTOR *MemoryDescriptor;
  RamPartitionEntry            *RamPartitions;
  UINT32                        RamPartitionCount;
  UINT8                         MemoryDescriptorCount;

  // Get Memory Map
  GetMemoryMap (&MemoryDescriptor, &MemoryDescriptorCount);

  // Get RAM Partitions
  Status = GetRamPartitions (&RamPartitions, &RamPartitionCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Parse each RAM Partition
  for (UINT32 i = 0; i < RamPartitionCount; i++) {
    ParseRamPartition (MemoryDescriptor, MemoryDescriptorCount, RamPartitions[i].Base, RamPartitions[i].AvailableLength);
  }

  // Free Buffer
  FreePool (RamPartitions);

  return EFI_SUCCESS;
}
