/**
  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>

VOID
BuildMemoryTypeInformationHob ();

EFI_STATUS
EFIAPI
MemoryPeim (
  IN EFI_PHYSICAL_ADDRESS UefiMemoryBase,
  IN UINT64               UefiMemorySize)
{
  EFI_STATUS                        Status                                              = EFI_SUCCESS;
  ARM_MEMORY_REGION_DESCRIPTOR      MemoryTable[MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT] = {0};
  EFI_PMEMORY_REGION_DESCRIPTOR_EX  MemoryDescriptorEx                                  = GetMemoryMap ();
  VOID                             *TranslationTableBase                                = NULL;
  UINTN                             TranslationTableSize                                = 0;
  UINTN                             Index                                               = 0;

  // Check Memory Size
  ASSERT (PcdGet64 (PcdSystemMemorySize) != 0);

  while (MemoryDescriptorEx->Length != 0) {
    switch (MemoryDescriptorEx->HobOption) {
      case AddMem:
      case AddDev:
      case HobOnlyNoCacheSetting:
        // Build Resource Descriptor HOB
        BuildResourceDescriptorHob (MemoryDescriptorEx->ResourceType, MemoryDescriptorEx->ResourceAttribute, MemoryDescriptorEx->Address, MemoryDescriptorEx->Length);

      case AllocOnly:
        // Build Memory Allocation HOB
        if (MemoryDescriptorEx->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY || MemoryDescriptorEx->MemoryType == EfiRuntimeServicesData) {
          BuildMemoryAllocationHob (MemoryDescriptorEx->Address, MemoryDescriptorEx->Length, MemoryDescriptorEx->MemoryType);
        }

        break;

      case NoHob:
      default:
        goto update;
    }

    // Switch to Next Region
    if (MemoryDescriptorEx->HobOption != HobOnlyNoCacheSetting) {
update:
      // Check Index
      ASSERT (Index < MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT);

      // Save Attributes
      MemoryTable[Index].PhysicalBase = MemoryDescriptorEx->Address;
      MemoryTable[Index].VirtualBase  = MemoryDescriptorEx->Address;
      MemoryTable[Index].Length       = MemoryDescriptorEx->Length;
      MemoryTable[Index].Attributes   = MemoryDescriptorEx->ArmAttributes;

      // Increase Index
      Index++;
    }

    // Switch to next Region
    MemoryDescriptorEx++;
  }

  // Build Memory Allocation HOB
  Status = ArmConfigureMmu (MemoryTable, &TranslationTableBase, &TranslationTableSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Build Memory Type Info HOB
  if (FeaturePcdGet (PcdPrePiProduceMemoryTypeInformationHob)) {
    BuildMemoryTypeInformationHob ();
  }

  return EFI_SUCCESS;
}
