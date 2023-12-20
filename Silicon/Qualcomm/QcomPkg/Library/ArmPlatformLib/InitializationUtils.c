#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>

#include <Library/MemoryMapHelperLib.h>
#include <Library/PlatformPrePiLib.h>

#include "InitializationUtils.h"

VOID
EarlyInitialization(VOID)
{
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayMemoryRegion;
  LocateMemoryMapAreaByName("Display Reserved", &DisplayMemoryRegion);

  // Initialize SerialPortLib
  SerialPortInitialize();

  // Clear Screen
  ZeroMem((VOID *)DisplayMemoryRegion.Address, DisplayMemoryRegion.Length);

  // Print UEFI Infos
  DEBUG ((EFI_D_WARN, "\nMu-Qcom on %a (AArch64)\n", (VOID *)FixedPcdGetPtr(PcdSmbiosSystemModel)));
  DEBUG ((EFI_D_WARN, "Firmware version %s built on %a at %a\n\n", (CHAR16 *)PcdGetPtr(PcdFirmwareVersionString), __TIME__, __DATE__));

  // Do platform specific initialization here
  PlatformInitialize();
}