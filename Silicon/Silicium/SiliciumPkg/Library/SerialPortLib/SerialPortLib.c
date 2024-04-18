#include <Library/ArmLib.h>
#include <Library/SerialPortLib.h>

#include "FrameBuffer.h"
#include "PStore.h"

STATIC BOOLEAN ScreenLogging;
STATIC BOOLEAN MemoryLogging;

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
  if (ScreenLogging) { while (Buffer < Final) { WriteFrameBuffer (*Buffer++); } }

  // Write Debug Message in Memory
  if (MemoryLogging) { for (UINTN i = 0; i < NumberOfBytes; i++) { WriteMemory (Buffer[i]); } }

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
  ScreenLogging = FrameBufferLogInit ();
  MemoryLogging = MemoryLogInit ();

  return RETURN_SUCCESS;
}
