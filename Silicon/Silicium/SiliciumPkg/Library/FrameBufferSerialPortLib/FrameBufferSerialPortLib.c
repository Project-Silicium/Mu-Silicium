#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>

#include "Fonts/5x12.h"
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
  UINTN Stride = FrameBufferData.Width - FrameBufferData.FontData.Width * FrameBufferData.FontData.Scale;

  for (UINT8 i = 0; i < 2; i++) {
    // Set Data Variable
    Data = Glyph[i];

    for (UINT8 j = 0; j < FrameBufferData.FontData.Height / 2; j++) {
      Temp = Data;

      for (UINT8 k = 0; k < FrameBufferData.FontData.Scale; k++) {
        Data = Temp;

        for (UINT8 l = 0; l < FrameBufferData.FontData.Width; l++) {
          if (Data & 1) {
            for (UINT8 m = 0; m < FrameBufferData.FontData.Scale; m++) {
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
            for (UINT8 m = 0; m < FrameBufferData.FontData.Scale; m++) {
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

  // Check for "\r"
  if (Buffer == '\r') {
    // Reset X Position
    FrameBufferData.CurrentPosition->XPos = 0;
    return;
  }

  // Check for "\n" and Max Position
  if (Buffer == '\n' || FrameBufferData.CurrentPosition->XPos >= FrameBufferData.MaxPosition.XPos - FrameBufferData.FontData.Scale) {
    // Wait
    MicroSecondDelay (FrameBufferData.PrintDelay);

    // Increase Y Position
    FrameBufferData.CurrentPosition->YPos += FrameBufferData.FontData.Scale;

    // Reset X Position
    FrameBufferData.CurrentPosition->XPos = 0;

    if (FrameBufferData.CurrentPosition->YPos >= FrameBufferData.MaxPosition.YPos) {
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
    return;
  }

  // Save some Space
  if (FrameBufferData.CurrentPosition->XPos == 0 && (UINT8)Buffer == ' ') {
    return;
  }

  // Set Debug Message Configuration
  Pixels  = (VOID *)FrameBufferData.MemoryRegion.Address;
  Pixels += FrameBufferData.CurrentPosition->YPos * (FrameBufferData.BytesPerPixel * FrameBufferData.FontData.Height * FrameBufferData.Width);
  Pixels += (FrameBufferData.CurrentPosition->XPos * FrameBufferData.FontData.Scale) * (FrameBufferData.BytesPerPixel * (FrameBufferData.FontData.Width + 1));

  // Draw Debug Message
  DrawDebugMessage (Pixels, Font5x12 + (Buffer - 32) * 2);

  // Increase X Position
  FrameBufferData.CurrentPosition->XPos++;
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

  // Get Secondary Frame Buffer Offset
  UINT64 FrameBufferOffset = FixedPcdGet64 (PcdSecondaryFrameBufferOffset);

  // Get Frame Buffer Infos
  if (FrameBufferOffset && FixedPcdGetBool (PcdSecondaryFrameBufferDebug)) {
    // Set Screen Infos
    FrameBufferData.Width         = FixedPcdGet32 (PcdSecondaryFrameBufferWidth);
    FrameBufferData.Height        = FixedPcdGet32 (PcdSecondaryFrameBufferHeight);
    FrameBufferData.BytesPerPixel = FixedPcdGet8  (PcdSecondaryFrameBufferColorDepth) / 8;
  } else {
    // Set Screen Infos
    FrameBufferData.Width         = FixedPcdGet32 (PcdPrimaryFrameBufferWidth);
    FrameBufferData.Height        = FixedPcdGet32 (PcdPrimaryFrameBufferHeight);
    FrameBufferData.BytesPerPixel = FixedPcdGet8  (PcdPrimaryFrameBufferColorDepth) / 8;
  }

  // Update "Display Reserved" Memory Region
  FrameBufferData.MemoryRegion.Address += FrameBufferOffset;
  FrameBufferData.MemoryRegion.Length   = FrameBufferData.Width * FrameBufferData.Height * FrameBufferData.BytesPerPixel;

  // Set Default Print Delay
  FrameBufferData.PrintDelay = FixedPcdGet32 (PcdDebugFrameBufferDelay);

  // Set Font Data (TODO: Add New Dynamic Scale Method)
  FrameBufferData.FontData.Width  = 5;
  FrameBufferData.FontData.Height = 12;
  FrameBufferData.FontData.Scale  = 1;

  // Set Total Position
  FrameBufferData.CurrentPosition = (EFI_FRAME_BUFFER_POSITION *)(FrameBufferData.MemoryRegion.Address + (FrameBufferData.Width * FrameBufferData.Height * FrameBufferData.BytesPerPixel));

  // Calculate Max Position
  FrameBufferData.MaxPosition.XPos = FrameBufferData.Width / FrameBufferData.FontData.Width;
  FrameBufferData.MaxPosition.YPos = FrameBufferData.Height / FrameBufferData.FontData.Height;

  return EFI_SUCCESS;
}
