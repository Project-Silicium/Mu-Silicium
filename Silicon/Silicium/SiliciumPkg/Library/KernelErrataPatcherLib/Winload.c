/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/AssemblyUtilsLib.h>

#include "Winload.h"

//
// Target Patch Locations
//
STATIC EFI_PHYSICAL_ADDRESS mTransferToKernelAddr  = 0;
STATIC EFI_PHYSICAL_ADDRESS mTargetInstructionAddr = 0;

BOOLEAN
IsWinloadMemory (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  // Populate DOS Header
  EFI_IMAGE_DOS_HEADER *DosHeader = (EFI_IMAGE_DOS_HEADER *)Base;

  // Verify DOS Signature
  if (DosHeader->e_magic != EFI_IMAGE_DOS_SIGNATURE) {
    return FALSE;
  }

  // Populate NT Header
  EFI_IMAGE_NT_HEADERS64 *NtHeader = (EFI_IMAGE_NT_HEADERS64 *)(Base + DosHeader->e_lfanew);

  // Verify NT Signature
  if (NtHeader->Signature != EFI_IMAGE_NT_SIGNATURE) {
    return FALSE;
  }

  // Go thru each Codebase Semester
  for (UINT8 i = 0; i < ARRAY_SIZE (CodebaseSemester); i++) {
    // Get "TransferToKernel" Function Offset
    UINT16 TransferToKernelOffset = CodebaseSemester[i].TransferToKernelOffset;

    // Save TransferToKernel Function Location
    mTransferToKernelAddr = Base + TransferToKernelOffset;

    // Go thru TransferToKernel Memory Area
    for (EFI_PHYSICAL_ADDRESS Current = mTransferToKernelAddr; Current < mTransferToKernelAddr + (Length - TransferToKernelOffset); Current += ARM64_INSTRUCTION_LENGTH) {
      // Verify Branch Instruction
      if (ARM64_INSTRUCTION (Current) != ARM64_BRANCH_LOCATION_INSTRUCTION (Current, mTransferToKernelAddr)) {
        continue;
      }

      // Verify Next Instruction
      if (ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (1)) != CodebaseSemester[i].TargetInstruction) {
        continue;
      }

      // Show Winload Details
      DEBUG ((EFI_D_WARN, "[KEP] Winload Memory Range      = 0x%p - 0x%llx\n", Base, Length));
      DEBUG ((EFI_D_WARN, "[KEP] Winload Codebase Semester = %a\n", CodebaseSemester[i].Name));

      // Save Target Patch Instruction
      mTargetInstructionAddr = Current;

      return TRUE;
    }
  }

  return FALSE;
}

VOID
PatchTransferToKernel (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINT64                Length,
  IN UINT8                *ShellCode,
  IN UINTN                 ShellCodeSize)
{
  // Set New TarnsferToKernel Address
  EFI_PHYSICAL_ADDRESS NewTransferToKernelAddr = mTransferToKernelAddr - ShellCodeSize;

  // Inject Jump Instruction
  ARM64_INSTRUCTION (mTargetInstructionAddr) = ARM64_BRANCH_LOCATION_INSTRUCTION (mTargetInstructionAddr, NewTransferToKernelAddr);

  // Copy Shell Code
  CopyMem ((VOID *)NewTransferToKernelAddr, (CONST VOID *)ShellCode, ShellCodeSize);

  // Flush Cache
  WriteBackInvalidateDataCacheRange ((VOID *)NewTransferToKernelAddr, ShellCodeSize);
  InvalidateInstructionCacheRange   ((VOID *)NewTransferToKernelAddr, ShellCodeSize);
  WriteBackInvalidateDataCacheRange ((VOID *)mTargetInstructionAddr,  ARM64_INSTRUCTION_LENGTH);
  InvalidateInstructionCacheRange   ((VOID *)mTargetInstructionAddr,  ARM64_INSTRUCTION_LENGTH);

  // Show Winload Details
  DEBUG ((EFI_D_WARN, "[KEP] Patched Branch Instruction at 0x%p\n", mTargetInstructionAddr));
  DEBUG ((EFI_D_WARN, "[KEP] Injected Shell Code at 0x%p\n",        NewTransferToKernelAddr));
}
