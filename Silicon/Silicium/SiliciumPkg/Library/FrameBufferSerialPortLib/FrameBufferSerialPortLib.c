#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include "FrameBuffer.h"
#include "Font5x16.h"

//
// Global Variables
//
STATIC EFI_FRAME_BUFFER_DATA FrameBufferData;

VOID
DrawDebugMessage (
  CHAR8  *Pixels,
  UINT32 *Glyph)
{
  UINT32 Color;
  UINT32 Data;
  UINT32 Temp;

  // Calculate Stride
  UINTN Stride = FrameBufferData.Width - (FONT_WIDTH * FrameBufferData.FontScale);

  for (UINT8 i = 0; i < 2; i++) {
    // Save current Glyph
    Data = Glyph[i];

    for (UINT8 j = 0; j < 6; j++) {
      // Save current Data
      Temp = Data;

      for (UINT8 k = 0; k < FrameBufferData.FontScale; k++) {
        // Restore Data
        Data = Temp;

        for (UINT8 l = 0; l < FONT_WIDTH; l++) {
          // Set Pixel Color
          UINT32 PixelColor = (Data & 1) ? 0xFFFFFFFF : 0x00000000;

          for (UINT8 m = 0; m < FrameBufferData.FontScale; m++) {
            // Save Color
            Color = PixelColor;

            for (UINT8 n = 0; n < FrameBufferData.BytesPerPixel; n++) {
              // Draw Color
              *Pixels = (UINT8)Color;

              // Shift Color
              Color >>= 8;

              // Next Pixel
              Pixels++;
            }
          }

          // Shift Data
          Data >>= 1;
        }

        // Next Pixel
        Pixels += (Stride * FrameBufferData.BytesPerPixel);
      }
    }
  }
}

VOID
WriteFrameBuffer (IN CHAR8 Buffer)
{
  CHAR8 *Pixels;

Print:
  // Check Buffer Length
  if ((UINT8)Buffer > 127) {
    return;
  }

  // Check Buffer Length
  if ((UINT8)Buffer < 32) {
    // Check for "\n" & Overflow X Position
    if (Buffer == '\n' || FrameBufferData.CurrentPosition->XPos >= FrameBufferData.MaxPosition.XPos - FrameBufferData.FontScale) {
      goto NewLine;
    }

    // Check for "\r"
    if (Buffer == '\r') {
      // Reset X Position
      FrameBufferData.CurrentPosition->XPos = 0;
    }

    return;
  }

  // Save some Space
  if (FrameBufferData.CurrentPosition->XPos == 0 && (UINT8)Buffer == ' ') {
    return;
  }

  // Set Debug Message Configuration
  Pixels  = (VOID *)FrameBufferData.MemoryRegion.Address;
  Pixels += (UINTN)FrameBufferData.CurrentPosition->YPos * ((FONT_HEIGHT - 4) * FrameBufferData.FontScale) * (FrameBufferData.Width * FrameBufferData.BytesPerPixel);
  Pixels += (UINTN)FrameBufferData.CurrentPosition->XPos * ((FONT_WIDTH + 1) * FrameBufferData.FontScale) * FrameBufferData.BytesPerPixel;

  // Draw Debug Message
  DrawDebugMessage (Pixels, Font5x16 + (Buffer - 32) * 2);

  // Increase X Position
  FrameBufferData.CurrentPosition->XPos++;

  // Check Max Position
  if (FrameBufferData.CurrentPosition->XPos < FrameBufferData.MaxPosition.XPos) {
    return;
  }

NewLine:
  // Wait
  MicroSecondDelay (FixedPcdGet32 (PcdFrameBufferDelay));

  // Increase Y Position
  FrameBufferData.CurrentPosition->YPos++;

  // Reset X Position
  FrameBufferData.CurrentPosition->XPos = 0;

  // Calculate next Line Position
  UINT32 NextLine = (FrameBufferData.CurrentPosition->YPos + 1) * ((FONT_HEIGHT - 4) * FrameBufferData.FontScale);

  // Verify Y Position
  if (NextLine >= FrameBufferData.Height) {
    // Clear Screen
    ZeroMem ((VOID *)FrameBufferData.MemoryRegion.Address, FrameBufferData.MemoryRegion.Length);

    // Flush Frame Buffer
    WriteBackInvalidateDataCacheRange ((VOID *)FrameBufferData.MemoryRegion.Address, FrameBufferData.MemoryRegion.Length);

    // Reset Y Position
    FrameBufferData.CurrentPosition->YPos = 0;

    goto Print;
  }

  // Flush Frame Buffer
  WriteBackInvalidateDataCacheRange ((VOID *)FrameBufferData.MemoryRegion.Address, FrameBufferData.MemoryRegion.Length);
}

UINTN
EFIAPI
SerialPortWrite (
  IN UINT8 *Buffer,
  IN UINTN  NumberOfBytes)
{
  UINT8 *CONST Final = &Buffer[NumberOfBytes];

  // Get Interrupts State
  BOOLEAN InterruptState = ArmGetInterruptState ();

  // Disable Interrupts
  if (InterruptState) {
    ArmDisableInterrupts ();
  }

  // Write Debug Message to Frame Buffer
  while (Buffer < Final) {
    WriteFrameBuffer (*Buffer++);
  }

  // Enable Interrupts
  if (InterruptState) {
    ArmEnableInterrupts ();
  }

  return NumberOfBytes;
}

UINTN
EFIAPI
SerialPortRead (
  OUT UINT8 *Buffer,
  IN  UINTN  NumberOfBytes)
{
  return 0;
}

BOOLEAN
EFIAPI
SerialPortPoll ()
{
  return FALSE;
}

EFI_STATUS
EFIAPI
SerialPortSetControl (IN UINT32 Control)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
SerialPortGetControl (OUT UINT32 *Control)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
SerialPortSetAttributes (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits)
{
  return EFI_UNSUPPORTED;
}

UINTN
SerialPortFlush ()
{
  return 0;
}

VOID
EnableSynchronousSerialPortIO ()
{
  // Do Nothing
}

EFI_STATUS
EFIAPI
SerialPortInitialize ()
{
  EFI_STATUS Status;

  // Locate "Display Reserved" Memory Region
  Status  = LocateMemoryMapAreaByName ("Display Reserved", &FrameBufferData.MemoryRegion);
  Status |= LocateMemoryMapAreaByName ("Display_Reserved", &FrameBufferData.MemoryRegion);
  if (EFI_ERROR (Status) && !FrameBufferData.MemoryRegion.Address) {
    return EFI_UNSUPPORTED;
  }

  // Set Frame Buffer Infos
  FrameBufferData.Width         = FixedPcdGet32 (PcdFrameBufferWidth);
  FrameBufferData.Height        = FixedPcdGet32 (PcdFrameBufferHeight);
  FrameBufferData.BytesPerPixel = FixedPcdGet8  (PcdFrameBufferColorDepth) / 8;

  // Update Display Reserved Length
  FrameBufferData.MemoryRegion.Length = FrameBufferData.Width * FrameBufferData.Height * FrameBufferData.BytesPerPixel;

  // Calculate Font Scales
  UINT8 ScaleX = FrameBufferData.Width / 480;
  UINT8 ScaleY = FrameBufferData.Height / 768;

  // Set Font Scale
  if (ScaleX == ScaleY) {
    FrameBufferData.FontScale = FrameBufferData.Width / 768;
  } else {
    FrameBufferData.FontScale = (ScaleX < ScaleY) ? ScaleX : ScaleY;
  }

  // Verify Font Scale
  if (!FrameBufferData.FontScale) {
    FrameBufferData.FontScale = 1;
  }

  // Set Total Position
  FrameBufferData.CurrentPosition = (EFI_FRAME_BUFFER_POSITION *)(FrameBufferData.MemoryRegion.Address + FrameBufferData.MemoryRegion.Length);

  // Calculate Max Position
  FrameBufferData.MaxPosition.XPos = FrameBufferData.Width / ((FONT_WIDTH + 1) * FrameBufferData.FontScale);
  FrameBufferData.MaxPosition.YPos = FrameBufferData.Height / ((FONT_HEIGHT - 3) * FrameBufferData.FontScale);

  return EFI_SUCCESS;
}
