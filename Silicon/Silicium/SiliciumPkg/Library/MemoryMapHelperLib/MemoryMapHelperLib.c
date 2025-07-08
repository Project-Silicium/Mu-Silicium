#include <Library/MemoryMapHelperLib.h>
#include <Library/DeviceMemoryMapLib.h>

EFI_STATUS
EFIAPI
LocateMemoryMapAreaByName (
  IN  CHAR8                           *MemoryMapAreaName,
  OUT ARM_MEMORY_REGION_DESCRIPTOR_EX *MemoryDescriptor)
{
  // Get Memory Map
  PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptorEx = GetDeviceMemoryMap ();

  while (MemoryDescriptorEx->Length != 0) {
    // Compare Memory Region Names
    if (AsciiStriCmp (MemoryMapAreaName, MemoryDescriptorEx->Name) == 0) {
      *MemoryDescriptor = *MemoryDescriptorEx;
      return EFI_SUCCESS;
    }

    // Switch to next Region
    MemoryDescriptorEx++;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
LocateMemoryMapAreaByAddress (
  IN  EFI_PHYSICAL_ADDRESS             MemoryMapAreaAddress,
  OUT ARM_MEMORY_REGION_DESCRIPTOR_EX *MemoryDescriptor)
{
  // Get Memory Map
  PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptorEx = GetDeviceMemoryMap ();

  while (MemoryDescriptorEx->Length != 0) {
    // Compare Memory Region Addresses
    if (MemoryDescriptorEx->Address == MemoryMapAreaAddress) {
      *MemoryDescriptor = *MemoryDescriptorEx;
      return EFI_SUCCESS;
    }

    // Switch to next Region
    MemoryDescriptorEx++;
  }

  return EFI_NOT_FOUND;
}
