/**
  Copyright (c) 2022-2023 DuoWoA authors
  SPDX-License-Identifier: MIT
**/

#include <Library/ErrataPatchesLib.h>

EFI_STATUS
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  return EFI_UNSUPPORTED;
}

UINT8*
GetPlatformTransferToKernelShellCode (OUT UINTN *ShellCodeSize)
{
  // Pass Size
  *ShellCodeSize = 0;

  return NULL;
}
