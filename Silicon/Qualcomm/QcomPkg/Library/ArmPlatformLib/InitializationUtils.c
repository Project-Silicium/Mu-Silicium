#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>

#include <Library/MemoryMapHelperLib.h>
#include <Library/PlatformPrePiLib.h>

#include "InitializationUtils.h"

VOID EarlyInitialization(VOID)
{
  // Init Serial Port
  SerialPortInitialize();

  // Print Firmware Info
  DEBUG((EFI_D_INFO, "Project Mu on %a (AArch64)\n", (VOID *)FixedPcdGetPtr(PcdSmbiosSystemModel)));
  DEBUG((EFI_D_INFO, "Firmware version %s built at %a on %a\n", (CHAR16 *)PcdGetPtr(PcdFirmwareVersionString), __TIME__, __DATE__));

  // Initialize GIC
  MmioWrite32(
      GICR_WAKER_CURRENT_CPU,
      (MmioRead32(GICR_WAKER_CURRENT_CPU) & ~GIC_WAKER_PROCESSORSLEEP));

  // Do platform specific initialization here
  PlatformInitialize();
}
