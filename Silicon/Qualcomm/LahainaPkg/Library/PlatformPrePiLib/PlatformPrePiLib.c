#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/PcdLib.h>
#include <Library/ConfigurationMapHelperLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  EFI_STATUS Status;
  UINT32     EarlyInitCoreCnt;

  // Get Early Cores Count
  Status = LocateConfigurationMapUINT32ByName("EarlyInitCoreCnt", &EarlyInitCoreCnt);
  if (!EFI_ERROR (Status)) {
    // Wake Up all Cores
    for (UINTN i = 0; i < EarlyInitCoreCnt; i++) {
      MmioWrite32 (GICR_WAKER_CPU(i), (MmioRead32 (GICR_WAKER_CPU(i)) & ~GIC_WAKER_PROCESSORSLEEP));
    }
  }
}
