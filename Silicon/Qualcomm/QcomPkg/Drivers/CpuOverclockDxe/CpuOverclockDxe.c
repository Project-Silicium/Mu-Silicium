#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

//#include <Protocol/EfiSettingsHandler.h>
#include <Protocol/EFIClock.h>

//
// Global Variables
//
//STATIC UINT8 FreqIndex = 0;

/*
VOID
UpdateSettingsHandlerInfo (
  IN EFI_SETTINGS_HANDLER_PROTOCOL *mSettingsHandlerProtocol,
  IN UINT64                         Freq,
  IN BOOLEAN                        IsTerminator)
{
  EFI_STATUS             Status;
  EFI_SETTING_ATTRIBUTES OverclockAttributes;

  // Get "CpuOverclockFreq" Setting Attributes
  Status = mSettingsHandlerProtocol->GetSettingAttributes ("CpuOverclockFreq", &OverclockAttributes);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get 'CpuOverclockFreq' Setting Attributes! Status = %r\n", Status));
    return;
  }

  // Add CPU Freq
  OverclockAttributes.PossibleValues[FreqIndex] = Freq;

  // Set "CpuOverclockFreq" Setting Attributes
  Status = mSettingsHandlerProtocol->SetSettingAttributes ("CpuOverclockFreq", OverclockAttributes);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set 'CpuOverclockFreq' Setting Attributes! Status = %r\n", Status));
    return;
  }

  // Increase Index
  FreqIndex++;
}
*/

EFI_STATUS
EFIAPI
SetCpuFreq (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                     Status                   = EFI_SUCCESS;
//EFI_SETTINGS_HANDLER_PROTOCOL *mSettingsHandlerProtocol = NULL;
  EFI_CLOCK_PROTOCOL            *mClockProtocol           = NULL;
//UINT64                         EnableOverclock          = 0;
//UINT64                         CpuOverclockFreq         = 0;
  UINT32                         MaxPerfLevel             = 0;
  UINT32                         MinPerfLevel             = 0;
  BOOLEAN                        EnableL3Overclock        = FALSE;

  /*
  // Locate Settings Handler Protocol
  Status = gBS->LocateProtocol (&gEfiSettingsHandlerProtocolGuid, NULL, (VOID *)&mSettingsHandlerProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Settings Handler Protocol! Status = %r\n", Status));
    return Status;
  }

  // Get "CpuOverclock" Setting Value
  Status = mSettingsHandlerProtocol->GetSetting ("CpuOverclock", &EnableOverclock);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get 'CpuOverclock' Setting Value! Status = %r\n", Status));
    return Status;
  }

  // Get "CpuOverclockFreq" Setting Value
  Status = mSettingsHandlerProtocol->GetSetting ("CpuOverclockFreq", &CpuOverclockFreq);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get 'CpuOverclockFreq' Setting Value! Status = %r\n", Status));
    return Status;
  }
  */

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

  // Get Max Perf Level
  Status = mClockProtocol->GetMaxPerfLevel (mClockProtocol, FixedPcdGet32 (PcdClusterCount) - 1, &MaxPerfLevel);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get Max CPU Perf Level! Status = %r\n", Status));
    return Status;
  }

  // Get Low Perf Level
  Status = mClockProtocol->GetMinPerfLevel (mClockProtocol, FixedPcdGet32 (PcdClusterCount) - 1, &MinPerfLevel);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get Min CPU Perf Level! Status = %r\n", Status));
    return Status;
  }

  // Get each Freq
  for (UINT32 i = MinPerfLevel; i < MaxPerfLevel + 1; i++) {
    UINT32 CpuFreq;
    UINT32 RequiredVoltage;

    // Get Perf Level Freq
    Status = mClockProtocol->GetCpuPerfLevelFrequency (mClockProtocol, FixedPcdGet32 (PcdClusterCount) - 1, i, &CpuFreq, &RequiredVoltage);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get CPU Freq for Perf Level %u! Status = %r\n", i, Status));
      continue;
    }

    /*
    // Populate Settings Handler Info
    if (i == MaxPerfLevel + 1) {
      UpdateSettingsHandlerInfo (mSettingsHandlerProtocol, CpuFreq, TRUE);
    } else {
      UpdateSettingsHandlerInfo (mSettingsHandlerProtocol, CpuFreq, FALSE);
    }
    */

    // Compare Freqs
    //if (CpuFreq == CpuOverclockFreq && EnableOverclock) {
    if (i == MaxPerfLevel) {
      // Set defined Freq
      for (UINT8 j = 0; j < FixedPcdGet32 (PcdClusterCount) + EnableL3Overclock; j++) {
        Status = mClockProtocol->SetCpuPerfLevel (mClockProtocol, j, i, &CpuFreq);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to set CPU Perf Level %u! Status = %r\n", i, Status));
          ASSERT_EFI_ERROR (Status);
        }

        DEBUG ((EFI_D_WARN, "CPU Cluster %u now runs at %llu MHz\n", j, CpuFreq / 1000 / 1000));
      }
    }
  }

  return EFI_SUCCESS;
}
