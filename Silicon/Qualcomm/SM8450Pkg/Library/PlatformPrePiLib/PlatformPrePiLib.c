#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/PcdLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  // Initialize GIC
  MmioWrite32 (GICR_WAKER_CURRENT_CPU, (MmioRead32 (GICR_WAKER_CURRENT_CPU) & ~GIC_WAKER_PROCESSORSLEEP));
}
