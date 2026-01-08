#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ArmSmmuDetachLib.h>

#include "PlatformRegisters.h"

VOID
DisableWatchDogTimer ()
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX WDogRegion;

  // Locate "APSS_WDT_TMR1" Memory Region
  Status = LocateMemoryMapAreaByName ("APSS_WDT_TMR1", &WDogRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable WatchDog Timer
  MmioWrite32 (WDogRegion.Address + APSS_WDT_ENABLE_OFFSET, 0x0);
}

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x420, 0x421 };

  // Disable WatchDog Timer
  DisableWatchDogTimer ();

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
