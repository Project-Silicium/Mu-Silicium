#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/ArmSmmuDetachLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = {
    0x0820,
    0x0821,
    0x0C21
  };

  // Disable WatchDog Timer
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_ENABLE_OFFSET, 0x0);

  ArmSmmuDetach(MdpStreams, ARRAY_SIZE(MdpStreams));
}
