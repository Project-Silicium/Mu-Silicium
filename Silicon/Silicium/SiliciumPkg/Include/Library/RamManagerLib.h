#ifndef _RAM_MANAGER_LIB_H_
#define _RAM_MANAGER_LIB_H_

//
// Memory Range
//
typedef struct {
  EFI_PHYSICAL_ADDRESS Address;
  UINT64               Length;
} EFI_MEMORY_RANGE;

/**
  This Function returns the Memory Ranges that are Usable by UEFI & the OS.

  @param[out] Range                        - The usable Memory Ranges.
  @param[out] RangeCount                   - The Amount of Memory Ranges.
**/
EFI_STATUS
GetUsableMemoryRanges (
  OUT EFI_MEMORY_RANGE **Range,
  OUT UINT8             *RangeCount
  );

#endif /* _RAM_MANAGER_LIB_H_ */
