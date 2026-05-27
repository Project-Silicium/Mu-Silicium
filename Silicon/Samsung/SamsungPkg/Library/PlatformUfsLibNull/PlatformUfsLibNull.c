#include <Library/DebugLib.h>
#include <Library/PlatformUfsLib.h>

EFI_STATUS
UfsBoardInit (struct UfsHost *Ufs)
{
  (VOID)Ufs;
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return EFI_UNSUPPORTED;
}
