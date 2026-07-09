#ifndef _EFI_MEMORY_MAP_UTILS_H_
#define _EFI_MEMORY_MAP_UTILS_H_

//
// EFI Memory Map Helper Macros
//
#define GET_MEMORY_DESCRIPTOR(EfiMemMap, Index, DescSize)                ((EFI_MEMORY_DESCRIPTOR *)((UINT8 *)(EfiMemMap) + ((Index) * (DescSize))))
#define MEMORY_DESCRIPTOR_LIST_SIZE(EfiMemMapSize, DescSize)             ((EfiMemMapSize) / (DescSize))

#endif /* _EFI_MEMORY_MAP_UTILS_H_ */
