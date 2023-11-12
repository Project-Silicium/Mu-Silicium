# FrameBufferMemDrawLib

FrameBufferMemDrawLib is a wrapper around FrameBufferBltLib. This offers some
nice benefits as the FrameBufferBltLib offers a nice abstractions
of manipulating the frame buffer while it's in memory. This is most useful
in situations where the full display protocol might not be up yet.
FrameBufferMemDrawLib tries to simplify using the FrameBufferBltLib by
gathering the information about the display, the pixel format, and getting
the handles needed to manipulate the frame buffer. This is done through a
constructor and a destructor.

## Methods offered

### MemDrawOnFrameBuffer

This is meant to take a buffer that is formatted with 32bit pixels in the
standard RGB+reserved format. It takes in the top left corner of the position
on the screen where the buffer should be drawn. It also takes in the number of
rows and columns of the buffer.

## MemFillOnFrameBuffer

This fills in a region with a solid color. This color is the standard 32 bit
format referenced in the previous method. This functions takes in the top left
corner of the position on the screen where the color should be filled. It also
takes in the number of rows and columns that the color should fill out to.
