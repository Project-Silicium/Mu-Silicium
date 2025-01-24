#include <Library/DebugLib.h>

EFI_STATUS
EFIAPI
XenonEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  DEBUG ((EFI_D_WARN, "Nothing to see here ;P\n"));

  return EFI_SUCCESS;
}
