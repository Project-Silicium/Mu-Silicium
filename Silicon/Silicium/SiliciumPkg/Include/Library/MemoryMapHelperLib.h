#ifndef _MEMORY_MAP_HELPER_LIB_H_
#define _MEMORY_MAP_HELPER_LIB_H_

#include <Library/MemoryMapLib.h>

EFI_STATUS
EFIAPI
LocateMemoryMapAreaByName (
  CHAR8                           *MemoryMapAreaName,
  EFI_MEMORY_REGION_DESCRIPTOR_EX *MemoryDescriptor
  );

EFI_STATUS
EFIAPI
LocateMemoryMapAreaByAddress (
  EFI_PHYSICAL_ADDRESS             MemoryMapAreaAddress,
  EFI_MEMORY_REGION_DESCRIPTOR_EX *MemoryDescriptor
  );

#endif /* _MEMORY_MAP_HELPER_LIB_H_ */
