#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/PcdLib.h>
#include <Library/ConfigurationMapHelperLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  // Initialize GIC
  UINT32 EarlyInitCoreCnt = 2;
  LocateConfigurationMapUINT32ByName("EarlyInitCoreCnt", &EarlyInitCoreCnt);

  for (int i = 0; i < EarlyInitCoreCnt; i++)
  {
    // Wake up redistributor for CPU i
    MmioWrite32(
        GICR_WAKER_CPU(i),
        (MmioRead32(GICR_WAKER_CPU(i)) & ~GIC_WAKER_PROCESSORSLEEP));
  }
}