/**
  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/SortLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>

VOID
BuildMemoryTypeInformationHob ();

INTN
EFIAPI
CompareMemoryRegions (
  IN CONST VOID *P1,
  IN CONST VOID *P2)
{
  // Populate Memory Region Structure
  CONST EFI_MEMORY_REGION_DESCRIPTOR_EX *Region1 = (EFI_MEMORY_REGION_DESCRIPTOR_EX *)P1;
  CONST EFI_MEMORY_REGION_DESCRIPTOR_EX *Region2 = (EFI_MEMORY_REGION_DESCRIPTOR_EX *)P2;

  // Compare Base Addresses
  if (Region1->Address < Region2->Address) {
    return -1;
  }

  // Compare Base Addresses
  if (Region1->Address > Region2->Address) {
    return 1;
  }

  return 0;
}

VOID
ValidateMemoryMap (IN EFI_PMEMORY_REGION_DESCRIPTOR_EX MemoryDescriptorEx)
{
  UINTN   RegionCount = 0;
  BOOLEAN NoErrors    = TRUE;

  // Count Memory Region
  while (MemoryDescriptorEx[RegionCount].Length != 0) {
    RegionCount++;
  }

  // Sort Memory Regions
  PerformQuickSort (MemoryDescriptorEx, RegionCount, sizeof (EFI_MEMORY_REGION_DESCRIPTOR_EX), CompareMemoryRegions);

  // Go thru each Memory Region
  for (UINTN i = 0; i < RegionCount; i++) {
    // Get Current Memory Region
    EFI_MEMORY_REGION_DESCRIPTOR_EX *CurrentRegion = &MemoryDescriptorEx[i];

    // Calculate End Address
    EFI_PHYSICAL_ADDRESS EndAddress = CurrentRegion->Address + CurrentRegion->Length;

    // Skip Register Regions
    if (CurrentRegion->Address < FixedPcdGet64 (PcdSystemMemoryBase)) {
      continue;
    }

    // Check for Memory Warparound
    if (EndAddress < CurrentRegion->Address) {
      DEBUG ((EFI_D_ERROR, "Memory Warparound Detected in \"%a\" (0x%llx)!\n", CurrentRegion->Name, CurrentRegion->Address));
      NoErrors = FALSE;
    }

    // Verify Next Memory Region
    if (i >= (RegionCount - 1)) {
      continue;
    }

    // Get Next Memory Region
    EFI_MEMORY_REGION_DESCRIPTOR_EX *NextRegion = &MemoryDescriptorEx[i + 1];

    // Check for Overlap
    if (EndAddress > NextRegion->Address) {
      DEBUG ((EFI_D_ERROR, "\"%a\" (0x%llx) and \"%a\" (0x%llx) Overlap!\n", CurrentRegion->Name, CurrentRegion->Address, NextRegion->Name, NextRegion->Address));
      NoErrors = FALSE;
    }
  }

  // Check for Errors
  ASSERT (NoErrors);
}

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

  // Validate Memory Map
  ValidateMemoryMap (MemoryDescriptorEx);

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
