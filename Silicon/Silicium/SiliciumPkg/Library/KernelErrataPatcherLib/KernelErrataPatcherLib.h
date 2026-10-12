/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#ifndef _KERNEL_ERRATA_PATCHER_LIB_H_
#define _KERNEL_ERRATA_PATCHER_LIB_H_

//
// Save Limits
//
#define MAX_LOADER_RANGES 64

//
// Functions
//
EFI_STATUS
LocateMemoryAttributeProtocol (VOID);

BOOLEAN
IsWinloadMemory (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN EFI_PHYSICAL_ADDRESS End
  );

EFI_STATUS
SetWinloadProtection (IN BOOLEAN Enable);

VOID
PatchWinloadTransferToKernel (
  IN UINT8 *ShellCode,
  IN UINTN  ShellCodeSize
  );

#endif /* _KERNEL_ERRATA_PATCHER_LIB_H_ */
