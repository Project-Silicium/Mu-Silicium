#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include "FrameBuffer.h"

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

  // Set Stride Value
  UINTN Stride = FrameBufferData.Width - 5 * FrameBufferData.FontScale;

  for (UINT8 i = 0; i < 2; i++) {
    // Set Data Variable
    Data = Glyph[i];

    for (UINT8 j = 0; j < 12 / 2; j++) {
      Temp = Data;

      for (UINT8 k = 0; k < FrameBufferData.FontScale; k++) {
        Data = Temp;

        for (UINT8 l = 0; l < 5; l++) {
          if (Data & 1) {
            for (UINT8 m = 0; m < FrameBufferData.FontScale; m++) {
              // Set Text Color
              Color = 0xFFFFFFFF;

              for (UINT8 n = 0; n < FrameBufferData.BytesPerPixel; n++) {
                // Draw Text Char
                *Pixels = (UINT8)Color;

                // Shift Color
                Color >>= 8;

                // Update Position
                Pixels++;
              }
            }
          } else {
            for (UINT8 m = 0; m < FrameBufferData.FontScale; m++) {
              // Update Position
              Pixels += FrameBufferData.BytesPerPixel;
            }
          }

          // Shift Position
          Data >>= 1;
        }

        // Update Position
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
    // Check for "\n"
    if (Buffer == '\n' || FrameBufferData.CurrentPosition->XPos >= FrameBufferData.MaxPosition.XPos - FrameBufferData.FontScale) {
      // Create New Line
      goto NewLine;
    }

    // Check for "\r"
    if (Buffer == '\r') {
      // Reset X Position
      FrameBufferData.CurrentPosition->XPos = 0;
      return;
    }

    return;
  }

  // Save some Space
  if (FrameBufferData.CurrentPosition->XPos == 0 && (UINT8)Buffer == ' ') {
    return;
  }

  // Set Debug Message Configuration
  Pixels  = (VOID *)FrameBufferData.MemoryRegion.Address;
  Pixels += FrameBufferData.CurrentPosition->YPos * (FrameBufferData.BytesPerPixel * 12 * FrameBufferData.Width);
  Pixels += FrameBufferData.CurrentPosition->XPos * FrameBufferData.FontScale * (FrameBufferData.BytesPerPixel * 6);

  // Draw Debug Message
  DrawDebugMessage (Pixels, Font + (Buffer - 32) * 2);

  // Increase X Position
  FrameBufferData.CurrentPosition->XPos++;

  // Check Max Position
  if (FrameBufferData.CurrentPosition->XPos < FrameBufferData.MaxPosition.XPos / FrameBufferData.FontScale) {
    return;
  }

NewLine:
  // Wait
  MicroSecondDelay (FrameBufferData.PrintDelay);

  // Increase Y Position
  FrameBufferData.CurrentPosition->YPos += FrameBufferData.FontScale;

  // Reset X Position
  FrameBufferData.CurrentPosition->XPos = 0;

  if (FrameBufferData.CurrentPosition->YPos >= FrameBufferData.MaxPosition.YPos - FrameBufferData.FontScale) {
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

  // Set Default Print Delay
  FrameBufferData.PrintDelay = FixedPcdGet32 (PcdFrameBufferDelay);

  // Set Font Scale (TODO: Add Dynamic Scale Method)
  FrameBufferData.FontScale = 1;

  // Set Total Position
  FrameBufferData.CurrentPosition = (EFI_FRAME_BUFFER_POSITION *)(FrameBufferData.MemoryRegion.Address + FrameBufferData.MemoryRegion.Length);

  // Calculate Max Position
  FrameBufferData.MaxPosition.XPos = FrameBufferData.Width / 6;
  FrameBufferData.MaxPosition.YPos = (FrameBufferData.Height - 1) / 12;

  return EFI_SUCCESS;
}
