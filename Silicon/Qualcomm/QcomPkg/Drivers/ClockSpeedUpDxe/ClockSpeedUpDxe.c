#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIClock.h>

EFI_STATUS
EFIAPI
SetMaxFreq (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS          Status;
  EFI_CLOCK_PROTOCOL *mClockProtocol;

  // Locate Clock Protocol
  Status = gBS->LocateProtocol(&gEfiClockProtocolGuid, NULL, (VOID *)&mClockProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Clock Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Check if Max Freq PCD is TRUE
  if (FixedPcdGetBool(PcdEnableMaxFreq) == TRUE) {
    // Set Max Freq for all CPU Cores
    for (UINT32 i = 0; i < FixedPcdGet32(PcdCoreCount) + FixedPcdGetBool(PcdHasLevel3Cache); i++) {
      UINT32 PerfLevel;
      UINT32 HzFreq;

      // Get Max Perf Level of CPU Cores
      Status = mClockProtocol->GetMaxPerfLevel(mClockProtocol, i, &PerfLevel);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Get Max Perf Level of CPU Core %d! Status = %r\n", i, Status));
        continue;
      }

      // Set Max Perf Level for CPU Cores
      Status = mClockProtocol->SetCpuPerfLevel(mClockProtocol, i, PerfLevel, &HzFreq);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Set Max Perf Level of CPU Core %d! Status = %r\n", i, Status));
        continue;
      }

      DEBUG ((EFI_D_WARN, "CPU Core %d Now runs at %d Hz.\n", i, HzFreq));
    }
  } else {
    DEBUG ((EFI_D_WARN, "Max Freq PCD is Disabled.\n"));
  }

exit:
  return Status;
}