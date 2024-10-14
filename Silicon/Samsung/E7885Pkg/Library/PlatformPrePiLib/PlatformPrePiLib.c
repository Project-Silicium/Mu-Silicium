#include <Library/PlatformPrePiLib.h>
#include <Library/IoLib.h>

#define HW_SW_TRIG_CONTROL 0x70

VOID
PlatformInitialize ()
{
  // Enable Frame Buffer Writes
  MmioWrite32 (0x14860000 + HW_SW_TRIG_CONTROL, 0x1281);
}
