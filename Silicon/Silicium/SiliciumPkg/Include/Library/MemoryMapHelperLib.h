#ifndef _MEMORY_MAP_HELPER_LIB_H_
#define _MEMORY_MAP_HELPER_LIB_H_

#include <Library/MemoryMapLib.h>

EFI_STATUS
EFIAPI
LocateMemoryRegionByName (
  IN  CHAR8                        *RegionName,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor
  );

EFI_STATUS
EFIAPI
LocateMemoryRegionByAddress (
  IN  EFI_PHYSICAL_ADDRESS          RegionAddress,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor
  );

#endif /* _MEMORY_MAP_HELPER_LIB_H_ */
