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
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RamManagerLib.h>
#include <Library/SortLib.h>

//
// Global Variables
//
STATIC EFI_MEMORY_DESCRIPTOR *EfiMemoryMap     = NULL;
STATIC UINTN                  EfiMemoryMapSize = 0;
STATIC UINTN                  DescriptorSize   = 0;

VOID
ParseMemoryRange (
  IN EFI_PHYSICAL_ADDRESS RangeBase,
  IN EFI_PHYSICAL_ADDRESS RangeEnd)
{
  EFI_STATUS Status;

  // Set Inital Memory Range Base
  EFI_PHYSICAL_ADDRESS CurrentRangeBase = RangeBase;

  // Go thru each Memory Map Region
  for (UINT16 i = 0; i < EfiMemoryMapSize / DescriptorSize; i++) {
    // Get new Memory Descriptor
    EFI_MEMORY_DESCRIPTOR *Descriptor = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)EfiMemoryMap + (i * DescriptorSize));

    // Save Memory Region Details
    EFI_PHYSICAL_ADDRESS RegionBase = Descriptor->PhysicalStart;
    EFI_PHYSICAL_ADDRESS RegionEnd  = Descriptor->PhysicalStart + EFI_PAGES_TO_SIZE (Descriptor->NumberOfPages);

    // Verify Memory Range End
    if (RegionBase >= RangeEnd) {
      break;
    }

    // Verify Memory Region End
    if (RegionEnd <= CurrentRangeBase) {
      continue;
    }

    // Check Memory Region Base
    if (RegionBase > CurrentRangeBase) {
      // Set new RAM Range Length
      UINT64 RamChunkLength = RegionBase - CurrentRangeBase;

      // Map RAM Range
      Status = MapMemoryRegion (CurrentRangeBase, RamChunkLength, EfiConventionalMemory);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Map RAM Range: 0x%llx - 0x%llx! Status = %r\n", CurrentRangeBase, RamChunkLength, Status));
      }
    }

    // Set new Memory Range Base
    if (RegionEnd > CurrentRangeBase) {
      CurrentRangeBase = RegionEnd;
    }
  }

  // Check Current Memory Range Base
  if (CurrentRangeBase < RangeEnd) {
    // Set new RAM Range Length
    UINT64 RamChunkLength = RangeEnd - CurrentRangeBase;

    // Map RAM Range
    Status = MapMemoryRegion (CurrentRangeBase, RamChunkLength, EfiConventionalMemory);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Map RAM Range: 0x%llx - 0x%llx! Status = %r\n", CurrentRangeBase, RamChunkLength, Status));
    }
  }
}

INTN
EFIAPI
CompareMemoryRegions (
  IN CONST VOID *Input1,
  IN CONST VOID *Input2)
{
  // Populate Memory Region Structure
  EFI_MEMORY_DESCRIPTOR *Region1 = (EFI_MEMORY_DESCRIPTOR *)Input1;
  EFI_MEMORY_DESCRIPTOR *Region2 = (EFI_MEMORY_DESCRIPTOR *)Input2;

  // Compare Base Addresses
  if (Region1->PhysicalStart < Region2->PhysicalStart) {
    return -1;
  } else if (Region1->PhysicalStart > Region2->PhysicalStart) {
    return 1;
  }

  return 0;
}

INTN
EFIAPI
CompareMemoryRanges (
  IN CONST VOID *Input1,
  IN CONST VOID *Input2)
{
  // Populate Memory Range Structure
  EFI_MEMORY_RANGE *Range1 = (EFI_MEMORY_RANGE *)Input1;
  EFI_MEMORY_RANGE *Range2 = (EFI_MEMORY_RANGE *)Input2;

  // Compare Base Addresses
  if (Range1->Address < Range2->Address) {
    return -1;
  } else if (Range1->Address > Range2->Address) {
    return 1;
  }

  return 0;
}

EFI_STATUS
GetEfiMemoryMap ()
{
  EFI_STATUS Status;

  // Get EFI Memory Map Size
  Status = gBS->GetMemoryMap (&EfiMemoryMapSize, EfiMemoryMap, NULL, NULL, NULL);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((EFI_D_ERROR, "Failed to get EFI Memory Map Size! Status = %r\n", Status));
    return Status;
  }

  // Add 1 KiB
  EfiMemoryMapSize += SIZE_1KB;

  // Allocate Memory
  EfiMemoryMap = AllocateZeroPool (EfiMemoryMapSize);
  if (EfiMemoryMap == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for EFI Memory Map!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Get EFI Memory Map
  Status = gBS->GetMemoryMap (&EfiMemoryMapSize, EfiMemoryMap, NULL, &DescriptorSize, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get EFI Memory Map! Status = %r\n", Status));
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
  EFI_STATUS        Status;
  EFI_MEMORY_RANGE *MemoryRange;
  UINT8             MemoryRangeCount;

  // Get EFI Memory Map
  Status = GetEfiMemoryMap ();
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Get usable Memory Ranges
  Status = GetUsableMemoryRanges (&MemoryRange, &MemoryRangeCount);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Sort Memory Map & Memory Ranges
  PerformQuickSort (EfiMemoryMap, EfiMemoryMapSize / DescriptorSize, DescriptorSize,            CompareMemoryRegions);
  PerformQuickSort (MemoryRange,  MemoryRangeCount,                  sizeof (EFI_MEMORY_RANGE), CompareMemoryRanges);

  // Go thru each Memory Range
  for (UINT8 i = 0; i < MemoryRangeCount; i++) {
    // Parse Memory Range
    ParseMemoryRange (MemoryRange[i].Address, (EFI_PHYSICAL_ADDRESS)(MemoryRange[i].Address + MemoryRange[i].Length));
  }

cleanup:
  // Free EFI Memory Map Buffer
  if (EfiMemoryMap != NULL) {
    FreePool (EfiMemoryMap);
  }

  // Free Memory Range Buffer
  if (MemoryRange != NULL) {
    FreePool (MemoryRange);
  }

  return Status;
}
