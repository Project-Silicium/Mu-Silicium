#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

//
// Frame Buffer Position
//
typedef struct {
  UINT32 XPos;
  UINT32 YPos;
} EFI_FRAME_BUFFER_POSITION;

//
// Frame Buffer Data
//
typedef struct {
  EFI_MEMORY_REGION_DESCRIPTOR_EX  MemoryRegion;
  EFI_FRAME_BUFFER_POSITION       *CurrentPosition;
  EFI_FRAME_BUFFER_POSITION        MaxPosition;
  UINT32                           Width;
  UINT32                           Height;
  UINT8                            BytesPerPixel;
  UINT8                            FontScale;
} EFI_FRAME_BUFFER_DATA;

#endif /* _FRAME_BUFFER_H_ */
