/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ErrataPatchesLib.h>
#include <Library/PerformanceLib.h>

#include "KernelErrataPatcherLib.h"

//
// gBS Function Backups
//
STATIC EFI_ALLOCATE_PAGES     mOrigAllocatePages    = NULL;
STATIC EFI_EXIT_BOOT_SERVICES mOrigExitBootServices = NULL;

//
// Winload Memory
//
extern EFI_PHYSICAL_ADDRESS mWinloadBase;
extern EFI_PHYSICAL_ADDRESS mWinloadEnd;

STATIC
BOOLEAN
KepParseAllocatedMemory (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Pages,
  IN EFI_MEMORY_TYPE      MemoryType)
{
  // Set Default Loader Range Details
  STATIC EFI_PHYSICAL_ADDRESS LoaderBase[MAX_LOADER_RANGES] = {0};
  STATIC EFI_PHYSICAL_ADDRESS LoaderEnd[MAX_LOADER_RANGES]  = {0};
  STATIC UINT8                LoaderCount                   = 0;

  // Save Memory Allocation
  if (MemoryType == EfiLoaderCode && LoaderCount < MAX_LOADER_RANGES) {
    LoaderBase[LoaderCount] = Base;
    LoaderEnd[LoaderCount]  = Base + EFI_PAGES_TO_SIZE (Pages);
    LoaderCount++;
  }

  // Go thru Saved Memory Allocations
  for (UINT8 i = 0; i < LoaderCount; i++) {
    // Check for Winload Memory
    if (IsWinloadMemory (LoaderBase[i], LoaderEnd[i])) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
EFIAPI
KepAllocatePagesHook (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS *Memory)
{
  EFI_STATUS Status;

  // Allocate Specified Pages
  Status = mOrigAllocatePages (Type, MemoryType, Pages, Memory);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Parse Allocated Memory
  if (!KepParseAllocatedMemory ((EFI_PHYSICAL_ADDRESS)(*Memory), Pages, MemoryType)) {
    return EFI_SUCCESS;
  }

  // Restore Original AllocatePages
  gBS->AllocatePages = mOrigAllocatePages;

  // Calculate new CRC32
  gBS->Hdr.CRC32 = 0;
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Show Winload .text Memory Range
  DEBUG ((EFI_D_WARN, "[KEP] Winload .text Memory Range = 0x%p - 0x%p\n", mWinloadBase, mWinloadEnd));

  // Unprotect Winload .text Section
  Status = SetWinloadProtection (FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Unprotect Winload .text Section! Status = %r\n", Status));
    return EFI_SUCCESS;
  }

  // Apply Platform Specific Patches
  ApplyPlatformErrataPatches (mWinloadBase, mWinloadEnd);

  // Reprotect Winload .text Section
  Status = SetWinloadProtection (TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Reprotect Winload .text Section! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KepExitBootServicesHook (
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey)
{
  EFI_STATUS  Status;
  UINT8      *ShellCode;
  UINTN       ShellCodeSize;

  // Restore Original EBS
  gBS->ExitBootServices = mOrigExitBootServices;

  // Calculate new CRC32
  gBS->Hdr.CRC32 = 0;
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Verify Winload Memory Range
  if (mWinloadBase == 0 || mWinloadEnd == 0) {
    goto exit;
  }

  // Get Platform Shell Code
  GetPlatformShellCode (&ShellCode, &ShellCodeSize);
  if (ShellCode == NULL || ShellCodeSize == 0) {
    goto exit;
  }

  // Unprotect Winload .text Section
  Status = SetWinloadProtection (FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Unprotect Winload .text Section! Status = %r\n", Status));
    goto exit;
  }

  // Patch Transfer to Kernel
  PatchWinloadTransferToKernel (ShellCode, ShellCodeSize);

  // Reprotect Winload .text Section
  Status = SetWinloadProtection (TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Reprotect Winload .text Section! Status = %r\n", Status));
  }

exit:
  // Call Original EBS
  return gBS->ExitBootServices (ImageHandle, MapKey);
}

VOID
EFIAPI
ReadyToBootHandler (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  // Save Original gBS Functions
  mOrigExitBootServices = gBS->ExitBootServices;
  mOrigAllocatePages    = gBS->AllocatePages;

  // Hook into gBS
  gBS->AllocatePages    = KepAllocatePagesHook;
  gBS->ExitBootServices = KepExitBootServicesHook;

  // Calculate new CRC32
  gBS->Hdr.CRC32 = 0;
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Close Event
  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
KernelErrataPatcherLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_EVENT  ReadyToBootEvent;

  // Create Ready To Boot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, ReadyToBootHandler, NULL, &gEfiEventReadyToBootGuid, &ReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Ready To Boot Event! Status = %r\n", __FUNCTION__, Status));
    return EFI_SUCCESS;
  }

  // Locate Memory Attribute Protocol
  return LocateMemoryAttributeProtocol ();
}
