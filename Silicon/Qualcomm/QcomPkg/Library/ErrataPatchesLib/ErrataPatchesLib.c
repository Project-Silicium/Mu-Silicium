/**
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/ErrataPatchesLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/AssemblyUtilsLib.h>

#include "ShellCode.h"

#if HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA == 1
VOID
ApplyReadActlrEl1Patch (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN EFI_PHYSICAL_ADDRESS End)
{
  // Go thru Winload Memory
  for (EFI_PHYSICAL_ADDRESS Current = Base; Current < End; Current += ARM64_INSTRUCTION_LENGTH) {
    // Get Current Instruction
    UINT32 Instruction = ARM64_INSTRUCTION (Current);

    // Verify Instruction | mrs x?, actlr_el1
    if ((Instruction & ~0x1F) != 0xD5381020) {
      continue;
    }

    // Replace Instruction | mov x?, #0x0
    ARM64_INSTRUCTION (Current) = 0xD2800000 | (Instruction & 0x1F);

    // Flush Cache
    WriteBackInvalidateDataCacheRange ((VOID *)Current, ARM64_INSTRUCTION_LENGTH);
    InvalidateInstructionCacheRange   ((VOID *)Current, ARM64_INSTRUCTION_LENGTH);

    // Show Progress
    DEBUG ((EFI_D_WARN, "[KEP] Patched ACTLR_EL1 Instruction at 0x%p\n", Current));
  }
}
#endif

VOID
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN EFI_PHYSICAL_ADDRESS End)
{
#if HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA == 1
  // Apply ACTLR_EL1 Errata Patch
  ApplyReadActlrEl1Patch (Base, End);
#endif
}

VOID
GetPlatformShellCode (
  OUT UINT8 **ShellCode,
  OUT UINTN  *ShellCodeSize)
{
  // Pass Shell Code Data
  *ShellCode     = TransferToKernelShellCode;
  *ShellCodeSize = sizeof (TransferToKernelShellCode);
}
