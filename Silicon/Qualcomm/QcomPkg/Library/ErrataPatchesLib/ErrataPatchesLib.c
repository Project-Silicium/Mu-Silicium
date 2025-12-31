/**
  Copyright (c) 2022-2023 DuoWoA authors
  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ErrataPatchesLib.h>

#include "ShellCode.h"

#define IN_RANGE(x, a, b) (x >= a && x <= b)
#define GET_BITS(x)       (IN_RANGE ((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (IN_RANGE (x, '0', '9') ? x - '0' : 0))
#define GET_BYTE(a, b)    (GET_BITS (a) << 4 | GET_BITS (b))

EFI_PHYSICAL_ADDRESS
FindPattern (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINTN                 Length,
  IN CONST CHAR8          *Pattern)
{
  EFI_PHYSICAL_ADDRESS  FirstMatch     = 0;
  CONST CHAR8          *CurrentPattern = Pattern;

  for (EFI_PHYSICAL_ADDRESS Current = Base; Current < Base + Length; Current++) {
    UINT8 CharByte = CurrentPattern[0];

    if (!CharByte) {
      return FirstMatch;
    }
      
    if (CharByte == '\?' || *(UINT8 *)(Current) == GET_BYTE (CharByte, CurrentPattern[1])) {
      if (!FirstMatch) {
        FirstMatch = Current;
      }
        
      if (!CurrentPattern[2]) {
        return FirstMatch;
      }

      ((CharByte == '\?') ? (CurrentPattern += 2) : (CurrentPattern += 3));
    } else {
      CurrentPattern = Pattern;
      FirstMatch     = 0;
    }
  }

  return 0;
}

#if HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA == 1
VOID
ApplyReadActrlEl1Patch (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  // Get ACTRL_EL1 Read Location
  EFI_PHYSICAL_ADDRESS IllegalInstruction = FindPattern (Base, Length, "28 10 38 D5");

  // Verify Location
  if (!IllegalInstruction) {
    DEBUG ((EFI_D_WARN, "%a: No ACTRL_EL1 Read Instruction was Found\n", __FUNCTION__));
    return;
  }

  // Set Fixed Instruction
  UINT8 FixedInstruction[] = {0x08, 0x00, 0x80, 0xD2};

  // Replace Faulty Instruction
  CopyMem ((VOID *)IllegalInstruction, (CONST VOID *)FixedInstruction, sizeof (FixedInstruction));
}
#endif

EFI_STATUS
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
#if HAS_ACTLR_EL1_UNIMPLEMENTED_ERRATA == 1
  // Apply ACTRL_EL1 Errata Patch
  ApplyReadActrlEl1Patch (Base, Length);
#endif

  return EFI_SUCCESS;
}

UINT8*
GetPlatformTransferToKernelShellCode (OUT UINTN *ShellCodeSize)
{
  // Pass Size
  *ShellCodeSize = sizeof (TransferToKernelShellCode);

  return TransferToKernelShellCode;
}
