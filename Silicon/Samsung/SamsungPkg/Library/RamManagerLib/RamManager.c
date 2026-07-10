#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/RamManagerLib.h>
#include <Library/IoLib.h>

#include "RamManager.h"

//
// Global Variables
//
STATIC EFI_PHYSICAL_ADDRESS MemoryStart;
STATIC EFI_PHYSICAL_ADDRESS MemoryEnd;
STATIC EFI_MEMORY_RANGE     HoleRange[5];

EFI_STATUS
GetUsableMemoryRanges (
  OUT EFI_MEMORY_RANGE **Range,
  OUT UINT8             *RangeCount)
{
  // Set Inital Values
  EFI_PHYSICAL_ADDRESS  CurrentMemoryStart = MemoryStart;
  EFI_MEMORY_RANGE     *LocalRange         = NULL;
  UINT8                 LocalRangeCount    = ARRAY_SIZE (HoleRange) + 1;

  // Allocate Memory
  LocalRange = AllocateZeroPool (sizeof (EFI_MEMORY_RANGE) * LocalRangeCount);
  if (LocalRange == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Memory Ranges!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // Go thru each Memory Hole
  for (UINT8 i = 0; i < ARRAY_SIZE (HoleRange); i++) {
    // Verify Memory Hole
    if (!HoleRange[i].Address) {
      continue;
    }

    // Set Memory Range Length
    UINT64 MemoryRangeLength = HoleRange[i].Address - CurrentMemoryStart;

    // Parse Memory Range
    if (MemoryRangeLength > 0) {
      LocalRange[i].Address = CurrentMemoryStart;
      LocalRange[i].Length  = MemoryRangeLength;
    }

    // Set new Memory Range Start
    CurrentMemoryStart = HoleRange[i].Address + HoleRange[i].Length;
  }

  // Add Remaining Memory
  if (MemoryEnd > CurrentMemoryStart) {
    LocalRange[LocalRangeCount - 1].Address = CurrentMemoryStart;
    LocalRange[LocalRangeCount - 1].Length  = MemoryEnd - CurrentMemoryStart;
  }

  // Pass Data
  *Range      = LocalRange;
  *RangeCount = LocalRangeCount;

  return EFI_SUCCESS;
}

EFI_STATUS
GetMemorySpace ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR NsIramRegion;
  UINT64                       MemorySize;

  // Locate "NS_IRAM" Memory Region
  Status = LocateMemoryRegionByName ("NS_IRAM", &NsIramRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"NS_IRAM\" Memory Region!\n"));
    return Status;
  }

  // Get System Memory Size
  MemorySize = MmioRead64 (NsIramRegion.Address + 0x848);

  // Set Memory Address
  MemoryStart = FixedPcdGet64 (PcdSystemMemoryBase);

  // Set Memory End Address
  if (MemoryStart == BASE_2GB) {
    MemoryEnd = BASE_32GB + MemorySize;
  } else {
    MemoryEnd = MemoryStart + MemorySize;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetMemoryHoles ()
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS HoleAddress;
  UINT64               HoleLength;

  // Clear Memory Hole Structure
  ZeroMem ((VOID *)HoleRange, sizeof (EFI_MEMORY_RANGE) * ARRAY_SIZE (HoleRange));

  // Get Secure DRAM Memory Hole
  Status = GetSecureDramRange (&HoleAddress, &HoleLength);
  if (!EFI_ERROR (Status)) {
    HoleRange[0].Address = HoleAddress;
    HoleRange[0].Length  = HoleLength;
  }

  // Get H-Arx Memory Hole
  Status = GetHarxRange (&HoleAddress, &HoleLength);
  if (!EFI_ERROR (Status)) {
    HoleRange[1].Address = HoleAddress;
    HoleRange[1].Length  = HoleLength;
  }

  // Get DRM Plugin Memory Hole
  Status = GetDrmPluginRange (&HoleAddress, &HoleLength);
  if (!EFI_ERROR (Status)) {
    HoleRange[2].Address = HoleAddress;
    HoleRange[2].Length  = HoleLength;
  }

  // Get Secure PGTBL Memory Hole
  Status = GetSecurePgtblRange (&HoleAddress, &HoleLength);
  if (!EFI_ERROR (Status)) {
    HoleRange[3].Address = HoleAddress;
    HoleRange[3].Length  = HoleLength;
  }

  // Add DRAM2 Carve-out
  if (MemoryStart == BASE_2GB) {
    HoleRange[4].Address = BASE_4GB;
    HoleRange[4].Length  = (UINT64)SIZE_1GB * 30;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetMemoryData (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Memory Start & End
  Status = GetMemorySpace ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get Memory Holes
  Status = GetMemoryHoles ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

