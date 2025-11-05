#ifndef _SIMPLE_FB_
#define _SIMPLE_FB_

//
// Display Details
//
typedef struct {
  EFI_MEMORY_REGION_DESCRIPTOR_EX MemoryRegion;
  UINT32                          Width;
  UINT32                          Height;
  UINT32                          ColorDepth;
} EFI_DISPLAY_DETAILS;

#endif /* _SIMPLE_FB_ */
