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
// Winload Memory
//
EFI_PHYSICAL_ADDRESS mWinloadBase = 0;
EFI_PHYSICAL_ADDRESS mWinloadEnd  = 0;

STATIC
EFI_STATUS
GetTextMemory (
  IN EFI_PHYSICAL_ADDRESS    Base,
  IN EFI_IMAGE_NT_HEADERS64 *NtHeader)
{
  // Get PE Section Headers
  EFI_IMAGE_SECTION_HEADER *SectionHeader = (EFI_IMAGE_SECTION_HEADER *)((UINTN)&NtHeader->OptionalHeader + NtHeader->FileHeader.SizeOfOptionalHeader);

  // Go thru each Section
  for (UINT16 i = 0; i < NtHeader->FileHeader.NumberOfSections; i++) {
    // Verify Section Name
    if (AsciiStrnCmp ((CHAR8 *)SectionHeader[i].Name, ".text", EFI_IMAGE_SIZEOF_SHORT_NAME) != 0) {
      continue;
    }

    // Save .text Memory Range
    mWinloadBase = Base + SectionHeader[i].VirtualAddress;
    mWinloadEnd  = mWinloadBase + SectionHeader[i].SizeOfRawData;

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

STATIC
BOOLEAN
IsValidString (
  IN CONST CHAR8 *String,
  IN CONST VOID  *End)
{
  BOOLEAN ValidString = FALSE;

  // Go thru each Character
  for (UINT8 i = 0; i < 32; i++) {
    // Check for Memory End
    if ((CONST VOID *)(String + i) >= End) {
      return FALSE;
    }

    // Check for Null-Termination
    if (String[i] == '\0') {
      ValidString = TRUE;
      break;
    }

    // Check for Invalid Characters
    if (String[i] < 0x20 || String[i] > 0x7E) {
      break;
    }
  }

  return ValidString;
}

BOOLEAN
IsWinloadMemory (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN EFI_PHYSICAL_ADDRESS End)
{
  // Verify Parameter
  if (Base == 0) {
    return FALSE;
  }

  // Verify Memory Size
  if (sizeof (EFI_IMAGE_DOS_HEADER) > (End - Base)) {
    return FALSE;
  }

  // Populate DOS Header
  EFI_IMAGE_DOS_HEADER *DosHeader = (EFI_IMAGE_DOS_HEADER *)Base;

  // Verify DOS Signature
  if (DosHeader->e_magic != EFI_IMAGE_DOS_SIGNATURE) {
    return FALSE;
  }

  // Verify Memory Size
  if ((DosHeader->e_lfanew + sizeof (EFI_IMAGE_NT_HEADERS64)) > (End - Base)) {
    return FALSE;
  }

  // Populate NT Header
  EFI_IMAGE_NT_HEADERS64 *NtHeader = (EFI_IMAGE_NT_HEADERS64 *)(Base + DosHeader->e_lfanew);

  // Verify NT Signature
  if (NtHeader->Signature != EFI_IMAGE_NT_SIGNATURE) {
    return FALSE;
  }

  // Verify Optional Header Magic
  if (NtHeader->OptionalHeader.Magic != EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    return FALSE;
  }

  // Verify Data Directory Presense
  if (NtHeader->OptionalHeader.NumberOfRvaAndSizes == 0) {
    return FALSE;
  }

  // Get Export Directory
  UINT32 ExportDirRva  = NtHeader->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
  UINT32 ExportDirSize = NtHeader->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

  // Verify Export Directory Presense
  if (ExportDirRva == 0 || ExportDirSize == 0) {
    return FALSE;
  }

  // Verify Memory Size
  if ((ExportDirRva + ExportDirSize) > (End - Base)) {
    return FALSE;
  }

  // Populate Export Directory
  EFI_IMAGE_EXPORT_DIRECTORY *ImgDirExport = (EFI_IMAGE_EXPORT_DIRECTORY *)(Base + ExportDirRva);

  // Verify Name Presense
  if (ImgDirExport->Name == 0) {
    return FALSE;
  }

  // Verify Memory Size
  if (ImgDirExport->Name > (End - Base)) {
    return FALSE;
  }

  // Get Export Directory Name
  CHAR8 *EntryExportName = (CHAR8 *)(Base + ImgDirExport->Name);

  // Verify Entry Export Name
  if (!IsValidString (EntryExportName, (CONST VOID *)End)) {
    return FALSE;
  }

  // Compare Export Directory Name
  if (AsciiStriCmp (EntryExportName, "winload.sys") == 0) {
    // Get Winload .text Memory
    return GetTextMemory (Base, NtHeader) == EFI_SUCCESS ? TRUE : FALSE;
  }

  return FALSE;
}

VOID
PatchWinloadTransferToKernel (
  IN UINT8 *ShellCode,
  IN UINTN  ShellCodeSize)
{
  // Verify Winload Memory Range
  if (mWinloadBase == 0 || mWinloadEnd == 0) {
    return;
  }

  // Go thru each Codebase Semester
  for (UINT8 i = 0; i < ARRAY_SIZE (CodebaseSemester); i++) {
    // Set TransferToKernel Function Locations
    EFI_PHYSICAL_ADDRESS TransferToKernelAddr    = mWinloadBase + CodebaseSemester[i].TransferToKernelOffset;
    EFI_PHYSICAL_ADDRESS NewTransferToKernelAddr = TransferToKernelAddr - ShellCodeSize;

    // Parse TransferToKernel Function
    for (EFI_PHYSICAL_ADDRESS Current = TransferToKernelAddr; Current < TransferToKernelAddr + (mWinloadEnd - TransferToKernelAddr); Current += ARM64_INSTRUCTION_LENGTH) {
      // Verify Branch Instruction
      if (ARM64_INSTRUCTION (Current) != ARM64_BRANCH_LOCATION_INSTRUCTION (Current, TransferToKernelAddr)) {
        continue;
      }

      // Verify Next Instruction
      if (ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (1)) != CodebaseSemester[i].TargetInstruction) {
        continue;
      }

      // Show Detected Codebase Semester
      DEBUG ((EFI_D_WARN, "[WINLOAD] Detected Codebase Semester = %a\n", CodebaseSemester[i].Name));

      // Inject Shell Code
      CopyMem ((VOID *)NewTransferToKernelAddr, (CONST VOID *)ShellCode, ShellCodeSize);

      // Patch Jump Instruction
      ARM64_INSTRUCTION (Current) = ARM64_BRANCH_LOCATION_INSTRUCTION (Current, NewTransferToKernelAddr);

      // Flush Cache
      WriteBackInvalidateDataCacheRange ((VOID *)NewTransferToKernelAddr, ShellCodeSize);
      InvalidateInstructionCacheRange   ((VOID *)NewTransferToKernelAddr, ShellCodeSize);
      WriteBackInvalidateDataCacheRange ((VOID *)Current, ARM64_INSTRUCTION_LENGTH);
      InvalidateInstructionCacheRange   ((VOID *)Current, ARM64_INSTRUCTION_LENGTH);

      // Show Progress
      DEBUG ((EFI_D_WARN, "[WINLOAD] Injected Shell Code at 0x%p\n",        NewTransferToKernelAddr));
      DEBUG ((EFI_D_WARN, "[WINLOAD] Patched Branch Instruction at 0x%p\n", Current));
      return;
    }
  }

  // Show Failure
  DEBUG ((EFI_D_ERROR, "[WINLOAD] Failed to Detect Codebase Semester!\n"));
}
