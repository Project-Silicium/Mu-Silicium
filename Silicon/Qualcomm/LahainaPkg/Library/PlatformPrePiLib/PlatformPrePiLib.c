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
  if (EFI_ERROR (Status)) {
    return;
  }

  // Wake Up all Cores
  for (UINTN i = 0; i < EarlyInitCoreCnt; i++) {
    UINT32 Value;

    // Modify current GIC Waker
    Value  = MmioRead32 (GICR_WAKER_CPU (i));
    Value &= ~GIC_WAKER_PROCESSORSLEEP;

    // Write new GIC Waker
    MmioWrite32 (GICR_WAKER_CPU (i), Value);
  }
}

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x820, 0x821, 0xC21 };

  // Wake Up Cores
  WakeUpCores ();

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
