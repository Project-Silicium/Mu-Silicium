#include <Library/UefiBootServicesTableLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Library/DebugLib.h>

#include <Protocol/EFISmem.h>

EFI_STATUS
GetRamPartitionVersion (
  IN  RamPartitionTable *RamPartTable,
  OUT UINT32            *Version)
{
  // Check if RAM Partition Magics are Valid
  if (RamPartTable->Magic1 == RAM_PART_MAGIC1 && RamPartTable->Magic2 == RAM_PART_MAGIC2) {
    *Version = RamPartTable->Version;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
GetRamPartitions (
  OUT RamPartitionTable **RamPartTable,
  OUT UINT32             *Version)
{
  EFI_STATUS         Status        = EFI_SUCCESS;
  EFI_SMEM_PROTOCOL *mSmemProtocol = NULL;
  UINT32             SmemSize      = 0;

  // Locate SMEM Protocol
  Status = gBS->LocateProtocol (&gEfiSMEMProtocolGuid, NULL, (VOID *)&mSmemProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SMEM Protocol! Status = %r\n", Status));
    return Status;
  }

  // Get the RAM Partition Table
  Status = mSmemProtocol->GetFunc (SMEM_USABLE_RAM_PARTITION_TABLE, &SmemSize, (VOID *)RamPartTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partition Table! Status = %r\n", Status));
    return Status;
  }

  // Get RAM Partition Version
  Status = GetRamPartitionVersion (*RamPartTable, Version);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partition Version! Status = %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
