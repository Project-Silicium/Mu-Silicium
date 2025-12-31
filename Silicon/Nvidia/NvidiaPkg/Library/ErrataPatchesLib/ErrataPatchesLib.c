#include <Library/ErrataPatchesLib.h>

EFI_STATUS
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  return EFI_SUCCESS;
}

UINT8*
GetPlatformTransferToKernelShellCode (OUT UINTN *ShellCodeSize)
{
  // Pass Dummy Size
  *ShellCodeSize = 0;

  return NULL;
}
