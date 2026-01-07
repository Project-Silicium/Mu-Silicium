#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/ConfigurationMapHelperLib.h>
#include <Library/ArmSmmuDetachLib.h>
#include <Library/PcdLib.h>

#include "PlatformRegisters.h"

VOID
WakeUpCores ()
{
  EFI_STATUS Status;
  UINT32     EarlyInitCoreCnt;

  // Get Early Cores Count
  Status = LocateConfigurationMapUINT32ByName ("EarlyInitCoreCnt", &EarlyInitCoreCnt);
  if (!EFI_ERROR (Status)) {
    // Wake Up all Cores
    for (UINTN i = 0; i < EarlyInitCoreCnt; i++) {
      MmioWrite32 (GICR_WAKER_CPU (i), (MmioRead32 (GICR_WAKER_CPU (i)) & ~GIC_WAKER_PROCESSORSLEEP));
    }
  }
}

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x820, 0x402 };

  // Wake Up Cores
  WakeUpCores ();

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
