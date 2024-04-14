#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/PcdLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  // Initialize GIC
  // Wake up redistributor for CPU 0
  MmioWrite32(
      GICR_WAKER_CURRENT_CPU,
      (MmioRead32(GICR_WAKER_CURRENT_CPU) & ~GIC_WAKER_PROCESSORSLEEP));

  // Wake up redistributor for CPU 1
  MmioWrite32(
      GICR_WAKER_CPU(1),
      (MmioRead32(GICR_WAKER_CPU(1)) & ~GIC_WAKER_PROCESSORSLEEP));
}