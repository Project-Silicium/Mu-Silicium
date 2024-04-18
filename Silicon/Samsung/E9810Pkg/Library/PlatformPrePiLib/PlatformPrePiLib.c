#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  // Enable Framebuffer
  MmioWrite32 (DECON_F_BASE + HW_SW_TRIG_CONTROL, 0x1281);
}