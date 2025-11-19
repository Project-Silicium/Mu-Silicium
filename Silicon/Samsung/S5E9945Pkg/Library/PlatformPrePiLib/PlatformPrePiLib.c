#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#include "PlatformRegisters.h"

VOID
EnableFrameBufferWrites ()
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX DrmDeconRegion;

  // Locate DRM Decon Memory Region
  Status = LocateMemoryMapAreaByName ("DRM Decon", &DrmDeconRegion);
  if (!EFI_ERROR (Status)) {
    // Configure DRM Decon
    for (UINT64 Length = 0; Length < DrmDeconRegion.Length; Length += 0x1000) {
      MmioWrite32 (DrmDeconRegion.Address + Length + HW_SW_TRIG_CONTROL, 0x1281);
    }
  }
}

VOID
DisableWatchdogTimer ()
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX WatchdogTimerRegion;
  UINT32                          Value;

  // Locate Watchdog Timner Memory Region
  Status = LocateMemoryMapAreaByName ("Watchdog Timer", &WatchdogTimerRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get current Config
  Value = MmioRead32 (WatchdogTimerRegion.Address);

  // Update Config
  Value &= ~WATCHDOG_ENABLE;

  // Write new Config
  MmioWrite32 (WatchdogTimerRegion.Address, Value);
}

VOID
PlatformInitialize ()
{
  // Allow Frame Buffer Writes
  EnableFrameBufferWrites ();

  // Disable Watchdog Timer
  DisableWatchdogTimer ();
}
