/**
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#include <Library/ErrataPatchesLib.h>

VOID
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  return;
}

VOID
GetPlatformShellCode (
  OUT UINT8 **ShellCode,
  OUT UINTN  *ShellCodeSize)
{
  // Pass Shell Code Data
  *ShellCode     = NULL;
  *ShellCodeSize = 0;
}
