//#include <Library/ArmLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <Resources/font5x12.h>

#include "FrameBuffer.h"

//
// Global Variables
//
STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX  DisplayMemoryRegion;
STATIC FBCON_POSITION                  *CurrentPosition;
STATIC FBCON_POSITION                   MaxPosition;
STATIC UINTN                            ScreenWidth;
STATIC UINTN                            ScreenHeight;
STATIC UINTN                            ScreenColorDepth;

VOID
DrawDebugMessage (
  CHAR8 *Pixels,
  UINTN  Stride,
  UINTN  Bpp,
  UINTN *Glyph)
{
  UINT32 ForegroundColor;
  UINT32 BackgroundColor;
  UINTN  Data;
  UINTN  Temp;

  // Set Background Pixels Variable
  CHAR8 *BackgroundPixels = Pixels;

  // Update Stride Value
  Stride -= FONT_WIDTH * SCALE_FACTOR;

  // Go thru each Pixel
  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        for (UINTN j = 0; j < SCALE_FACTOR; j++) {
          // Set Background Color
          BackgroundColor = 0xFF000000;

          for (UINTN k = 0; k < Bpp; k++) {
            // Draw Background Color
            *BackgroundPixels = (UINT8)BackgroundColor;
            BackgroundColor   = BackgroundColor >> 8;

            // Update Position
            BackgroundPixels++;
          }
        }
      }

      // Update Position
      BackgroundPixels += (Stride * Bpp);
    }
  }

  // Go thru each Pixel
  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        for (UINTN j = 0; j < SCALE_FACTOR; j++) {
          // Set background Pixels
          BackgroundColor = 0xFF000000;

          for (UINTN k = 0; k < Bpp; k++) {
            // Draw Background Color
            *BackgroundPixels = (UINT8)BackgroundColor;
            BackgroundColor   = BackgroundColor >> 8;

            // Update Position
            BackgroundPixels++;
          }
        }
      }

      // Update Position
      BackgroundPixels += (Stride * Bpp);
    }
  }

  // Set Data Variable
  Data = Glyph[0];

  // Go thru each Pixel
  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    Temp = Data;

    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      Data = Temp;

      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        if (Data & 1) {
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            // Set Foreground Color
            ForegroundColor = 0xFFFFFFFF;

            for (UINTN k = 0; k < Bpp; k++) {
              // Draw Foreground Color
              *Pixels         = (UINT8)ForegroundColor;
              ForegroundColor = ForegroundColor >> 8;

              // Update Position
              Pixels++;
            }
          }
        } else {
          // Update Position
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            Pixels = Pixels + Bpp;
          }
        }

        // Shift Position
        Data >>= 1;
      }

      // Update Position
      Pixels += (Stride * Bpp);
    }
  }

  // Set Data Position
  Data = Glyph[1];

  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    Temp = Data;

    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      Data = Temp;

      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        if (Data & 1) {
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            // Set Foreground Color
            ForegroundColor = 0xFFFFFFFF;

            for (UINTN k = 0; k < Bpp; k++) {
              // Draw Foreground Color
              *Pixels  = (UINT8)ForegroundColor;
              ForegroundColor = ForegroundColor >> 8;

              // Update Position
              Pixels++;
            }
          }
        } else {
          // Update Position
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            Pixels = Pixels + Bpp;
          }
        }

        // Shift Position
        Data >>= 1;
      }

      // Update Position
      Pixels += (Stride * Bpp);
    }
  }
}

VOID
WriteFrameBuffer (CHAR8 Buffer)
{
  CHAR8  *Pixels;

  // Get Interrupts State
  BOOLEAN InterruptsEnabled = ArmGetInterruptState ();

Print:
  // Check Buffer Length
  if ((UINT8)Buffer > 127) {
    return;
  }

  if ((UINT8)Buffer < 32) {
    if (Buffer == '\n') {
      // Create new Line
      goto newline;
    } else if (Buffer == '\r') {
      // Reset X Position
      CurrentPosition->x = 0;
      return;
    } else {
      return;
    }
  }

  // Save some Space
  if (CurrentPosition->x == 0 && (UINT8)Buffer == ' ') {
    return;
  }

  // Disable Interrupts
  if (InterruptsEnabled) {
    ArmDisableInterrupts ();
  }

  // Set Debug Message Configuration
  Pixels  = (VOID *)DisplayMemoryRegion.Address;
  Pixels += CurrentPosition->y * ((ScreenColorDepth / 8) * FONT_HEIGHT * ScreenWidth);
  Pixels += CurrentPosition->x * SCALE_FACTOR * ((ScreenColorDepth / 8) * (FONT_WIDTH + 1));

  // Draw Debug Message
  DrawDebugMessage (Pixels, ScreenWidth, (ScreenColorDepth / 8), font5x12 + (Buffer - 32) * 2);

  // Increase X Position
  CurrentPosition->x++;

  // Create new Line
  if (CurrentPosition->x >= (INT32)(MaxPosition.x / SCALE_FACTOR)) {
    goto newline;
  }

  // Enable Interrupts
  if (InterruptsEnabled) {
    ArmEnableInterrupts ();
  }

  return;

newline:
  // Wait
  MicroSecondDelay (FixedPcdGet32 (PcdDebugFrameBufferDelay));

  // Increase Y Position
  CurrentPosition->y += SCALE_FACTOR;

  // Reset X Position
  CurrentPosition->x  = 0;

  if (CurrentPosition->y >= MaxPosition.y - SCALE_FACTOR) {
    // Clear Screen
    ZeroMem ((VOID *)DisplayMemoryRegion.Address, (ScreenWidth * ScreenHeight * (ScreenColorDepth / 8)));

    // Flush Frame Buffer
    WriteBackInvalidateDataCacheRange ((VOID *)DisplayMemoryRegion.Address, (ScreenWidth * ScreenHeight * (ScreenColorDepth / 8)));

    // Reset Y Position
    CurrentPosition->y = -1;

    // Enable Interrupts
    if (InterruptsEnabled) {
      ArmEnableInterrupts ();
    }

    goto Print;
  }

  // Flush FrameBuffer
  WriteBackInvalidateDataCacheRange ((VOID *)DisplayMemoryRegion.Address, (ScreenWidth * ScreenHeight * (ScreenColorDepth / 8)));

  // Enable Interrupts
  if (InterruptsEnabled) {
    ArmEnableInterrupts ();
  }
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
  Status  = LocateMemoryMapAreaByName ("Display Reserved", &DisplayMemoryRegion);
  Status |= LocateMemoryMapAreaByName ("Display_Reserved", &DisplayMemoryRegion);
  if (EFI_ERROR (Status) && !DisplayMemoryRegion.Address) { 
    return EFI_UNSUPPORTED;
  }

  // Get Secondary Frame Buffer Offset
  UINT32 Offset = FixedPcdGet32 (PcdSecondaryFrameBufferOffset);

  // Get Frame Buffer Infos
  if (Offset && FixedPcdGetBool (PcdSecondaryFrameBufferDebug)) {
    // Update "Display Reserved" Memory Region
    DisplayMemoryRegion.Address += Offset;
    DisplayMemoryRegion.Length  -= Offset;

    // Set Screen Infos
    ScreenWidth      = FixedPcdGet32 (PcdSecondaryFrameBufferWidth);
    ScreenHeight     = FixedPcdGet32 (PcdSecondaryFrameBufferHeight);
    ScreenColorDepth = FixedPcdGet32 (PcdSecondaryFrameBufferColorDepth);
  } else {
    // Set Screen Infos
    ScreenWidth      = FixedPcdGet32 (PcdPrimaryFrameBufferWidth);
    ScreenHeight     = FixedPcdGet32 (PcdPrimaryFrameBufferHeight);
    ScreenColorDepth = FixedPcdGet32 (PcdPrimaryFrameBufferColorDepth);
  }

  // Set Total Position
  CurrentPosition = (FBCON_POSITION *)(DisplayMemoryRegion.Address + (ScreenWidth * ScreenHeight * ScreenColorDepth / 8));

  // Calculate Max Position
  MaxPosition.x = ScreenWidth / (FONT_WIDTH + 1);
  MaxPosition.y = ScreenHeight / FONT_HEIGHT;

  return RETURN_SUCCESS;
}
