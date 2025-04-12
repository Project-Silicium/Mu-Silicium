#include <Library/BaseMemoryLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>

#define HW_SW_TRIG_CONTROL 0x30
#define WATCHDOG_ENABLE    ((1 << 5) | 1)

VOID
EnableFrameBufferWrites ()
{
  EFI_STATUS                      Status           = EFI_SUCCESS;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DrmDeconFRegion  = {0};
  CHAR8                           DrmDeconName[12] = "";

  // Clear Message
  ZeroMem (DrmDeconName, 12);

  // Enable Frame Buffer Writes
  for (UINT8 i = 0; i < 11; i++) {
    // Append Number
    AsciiSPrint (DrmDeconName, sizeof (DrmDeconName), "DRM-Decon-%u", i);

    // Locate DRM Decon Memory Region
    Status = LocateMemoryMapAreaByName (DrmDeconName, &DrmDeconFRegion);
    if (EFI_ERROR (Status)) {
      // Exit Loop
      continue;
    }

    // Configure DRM Decon
    MmioWrite32 (DrmDeconFRegion.Address + HW_SW_TRIG_CONTROL, 0x1281);
  }
}

VOID
DisableWatchdogTimer ()
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX WatchdogTimerFRegion;
  UINT32                          Value;

  // Locate Watchdog Timner Memory Region
  Status = LocateMemoryMapAreaByName ("Watchdog Timer", &WatchdogTimerFRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get current Config
  Value = MmioRead32 (WatchdogTimerFRegion.Address);

  // Update Config
  Value &= ~WATCHDOG_ENABLE;

  // Write new Config
  MmioWrite32 (WatchdogTimerFRegion.Address, Value);
}

VOID
PlatformInitialize ()
{
  // Allow Frame Buffer Writes
  EnableFrameBufferWrites ();

  // Disable Watchdog Timer
  DisableWatchdogTimer ();
}
