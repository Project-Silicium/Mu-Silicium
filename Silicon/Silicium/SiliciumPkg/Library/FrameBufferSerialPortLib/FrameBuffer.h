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
// Font Data
//
typedef struct {
  UINT8 Width;
  UINT8 Height;
  UINT8 Scale;
} EFI_FONT_DATA;

//
// Frame Buffer Data
//
typedef struct {
  ARM_MEMORY_REGION_DESCRIPTOR_EX  MemoryRegion;
  EFI_FRAME_BUFFER_POSITION       *CurrentPosition;
  EFI_FRAME_BUFFER_POSITION        MaxPosition;
  EFI_FONT_DATA                    FontData;
  UINT32                           PrintDelay;
  UINT32                           Width;
  UINT32                           Height;
  UINT8                            BytesPerPixel;
} EFI_FRAME_BUFFER_DATA;

#endif /* _FRAME_BUFFER_H_ */
