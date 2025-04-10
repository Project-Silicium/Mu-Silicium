#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#define HW_SW_TRIG_CONTROL 0x70

VOID
PlatformInitialize ()
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DeconFRegion;

  // Locate Decon-F Memory Region
  Status = LocateMemoryMapAreaByName ("Decon-F", &DeconFRegion);
  if (!EFI_ERROR (Status)) {
    // Enable Frame Buffer Writes
    MmioWrite32 (DeconFRegion.Address + HW_SW_TRIG_CONTROL, 0x1281);
  }
}
