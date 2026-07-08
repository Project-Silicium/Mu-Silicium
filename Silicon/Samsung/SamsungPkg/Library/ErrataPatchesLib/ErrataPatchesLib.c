/**
  Copyright (c) 2022-2023 DuoWoA authors
  SPDX-License-Identifier: MIT
**/

#include <Library/ErrataPatchesLib.h>

#include "ShellCode.h"

EFI_STATUS
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  return EFI_SUCCESS;
}

VOID
GetPlatformTransferToKernelShellCode (
  OUT UINT8 **ShellCode,
  OUT UINTN  *ShellCodeSize)
{
  // Pass Shell Code Data
  *ShellCode     = TransferToKernelShellCode;
  *ShellCodeSize = sizeof (TransferToKernelShellCode);
}
