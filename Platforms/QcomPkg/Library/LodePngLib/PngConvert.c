#include <PiDxe.h>

#include <Library/BgraRgbaConvert.h>

void ConvertBetweenBGRAandRGBA(UINT8 *input, int pixel_width, int pixel_height)
{
  int offset = 0;

  for (int y = 0; y < pixel_height; y++) {
    for (int x = 0; x < pixel_width; x++) {
      input[offset] ^= input[offset + 2];
      input[offset + 2] ^= input[offset];
      input[offset] ^= input[offset + 2];

      offset += 4;
    }
  }
}