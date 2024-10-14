#include <Library/ArmLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include <Resources/font5x12.h>

#include "FrameBufferSerialPortLib.h"

ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegion;

FBCON_POSITION *CurrentPosition;
FBCON_POSITION  MaxPosition;
FBCON_COLOR     FrameBufferColor;

UINTN ScreenWidth      = FixedPcdGet32 (PcdMipiFrameBufferWidth);
UINTN ScreenHeight     = FixedPcdGet32 (PcdMipiFrameBufferHeight);
UINTN ScreenColorDepth = FixedPcdGet32 (PcdMipiFrameBufferColorDepth);
UINTN PrintDelay       = FixedPcdGet32 (PcdMipiFrameBufferDelay);

VOID
DrawDebugMessage (
  CHAR8 *Pixels,
  UINTN  Stride,
  UINTN  Bpp,
  UINTN *Glyph)
{
  CHAR8  *BackgroundPixels = Pixels;
  UINTN   Data             = 0;
  UINTN   Temp             = 0;
  UINT32  ForegroundColor  = FrameBufferColor.Foreground;
  UINT32  BackgroundColor  = FrameBufferColor.Background;

  Stride  -= FONT_WIDTH * SCALE_FACTOR;

  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        for (UINTN j = 0; j < SCALE_FACTOR; j++) {
          BackgroundColor = FrameBufferColor.Background;

          for (UINTN k = 0; k < Bpp; k++) {
            *BackgroundPixels = (UINT8)BackgroundColor;
            BackgroundColor   = BackgroundColor >> 8;
            BackgroundPixels++;
          }
        }
      }

      BackgroundPixels += (Stride * Bpp);
    }
  }

  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        for (UINTN j = 0; j < SCALE_FACTOR; j++) {
          BackgroundColor = FrameBufferColor.Background;

          for (UINTN k = 0; k < Bpp; k++) {
            *BackgroundPixels = (UINT8)BackgroundColor;
            BackgroundColor   = BackgroundColor >> 8;
            BackgroundPixels++;
          }
        }
      }

      BackgroundPixels += (Stride * Bpp);
    }
  }

  Data = Glyph[0];

  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    Temp = Data;

    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      Data = Temp;

      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        if (Data & 1) {
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            ForegroundColor = FrameBufferColor.Foreground;

            for (UINTN k = 0; k < Bpp; k++) {
              *Pixels  = (UINT8)ForegroundColor;
              ForegroundColor = ForegroundColor >> 8;
              Pixels++;
            }
          }
        } else {
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            Pixels = Pixels + Bpp;
          }
        }

        Data >>= 1;
      }

      Pixels += (Stride * Bpp);
    }
  }

  Data = Glyph[1];

  for (UINTN y = 0; y < FONT_HEIGHT / 2; y++) {
    Temp = Data;

    for (UINTN i = 0; i < SCALE_FACTOR; i++) {
      Data = Temp;

      for (UINTN x = 0; x < FONT_WIDTH; x++) {
        if (Data & 1) {
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            ForegroundColor = FrameBufferColor.Foreground;

            for (UINTN k = 0; k < Bpp; k++) {
              *Pixels  = (UINT8)ForegroundColor;
              ForegroundColor = ForegroundColor >> 8;
              Pixels++;
            }
          }
        } else {
          for (UINTN j = 0; j < SCALE_FACTOR; j++) {
            Pixels = Pixels + Bpp;
          }
        }

        Data >>= 1;
      }

      Pixels += (Stride * Bpp);
    }
  }
}

VOID
WriteFrameBuffer (CHAR8 Buffer)
{
  CHAR8  *Pixels            = NULL;
  BOOLEAN InterruptsEnabled = ArmGetInterruptState ();

Print:
  if ((UINT8)Buffer > 127) {
    return;
  }

  if ((UINT8)Buffer < 32) {
    if (Buffer == '\n') {
      goto newline;
    } else if (Buffer == '\r') {
      CurrentPosition->x = 0;
      return;
    } else {
      return;
    }
  }

  // Save some space
  if (CurrentPosition->x == 0 && (UINT8)Buffer == ' ') { return; }

  // Disable Interrupts
  if (InterruptsEnabled) { ArmDisableInterrupts (); }

  // Set Debug Message Configuration
  Pixels  = (VOID *)DisplayMemoryRegion.Address;
  Pixels += CurrentPosition->y * ((ScreenColorDepth / 8) * FONT_HEIGHT * ScreenWidth);
  Pixels += CurrentPosition->x * SCALE_FACTOR * ((ScreenColorDepth / 8) * (FONT_WIDTH + 1));

  // Draw Debug Message on Frame Buffer
  DrawDebugMessage (Pixels, ScreenWidth, (ScreenColorDepth / 8), font5x12 + (Buffer - 32) * 2);

  CurrentPosition->x++;

  if (CurrentPosition->x >= (INT32)(MaxPosition.x / SCALE_FACTOR)) { goto newline; }

  // Enable Interrupts
  if (InterruptsEnabled) { ArmEnableInterrupts (); }

  return;

newline:
  MicroSecondDelay (PrintDelay);

  CurrentPosition->y += SCALE_FACTOR;
  CurrentPosition->x  = 0;

  if (CurrentPosition->y >= MaxPosition.y - SCALE_FACTOR) {
    // Reset Frame Buffer
    ZeroMem ((VOID *)DisplayMemoryRegion.Address, DisplayMemoryRegion.Length);

    // Flush Frame Buffer
    WriteBackInvalidateDataCacheRange ((VOID *)DisplayMemoryRegion.Address, (ScreenWidth * ScreenHeight * (ScreenColorDepth / 8)));

    // Set CurrentPosition Height to First Line
    CurrentPosition->y = -1;

    // Enable Interrupts
    if (InterruptsEnabled) { ArmEnableInterrupts (); }

    goto Print;
  } else {
    // Flush FrameBuffer
    WriteBackInvalidateDataCacheRange ((VOID *)DisplayMemoryRegion.Address, (ScreenWidth * ScreenHeight * (ScreenColorDepth / 8)));

    // Enable Interrupts
    if (InterruptsEnabled) { ArmEnableInterrupts (); }
  }
}

UINTN
EFIAPI
SerialPortWrite (
  IN UINT8 *Buffer,
  IN UINTN  NumberOfBytes)
{
  UINT8 *CONST Final          = &Buffer[NumberOfBytes];
  UINTN        InterruptState = ArmGetInterruptState ();

  // Disable Interrupts
  if (InterruptState) { ArmDisableInterrupts (); }

  // Write Debug Message to Frame Buffer
  while (Buffer < Final) { WriteFrameBuffer (*Buffer++); }

  // Enable Interrupts
  if (InterruptState) { ArmEnableInterrupts (); }

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
SerialPortPoll () { return FALSE; }

RETURN_STATUS
EFIAPI
SerialPortSetControl (IN UINT32 Control) { return RETURN_UNSUPPORTED; }

RETURN_STATUS
EFIAPI
SerialPortGetControl (OUT UINT32 *Control) { return RETURN_UNSUPPORTED; }

RETURN_STATUS
EFIAPI
SerialPortSetAttributes (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits)
{
  return RETURN_UNSUPPORTED;
}

UINTN
SerialPortFlush () { return 0; }

VOID
EnableSynchronousSerialPortIO () {}

RETURN_STATUS
EFIAPI
SerialPortInitialize ()
{
  EFI_STATUS Status;

  // Get Frame Buffer Base Address
  Status = LocateMemoryMapAreaByName ("Display Reserved", &DisplayMemoryRegion);
  if (EFI_ERROR (Status)) { 
    // Get Secondary Frame Buffer Base Address
    Status = LocateMemoryMapAreaByName ("Display Reserved-2", &DisplayMemoryRegion);
    if (EFI_ERROR (Status)) { return RETURN_UNSUPPORTED; }
  }

  // Set Total Position
  CurrentPosition = (FBCON_POSITION *)(DisplayMemoryRegion.Address + (ScreenWidth * ScreenHeight * ScreenColorDepth / 8));

  // Calculate Max Position.
  MaxPosition.x = ScreenWidth / (FONT_WIDTH + 1);
  MaxPosition.y = ScreenHeight / FONT_HEIGHT;

  // Set Frame Buffer Colors
  FrameBufferColor.Foreground = 0xFFFFFFFF; // White
  FrameBufferColor.Background = 0xFF000000; // Black

  return RETURN_SUCCESS;
}
