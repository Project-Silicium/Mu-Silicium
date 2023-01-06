#include <PiPei.h>
#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include "PlatformUtils.h"

BOOLEAN IsLinuxBootRequested(VOID)
{
  return (MmioRead32(LID0_GPIO38_STATUS_ADDR) & 1) == 0;
}

VOID PlatformInitialize(VOID) {}