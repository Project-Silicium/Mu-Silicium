#ifndef _MEMORY_MAP_HELPER_LIB_H_
#define _MEMORY_MAP_HELPER_LIB_H_

#include <Library/MemoryMapLib.h>

/**
  This Function gets the Specified Memory Region by Name.

  @param[in]  RegionName                   - The Name of the Memory Region.
  @param[out] MemoryRegionDescriptor       - The Memory Region Data.

  @return EFI_SUCCESS                      - The Specified Memory Region was Found Successfully.
  @return EFI_INVALID_PARAMETER            - One or more Parameters are NULL.
  @return EFI_NOT_FOUND                    - The Specified Memory Region does not Exist.
**/
EFI_STATUS
EFIAPI
LocateMemoryRegionByName (
  IN  CHAR8                        *RegionName,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor
  );

/**
  This Function gets the Specified Memory Region by Base Address.

  @param[in]  RegionAddress                - The Base Address of the Memory Region.
  @param[out] MemoryRegionDescriptor       - The Memory Region Data.

  @return EFI_SUCCESS                      - The Specified Memory Region was Found Successfully.
  @return EFI_INVALID_PARAMETER            - The "MemoryRegionDescriptor" Parameter is NULL.
  @return EFI_NOT_FOUND                    - The Specified Memory Region does not Exist.
**/
EFI_STATUS
EFIAPI
LocateMemoryRegionByAddress (
  IN  EFI_PHYSICAL_ADDRESS          RegionAddress,
  OUT EFI_MEMORY_REGION_DESCRIPTOR *MemoryRegionDescriptor
  );

#endif /* _MEMORY_MAP_HELPER_LIB_H_ */
