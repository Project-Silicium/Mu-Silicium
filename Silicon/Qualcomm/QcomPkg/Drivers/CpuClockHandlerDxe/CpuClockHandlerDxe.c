// TODO: Implement Settings Handler here!

#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include <Protocol/EFIClock.h>

EFI_STATUS
EFIAPI
SetCpuFreq (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS          Status            = EFI_SUCCESS;
  EFI_CLOCK_PROTOCOL *mClockProtocol    = NULL;
  UINT32              ClusterCount      = FixedPcdGet32 (PcdClusterCount);
  BOOLEAN             EnableL3Overclock = FALSE;

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

  // Enable L3 Cache Overclock
  if (FixedPcdGet32 (PcdSmbiosLevel3CacheSize) != 0) {
    EnableL3Overclock = TRUE;
  } else {
    EnableL3Overclock = FALSE;
  }

  // Set Dummy Variables
  UINT32 MaxPerfLevel[ClusterCount + EnableL3Overclock];
  UINT32 MinPerfLevel[ClusterCount + EnableL3Overclock];

  // Clear Dummy Variables
  SetMem (&MaxPerfLevel, sizeof (MaxPerfLevel), 0);
  SetMem (&MinPerfLevel, sizeof (MinPerfLevel), 0);

  // Get Perf Levels
  for (UINT8 i = 0; i < ClusterCount + EnableL3Overclock; i++) {
    // Get Max Perf Level
    Status = mClockProtocol->GetMaxPerfLevel (mClockProtocol, i, &MaxPerfLevel[i]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to get Max CPU Perf Level! Status = %r\n", Status));
      return Status;
    }

    // Get Low Perf Level
    Status = mClockProtocol->GetMinPerfLevel (mClockProtocol, i, &MinPerfLevel[i]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to get Min CPU Perf Level! Status = %r\n", Status));
      return Status;
    }
  }

  // Set Max Freqs
  for (UINT8 i = 0; i < ClusterCount + EnableL3Overclock; i++) {
    for (UINT8 j = MinPerfLevel[i]; j < MaxPerfLevel[i] + 1; j++) {
      UINT32 CpuFreq;
      UINT32 RequiredVoltage;

      // Get Perf Level Freq
      Status = mClockProtocol->GetCpuPerfLevelFrequency (mClockProtocol, i, j, &CpuFreq, &RequiredVoltage);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Get CPU Cluster %u Freq for Perf Level %u! Status = %r\n", i, j, Status));
        continue;
      }

      // Set defined Freq
      if (j == MaxPerfLevel[i]) {
        UINT32 DummyFreq;

        // Set Perf Level for CPU Cluster
        Status = mClockProtocol->SetCpuPerfLevel (mClockProtocol, i, j, &DummyFreq);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to set Perf Level %u for CPU Cluster %u! Status = %r\n", j, i, Status));
          ASSERT_EFI_ERROR (Status);
        }

        // Use Dummy Freq
        if (!CpuFreq) {
          CpuFreq = DummyFreq;
        }

        // Print New Clock Hz
        DEBUG ((EFI_D_WARN, "CPU Cluster %u now runs at %llu Hz\n", i, CpuFreq));
      }
    }
  }

  return EFI_SUCCESS;
}
