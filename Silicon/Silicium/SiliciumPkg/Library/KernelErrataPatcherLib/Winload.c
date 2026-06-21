/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/AssemblyUtilsLib.h>

#include "KernelErrataPatcherLib.h"
#include "Winload.h"

EFI_STATUS
LocateWinloadMemoryRange (
  IN  EFI_PHYSICAL_ADDRESS  fwpKernelSetupPhase1,
  OUT EFI_PHYSICAL_ADDRESS *Base,
  OUT UINTN                *Length)
{
  // Verify Parameters
  if (Base == NULL || Length == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Align FWP Address
  fwpKernelSetupPhase1 = ALIGN_VALUE (fwpKernelSetupPhase1, EFI_PAGE_SIZE);

  // Set FWP End Address
  EFI_PHYSICAL_ADDRESS fwpKernelSetupPhase1End = fwpKernelSetupPhase1 - MAX_FWP_SCAN_LENGTH;

  // Go thru FWP Scan Area
  for (EFI_PHYSICAL_ADDRESS Current = fwpKernelSetupPhase1; Current > fwpKernelSetupPhase1End; Current -= EFI_PAGE_SIZE) {
    // Get DOS Header
    EFI_IMAGE_DOS_HEADER *DosHeader = (EFI_IMAGE_DOS_HEADER *)Current;

    // Verify DOS Data
    if (DosHeader->e_magic != EFI_IMAGE_DOS_SIGNATURE || DosHeader->e_lfanew > MAX_FWP_SCAN_LENGTH) {
      continue;
    }

    // Get NT Header
    EFI_IMAGE_NT_HEADERS64 *NtHeader = (EFI_IMAGE_NT_HEADERS64 *)(Current + DosHeader->e_lfanew);

    // Verify NT Signature
    if (NtHeader->Signature != EFI_IMAGE_NT_SIGNATURE) {
      continue;
    }

    // Pass Data
    *Base   = Current;
    *Length = ALIGN_VALUE (NtHeader->OptionalHeader.SizeOfImage, EFI_PAGE_SIZE);

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
PatchOsLoaderArm64TransferToKernel (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINT64                Length,
  IN UINT8                *ShellCode,
  IN UINTN                 ShellCodeSize)
{
  // Verify Parameters
  if (Base == 0 || ShellCode == NULL || ShellCodeSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  // Go thru each Windows Semester
  for (UINT8 i = 0; i < ARRAY_SIZE (WinSemesterData); i++) {
    // Set Transfer To Kernel Details
    EFI_PHYSICAL_ADDRESS TransferToKernelAddr    = Base + 0xC00 + WinSemesterData[i].TransferToKernelOffset;
    EFI_PHYSICAL_ADDRESS TransferToKernelAddrEnd = TransferToKernelAddr + Length;

    // Set new Transfer To Kernel Address
    EFI_PHYSICAL_ADDRESS NewTransferToKernelAddr = TransferToKernelAddr - ShellCodeSize;

    // Go thru Transfer To Kernel Memory Area
    for (EFI_PHYSICAL_ADDRESS Current = TransferToKernelAddr; Current < TransferToKernelAddrEnd; Current += ARM64_INSTRUCTION_LENGTH) {
      // Verify Branch Instruction
      if (ARM64_INSTRUCTION (Current) != ARM64_BRANCH_LOCATION_INSTRUCTION (Current, TransferToKernelAddr)) {
        continue;
      }

      // Verify Next Instruction
      if (ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (1)) != WinSemesterData[i].TargetInstruction) {
        continue;
      }

      // Copy Shell Code
      CopyMem ((VOID *)NewTransferToKernelAddr, (CONST VOID *)ShellCode, ShellCodeSize);

      // Inject Jump Instruction
      ARM64_INSTRUCTION (Current) = ARM64_BRANCH_LOCATION_INSTRUCTION (Current, NewTransferToKernelAddr);

      // Flush Cache
      WriteBackInvalidateDataCacheRange ((VOID *)NewTransferToKernelAddr, ShellCodeSize);
      InvalidateInstructionCacheRange   ((VOID *)NewTransferToKernelAddr, ShellCodeSize);
      WriteBackInvalidateDataCacheRange ((VOID *)Current, ARM64_INSTRUCTION_LENGTH);
      InvalidateInstructionCacheRange   ((VOID *)Current, ARM64_INSTRUCTION_LENGTH);

      // Print Windows Semester
      DEBUG ((EFI_D_WARN, "%a: Detected Windows Semester = %a\n", __FUNCTION__, WinSemesterData[i].SemesterName));
  
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}
