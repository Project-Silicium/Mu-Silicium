#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
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
  Status = gBS->LocateProtocol (&gEfiClockProtocolGuid, NULL, (VOID *)&mClockProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Clock Protocol! Status = %r\n", Status));
    return Status;
  }

  // Check for Valid Version
  if (mClockProtocol->Version < 0x10006) {
    DEBUG ((EFI_D_ERROR, "Clock Version 0x%x does not Support CPU Overclock!\n", mClockProtocol->Version));
    return EFI_UNSUPPORTED;
  }

  // Set Max Freq for all CPU Clusters
  for (UINT32 i = 0; i < FixedPcdGet32 (PcdClusterCount) + FixedPcdGetBool (PcdHasLevel3Cache); i++) {
    UINT32 PerfLevel;
    UINT32 HzFreq;

    // Get Max Perf Level of CPU Clusters
    Status = mClockProtocol->GetMaxPerfLevel (mClockProtocol, i, &PerfLevel);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Max Perf Level of CPU Cluster %u!\n", i));
      ASSERT_EFI_ERROR (Status);
    }

    // Set Max Perf Level for CPU Clusters
    Status = mClockProtocol->SetCpuPerfLevel (mClockProtocol, i, PerfLevel, &HzFreq);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Set Max Perf Level of CPU Cluster %u!\n", i));
      ASSERT_EFI_ERROR (Status);
    }

    DEBUG ((EFI_D_WARN, "CPU Cluster %u Now runs at %u Hz.\n", i, HzFreq));
  }

  return EFI_SUCCESS;
}
