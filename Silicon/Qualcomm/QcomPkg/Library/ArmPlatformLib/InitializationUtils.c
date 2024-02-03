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

  // Get Display Reserved Memory Region Infos
  Status = LocateMemoryMapAreaByName ("Display Reserved", &DisplayMemoryRegion);
  if (!EFI_ERROR (Status)) {
    // Clear Screen
    ZeroMem ((VOID *)DisplayMemoryRegion.Address, DisplayMemoryRegion.Length);
  }

  // Initialize SerialPortLib
  SerialPortInitialize ();

  // Do Platform Specific Initialization here
  PlatformInitialize ();

  // Print UEFI Infos
  DEBUG ((EFI_D_WARN, "\nMu-Qcom on %a (AArch64)\n", (CHAR16 *)FixedPcdGetPtr(PcdSmbiosSystemModel)));
  DEBUG ((EFI_D_WARN, "Firmware version %s built on %a at %a\n\n", (CHAR16 *)PcdGetPtr(PcdFirmwareVersionString), __TIME__, __DATE__));
}
