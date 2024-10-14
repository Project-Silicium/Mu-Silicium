#include <Library/DevicePrePiLib.h>

VOID
DeviceInitialize ()
{
  // Set Stack Pointer to EL1
  //__asm__ ("msr SPSel, #1");

  // Placing anything here makes the Nintendo Switch Crash, Weird.
}
