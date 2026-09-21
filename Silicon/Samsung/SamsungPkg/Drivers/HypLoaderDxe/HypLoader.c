#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ArmSmcLib.h>

#include "HypLoader.h"

EFI_STATUS
StartHyp (IN EFI_PHYSICAL_ADDRESS Entry)
{
  ARM_SMC_ARGS HypStart = {0};

  // Set SMC Arguments
  HypStart.Arg0 = SMC_VMM_CALL_MAGIC_64;
  HypStart.Arg1 = Entry;
  HypStart.Arg2 = VMM_STACK_OFFSET;
  HypStart.Arg3 = VMM_MODE_AARCH64;

  // Start Hypervisor
  ArmCallSmc (&HypStart);

  // Verify SMC Return Value
  if (HypStart.Arg0 != 0) {
    DEBUG ((EFI_D_ERROR, "Failed to Start Hypervisor! SMC Return = 0x%llx\n", HypStart.Arg0));
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
LoadHyp (
  OUT EFI_PHYSICAL_ADDRESS *Base,
  OUT UINTN                *Length)
{
  EFI_STATUS  Status;
  UINT8      *Binary;
  UINTN       BinarySize;

  // Get Hypervisor Binary
  Status = GetSectionFromAnyFv (FixedPcdGetPtr (PcdHypFile), EFI_SECTION_RAW, 0, (VOID *)&Binary, &BinarySize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get SoC Hypervisor Binary!\n"));
    return Status;
  }

  // Set Hypervisor Memory Length
  *Length = EFI_SIZE_TO_PAGES (BinarySize);

  // Allocate Memory
  Status = gBS->AllocatePages (AllocateAnyPages, EfiReservedMemoryType, *Length, Base);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Hypervisor!\n"));
    return Status;
  }

  // Relocate Hypervisor Binary
  CopyMem ((VOID *)(UINTN)*Base, Binary, BinarySize);

  // Flush Cache
  WriteBackInvalidateDataCacheRange ((VOID *)(UINTN)*Base, BinarySize);
  InvalidateInstructionCacheRange   ((VOID *)(UINTN)*Base, BinarySize);

  // Show Progress
  DEBUG ((EFI_D_WARN, "Hypervisor Loaded at 0x%llx (%u Bytes)\n", *Base, BinarySize));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
HypLoaderEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS HypBase;
  UINTN                HypLength;

  // Load Hypervisor
  Status = LoadHyp (&HypBase, &HypLength);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Start Hypervisor
  Status = StartHyp (HypBase);
  if (EFI_ERROR (Status)) {
    // Free Memory
    gBS->FreePages (HypBase, HypLength);
  }

  return Status;
}
