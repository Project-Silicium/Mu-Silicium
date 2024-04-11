#include <Library/UefiBootServicesTableLib.h>
#include <Library/RamPartitionTableLib.h>
#include <Library/DebugLib.h>

#include <Protocol/EFISmem.h>

EFI_STATUS
GetRamPartitionVersion (
  IN  RamPartitionTable *RamPartitionTable,
  OUT UINT32            *Version)
{
  // Check if RAM Partition Magics are Valid
  if (RamPartitionTable->Magic1 == RAM_PART_MAGIC1 && RamPartitionTable->Magic2 == RAM_PART_MAGIC2) {
    *Version = RamPartitionTable->Version;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
GetRamPartitions (
  OUT RamPartitionTable **RamPartitionTable,
  OUT UINT32             *NumPartitions,
  OUT UINT32             *Version)
{
  EFI_STATUS         Status        = EFI_SUCCESS;
  EFI_SMEM_PROTOCOL *gSmemProtocol = NULL;

  // Locate SMEM Protocol
  Status = gBS->LocateProtocol (&gEfiSMEMProtocolGuid, NULL, (VOID *)&gSmemProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SMEM Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Get the RAM Partition Table
  Status = gSmemProtocol->GetFunc (SMEM_USABLE_RAM_PARTITION_TABLE, NumPartitions, (VOID *)RamPartitionTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partition Table! Status = %r\n", Status));
    goto exit;
  }

  // Get RAM Partition Version
  Status = GetRamPartitionVersion (*RamPartitionTable, Version);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get RAM Partition Version! Status = %r\n", Status));
    goto exit;
  }

exit:
  return Status;
}
