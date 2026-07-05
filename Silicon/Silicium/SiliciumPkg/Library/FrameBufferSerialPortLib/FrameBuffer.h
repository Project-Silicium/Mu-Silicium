#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

//
// Frame Buffer Details
//
#define FB_WIDTH     FixedPcdGet32 (PcdFrameBufferWidth)
#define FB_HEIGHT    FixedPcdGet32 (PcdFrameBufferHeight)
#define FB_BPP      (FixedPcdGet8  (PcdFrameBufferColorDepth) / 8)
#define FB_DELAY_US  FixedPcdGet32 (PcdFrameBufferDelay)

//
// Frame Buffer Position
//
typedef struct {
  UINT32 XPos;
  UINT32 YPos;
} EFI_FRAME_BUFFER_POSITION;

#endif /* _FRAME_BUFFER_H_ */
