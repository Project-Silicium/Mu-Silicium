#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#include "PlatformRegisters.h"

VOID
PlatformInitialize ()
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX DeconRegion;

  // Locate Decon-F Memory Region
  Status = LocateMemoryMapAreaByName ("Decon", &DeconRegion);
  if (!EFI_ERROR (Status)) {
    // Configure Decon
    MmioWrite32 (DeconRegion.Address + HW_SW_TRIG_CONTROL, 0x1281);
  }
}
