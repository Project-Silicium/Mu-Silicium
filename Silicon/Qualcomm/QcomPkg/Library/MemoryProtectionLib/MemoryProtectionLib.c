#include <Library/MemoryProtectionLib.h>
#include <Library/PrePiLib.h>

#include "MemoryProtectionSettings.h"

VOID
EnableMemoryProtection ()
{
  // Set Memory Protection Settings
  DXE_MEMORY_PROTECTION_SETTINGS DxeMemorySettings = (DXE_MEMORY_PROTECTION_SETTINGS)DXE_MEMORY_PROTECTION_SETTINGS_ON;
  MM_MEMORY_PROTECTION_SETTINGS  MmMemorySettings  = (MM_MEMORY_PROTECTION_SETTINGS)MM_MEMORY_PROTECTION_SETTINGS_ON;

  // Apply Memory Protection Settings
  BuildGuidDataHob (&gDxeMemoryProtectionSettingsGuid, &DxeMemorySettings, sizeof (DxeMemorySettings));
  BuildGuidDataHob (&gMmMemoryProtectionSettingsGuid,  &MmMemorySettings,  sizeof (MmMemorySettings));
}
