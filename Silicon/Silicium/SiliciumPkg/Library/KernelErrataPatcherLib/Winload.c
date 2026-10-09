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
#include <Library/BaseLib.h>

#include "Winload.h"

//
// Target Patch Locations
//
STATIC EFI_PHYSICAL_ADDRESS mTransferToKernelAddr  = 0;
STATIC EFI_PHYSICAL_ADDRESS mTargetInstructionAddr = 0;

BOOLEAN
IsWinloadMemory (
  IN  EFI_PHYSICAL_ADDRESS  Base,
  IN  EFI_PHYSICAL_ADDRESS  End,
  OUT EFI_PHYSICAL_ADDRESS *TextBase,
  OUT EFI_PHYSICAL_ADDRESS *TextEnd)
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

  // Get PE Section Headers
  EFI_IMAGE_SECTION_HEADER *SectionHeader = (EFI_IMAGE_SECTION_HEADER *)((UINTN)&NtHeader->OptionalHeader + NtHeader->FileHeader.SizeOfOptionalHeader);

  // Go thru each Section
  for (UINT16 i = 0; i < NtHeader->FileHeader.NumberOfSections; i++) {
    // Verify Section Name
    if (AsciiStrnCmp ((CHAR8 *)SectionHeader[i].Name, ".text", EFI_IMAGE_SIZEOF_SHORT_NAME) != 0) {
      continue;
    }

    // Save .text Memory Range
    *TextBase = Base + SectionHeader[i].VirtualAddress;
    *TextEnd  = *TextBase + ALIGN_VALUE (SectionHeader[i].SizeOfRawData, EFI_PAGE_SIZE);
    break;
  }

  // Verify .text Memory Range
  if (*TextBase == 0 || *TextEnd == 0) {
    return FALSE;
  }

  // Go thru each Codebase Semester
  for (UINT8 i = 0; i < ARRAY_SIZE (CodebaseSemester); i++) {
    // Save TransferToKernel Function Location
    mTransferToKernelAddr = *TextBase + CodebaseSemester[i].TransferToKernelOffset;

    // Go thru TransferToKernel Memory Area
    for (EFI_PHYSICAL_ADDRESS Current = mTransferToKernelAddr; Current < mTransferToKernelAddr + (*TextEnd - mTransferToKernelAddr); Current += ARM64_INSTRUCTION_LENGTH) {
      // Verify Branch Instruction
      if (ARM64_INSTRUCTION (Current) != ARM64_BRANCH_LOCATION_INSTRUCTION (Current, mTransferToKernelAddr)) {
        continue;
      }

      // Verify Next Instruction
      if (ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (1)) != CodebaseSemester[i].TargetInstruction) {
        continue;
      }

      // Show Winload Details
      DEBUG ((EFI_D_WARN, "[KEP] Winload Memory Range       = 0x%p - 0x%p\n", Base, End));
      DEBUG ((EFI_D_WARN, "[KEP] Winload .text Memory Range = 0x%p - 0x%p\n", *TextBase, *TextEnd));
      DEBUG ((EFI_D_WARN, "[KEP] Winload Codebase Semester  = %a\n", CodebaseSemester[i].Name));

      // Save Target Patch Instruction
      mTargetInstructionAddr = Current;

      return TRUE;
    }
  }

  // Pass Empty Data
  *TextBase = 0;
  *TextEnd  = 0;

  return FALSE;
}

VOID
PatchTransferToKernel (
  IN UINT8 *ShellCode,
  IN UINTN  ShellCodeSize)
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
