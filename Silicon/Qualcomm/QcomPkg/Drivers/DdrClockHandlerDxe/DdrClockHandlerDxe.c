// TODO: Implement Settings Handler here!

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Protocol/EFIDDRGetConfig.h>

EFI_STATUS
EFIAPI
SetDdrFreq (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS               Status;
  EFI_DDRGETINFO_PROTOCOL *mDdrConfigProtocol;
  ddr_freq_plan_entry      FreqTable;

  // Locate DDR Config Protocol
  Status = gBS->LocateProtocol (&gEfiDDRGetInfoProtocolGuid, NULL, (VOID *)&mDdrConfigProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate DDR Config Protocol! Status = %r\n", Status));
    return Status;
  }

  // Check for Valid Version
  if (mDdrConfigProtocol->Revision < 0x30000) {
    DEBUG ((EFI_D_ERROR, "DDR Config Version 0x%x does not Support DDR Freq Changes!\n", mDdrConfigProtocol->Revision));
    return EFI_UNSUPPORTED;
  }

  // Get DDR Freq Table
  Status = mDdrConfigProtocol->GetDDRFreqTable (mDdrConfigProtocol, &FreqTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get DDR Freq Table! Status = %r\n", Status));
    return Status;
  }

  // Set Max Freq
  for (UINT8 i = 0; i < FreqTable.num_ddr_freqs; i++) {
    // Check Freq
    if (!FreqTable.ddr_freq[i].freq_khz || !FreqTable.ddr_freq[i].enable) {
      continue;
    }

    // Set defined Freq
    if (i == FreqTable.num_ddr_freqs - 1) {
      UINT32 DdrFreq;

      // Set new DDR Freq
      Status = mDdrConfigProtocol->SetDDRFreq (mDdrConfigProtocol, FreqTable.ddr_freq[i].freq_khz);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Set DDR Freq %llu! Status = %r\n", FreqTable.ddr_freq[i].freq_khz * 1000, Status));
        return Status;
      }

      // Get New DDR Freq
      Status = mDdrConfigProtocol->GetDDRFreq (mDdrConfigProtocol, &DdrFreq);
      if (EFI_ERROR (Status)) {
        DdrFreq = FreqTable.ddr_freq[i].freq_khz * 1000;
      } else {
        DdrFreq = DdrFreq * 1000 * 1000;
      }

      DEBUG ((EFI_D_WARN, "DDR Memory now runs at %llu Hz\n", DdrFreq));
    }
  }

  return EFI_SUCCESS;
}
