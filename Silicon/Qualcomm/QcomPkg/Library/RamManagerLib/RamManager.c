#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RamManagerLib.h>

#include <Protocol/EFIRamPartition.h>

//
// Global Variables
//
STATIC EFI_RAMPARTITION_PROTOCOL *mRamPartitionProtocol;
STATIC RamPartitionEntry         *RamPartition;
STATIC UINT32                     RamPartitionCount;

EFI_STATUS
GetUsableMemoryRanges (
  OUT EFI_MEMORY_RANGE **Range,
  OUT UINT8             *RangeCount)
{
  EFI_STATUS        Status     = EFI_SUCCESS;
  EFI_MEMORY_RANGE *LocalRange = NULL;

  // Allocate Memory
  LocalRange = AllocateZeroPool (sizeof (EFI_MEMORY_RANGE) * RamPartitionCount);
  if (LocalRange == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for usable Memory Ranges!\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto cleanup;
  }

  // Populate Memory Ranges
  for (UINT32 i = 0; i < RamPartitionCount; i++) {
    LocalRange[i].Address = RamPartition[i].Base;
    LocalRange[i].Length  = RamPartition[i].AvailableLength;
  }

  // Pass Memory Range Details
  *Range      = LocalRange;
  *RangeCount = RamPartitionCount;

cleanup:
  // Free Buffer
  FreePool (RamPartition);

  return Status;
}

EFI_STATUS
EFIAPI
GetRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate RAM Partition Protocol
  Status = gBS->LocateProtocol (&gEfiRamPartitionProtocolGuid, NULL, (VOID *)&mRamPartitionProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate RAM Partition Protocol!\n"));
    return Status;
  }

  // Get RAM Partition Count
  Status = mRamPartitionProtocol->GetRamPartitions (mRamPartitionProtocol, NULL, &RamPartitionCount);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((EFI_D_ERROR, "Failed to get the Amount of RAM Partitions!\n"));
    return Status;
  }

  // Allocate Memory
  RamPartition = AllocateZeroPool (sizeof (RamPartitionEntry) * RamPartitionCount);
  if (RamPartition == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for RAM Partitions!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Get RAM Partitions
  Status = mRamPartitionProtocol->GetRamPartitions (mRamPartitionProtocol, RamPartition, &RamPartitionCount);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partitions!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
