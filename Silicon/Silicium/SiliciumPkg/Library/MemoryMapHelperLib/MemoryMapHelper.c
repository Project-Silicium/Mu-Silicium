#include <Library/MemoryMapHelperLib.h>

EFI_STATUS
EFIAPI
LocateMemoryRegionByName (
  IN  CHAR8                        *RegionName,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor)
{
  EFI_MEMORY_REGION_DESCRIPTOR *MemoryDescriptor;
  UINT8                         MemoryDescriptorCount;

  // Verify Parameters
  if (RegionName == NULL || MemoryRegionDescriptor == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Memory Map
  GetMemoryMap (&MemoryDescriptor, &MemoryDescriptorCount);

  // Go thru each Memory Region
  for (UINT8 i = 0; i < MemoryDescriptorCount; i++) {
    // Compare Memory Region Names
    if (AsciiStriCmp (RegionName, MemoryDescriptor[i].Name)) {
      continue;
    }

    // Pass Memory Region
    *MemoryRegionDescriptor = MemoryDescriptor[i];

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
LocateMemoryRegionByAddress (
  IN  EFI_PHYSICAL_ADDRESS          RegionAddress,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor)
{
  EFI_MEMORY_REGION_DESCRIPTOR *MemoryDescriptor;
  UINT8                         MemoryDescriptorCount;

  // Verify Parameters
  if (MemoryRegionDescriptor == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Memory Map
  GetMemoryMap (&MemoryDescriptor, &MemoryDescriptorCount);

  // Go thru each Memory Region
  for (UINT8 i = 0; i < MemoryDescriptorCount; i++) {
    // Compare Memory Region Addresses
    if (MemoryDescriptor->Address != RegionAddress) {
      continue;
    }

    // Pass Memory Region
    *MemoryRegionDescriptor = MemoryDescriptor[i];

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}
