#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

typedef struct _FBCON_POSITION {
  INTN x;
  INTN y;
} FBCON_POSITION, *PFBCON_POSITION;

typedef struct _FBCON_COLOR {
  UINTN Foreground;
  UINTN Background;
} FBCON_COLOR, *PFBCON_COLOR;

VOID
WriteFrameBuffer (CHAR8 Buffer);

BOOLEAN
FrameBufferLogInit ();

#endif /* _FRAME_BUFFER_H_ */
