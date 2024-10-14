#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/DynamicRamLib.h>

#define RAM_PARTITION_START_ADDRESS FixedPcdGet64 (PcdRamPartitionBase)

BOOLEAN AnyRamPartitionAdded = FALSE;

VOID
AddRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length)
{
  EFI_STATUS Status;

  if (Length <= 0) { return; }

  // Add New Memory Space
  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeSystemMemory, Base, Length, 0xF);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Add Memory Space for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  // Set New Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, EFI_RESOURCE_SYSTEM_MEMORY, 0);
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

EFI_STATUS
EFIAPI
AddRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  UINT64     MemorySize;

  // Get Memory Size
  Status = GetMemorySize (&MemorySize);
  if (Status == EFI_NOT_FOUND) {
    DEBUG ((EFI_D_WARN, "This Device does not Support this Driver.\n"));
    return Status;
  } else if (EFI_ERROR (Status)) {
    goto exit;
  }

  // Print Total RAM Size
  DEBUG ((EFI_D_WARN, "Total RAM Size: %u MB\n", MemorySize / 0x100000));

  // Update Memory Size
  MemorySize = MemorySize - RAM_PARTITION_START_ADDRESS + FixedPcdGet64 (PcdSystemMemoryBase);

  // Fill Rest of DRAM
  AddRamPartition (RAM_PARTITION_START_ADDRESS, MemorySize);

exit:
  if (!AnyRamPartitionAdded) {
    DEBUG ((EFI_D_ERROR, "No RAM Partitions were Added! Stoping UEFI now.\n"));
    ASSERT_EFI_ERROR (EFI_NOT_READY);
  }

  return EFI_SUCCESS;
}
