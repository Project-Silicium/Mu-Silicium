#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  // Disable WatchDog Timer
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_ENABLE_OFFSET, 0x0);
}
