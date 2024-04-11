#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "InitializationUtils.h"

VOID
EarlyInitialization (VOID)
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegion;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegionPrimary;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegionSecondary;

  // Get Display Reserved Memory Region Infos
  Status = LocateMemoryMapAreaByName ("Display Reserved", &DisplayMemoryRegion);
  if (EFI_ERROR (Status)) {
    // Get Primary Display Reserved Memory Region Info
    Status = LocateMemoryMapAreaByName ("Display Reserved-1", &DisplayMemoryRegionPrimary);
    if (!EFI_ERROR (Status)) {
      // Clear Primary Screen
      ZeroMem ((VOID *)DisplayMemoryRegionPrimary.Address, DisplayMemoryRegionPrimary.Length);

      // Get Secondary Display Reserved Memory Region Info
      Status = LocateMemoryMapAreaByName ("Display Reserved-2", &DisplayMemoryRegionSecondary);
      if (!EFI_ERROR (Status)) {
        // Clear Secondary Screen
        ZeroMem ((VOID *)DisplayMemoryRegionSecondary.Address, DisplayMemoryRegionSecondary.Length);
      }
    }
  } else {
    // Clear Screen
    ZeroMem ((VOID *)DisplayMemoryRegion.Address, DisplayMemoryRegion.Length);
  }

  // Initialize SerialPortLib
  SerialPortInitialize ();

  // Do Platform Specific Initialization here
  PlatformInitialize ();

  // Print UEFI Infos
  DEBUG ((EFI_D_WARN, "\nMu-Qcom on %a\n", (CHAR16 *)FixedPcdGetPtr(PcdSmbiosSystemModel)));
  DEBUG ((EFI_D_WARN, "Firmware version %s built at %a on %a\n\n", (CHAR16 *)PcdGetPtr(PcdFirmwareVersionString), __TIME__, __DATE__));
}
