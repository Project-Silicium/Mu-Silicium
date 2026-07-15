#include <Library/PlatformBootOptionsLib.h>

VOID
GetPlatformBootOptions (
  OUT EFI_PLATFORM_BOOT_OPTION **BootOption,
  OUT UINT8                     *BootOptionCount)
{
  // Pass Empty Data
  *BootOption      = NULL;
  *BootOptionCount = 0;
}
