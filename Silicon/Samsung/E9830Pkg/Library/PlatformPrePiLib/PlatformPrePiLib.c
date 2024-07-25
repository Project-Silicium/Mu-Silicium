#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/DevicePrePiLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
#if DEVICE_PRESERVES_FDT == 0
  // Enable Framebuffer
  MmioWrite32 (DECON_F_BASE + HW_SW_TRIG_CONTROL, 0x1281);
#endif

  // Run Device Specific Code
  DeviceInitialize ();
}
