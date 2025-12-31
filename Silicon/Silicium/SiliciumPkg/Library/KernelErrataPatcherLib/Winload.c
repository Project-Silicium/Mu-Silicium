/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

#include "KernelErrataPatcherLib.h"
#include "Winload.h"

EFI_PHYSICAL_ADDRESS
LocateWinloadBase (
  IN  EFI_PHYSICAL_ADDRESS  Base,
  OUT UINTN                *Length)
{
  // Align Base Address
  Base = ALIGN_VALUE (Base, EFI_PAGE_SIZE);

  // Set Scan Length
  EFI_PHYSICAL_ADDRESS BaseEnd = Base - SCAN_MAX;

  for (; Base > BaseEnd; Base -= EFI_PAGE_SIZE) {
    // Verify DOS Signature
    if (*(UINT16 *)Base != EFI_IMAGE_DOS_SIGNATURE) {
      continue;
    }

    // Set Base Offset
    UINT32 BaseOffset = *(UINT32 *)(Base + 0x3C);

    // Set new winload.efi Base Address
    EFI_PHYSICAL_ADDRESS NewBase = Base + BaseOffset;

    // Verify NT Signature
    if (*(UINT16 *)NewBase != EFI_IMAGE_NT_SIGNATURE) {
      continue;
    }

    // Set winload.efi Length
    *Length = *(UINT32 *)(NewBase + 0x110);

    // Align Length
    *Length = ALIGN_VALUE (*Length, EFI_PAGE_SIZE);

    return Base;
  }

  return 0;
}

VOID
PatchOsLoaderArm64TransferToKernel (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINT8                *ShellCode,
  IN UINTN                 ShellCodeSize)
{
  for (UINT8 i = 0; i < sizeof (WinSemesterData); i++) {
    // Set Transfer To Kernel Address
    EFI_PHYSICAL_ADDRESS TransferToKernelAddr = Base + WinSemesterData[i].TransferToKernelOffset;

    // Set new Transfer To Kernel Address
    EFI_PHYSICAL_ADDRESS NewTransferToKernelAddr = TransferToKernelAddr - ShellCodeSize;

    for (EFI_PHYSICAL_ADDRESS Current = TransferToKernelAddr; Current < TransferToKernelAddr + SCAN_MAX; Current += sizeof (UINT32)) {
      // Verify Branch Instruction
      if (ARM64_BRANCH_LOCATION_INSTRUCTION (Current, TransferToKernelAddr) != *(UINT32 *)Current) {
        continue;
      }

      // Verify Next Instruction
      if (*(UINT32*)(Current + sizeof (UINT32)) != WinSemesterData[i].Instruction) {
        continue;
      }

      // Print Windows Semester
      DEBUG ((EFI_D_WARN, "%a: Detected Windows Semester: %a\n", __FUNCTION__, WinSemesterData[i].Name));

      // Inject Jump Instruction
      *(UINT32 *)Current = ARM64_BRANCH_LOCATION_INSTRUCTION (Current, NewTransferToKernelAddr);

      // Copy Shell Code
      CopyMem ((VOID *)NewTransferToKernelAddr, (CONST VOID *)ShellCode, ShellCodeSize);

      return;
    }
  }

  DEBUG ((EFI_D_ERROR, "%a: No Suitable Transfer to Kernel Function Offset was Found!\n", __FUNCTION__));
}
