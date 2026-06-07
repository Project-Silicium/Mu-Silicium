#ifndef _MEMORY_ALLOCATION_HELPER_LIB_H_
#define _MEMORY_ALLOCATION_HELPER_LIB_H_

/**
  This Function Maps the Specified Memory Region.

  @param[in] Address                       - The Memory Region Address.
  @param[in] Length                        - The Memory Region Length.
  @param[in] Type                          - The Memory Type.

  @return EFI_SUCCESS                      - Successfully Maped the Specified Memory Region.
  @return EFI_UNSUPPORTED                  - The Specified Memory Type is Unsupported.
  ...
**/
EFI_STATUS
MapMemoryRegion (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN UINT64               Length,
  IN EFI_MEMORY_TYPE      Type
  );

#endif /* _MEMORY_ALLOCATION_HELPER_LIB_H_ */
