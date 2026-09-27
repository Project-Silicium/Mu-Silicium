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
// Memory Allocation Details
//
STATIC EFI_LOADER_RANGE mLoaderRange[MAX_LOADER_RANGES] = {0};
STATIC UINT8            mLoaderCount                    = 0;

//
// Winload Memory Details
//
STATIC EFI_PHYSICAL_ADDRESS mWinloadBase   = 0;
STATIC UINTN                mWinloadLength = 0;

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

  // Save Memory Allocation
  if (MemoryType == EfiLoaderCode && mLoaderCount < MAX_LOADER_RANGES) {
    mLoaderRange[mLoaderCount].Base   = (EFI_PHYSICAL_ADDRESS)(*Memory);
    mLoaderRange[mLoaderCount].Length = EFI_PAGES_TO_SIZE (Pages);
    mLoaderCount++;
  }

  // Go thru Saved Memory Allocations
  for (UINT8 i = 0; i < mLoaderCount; i++) {
    // Check for Winload Memory
    if (IsWinloadMemory (mLoaderRange[i].Base, mLoaderRange[i].Length)) {
      mWinloadBase   = mLoaderRange[i].Base;
      mWinloadLength = mLoaderRange[i].Length;
      break;
    }
  }

  // Verify Winload Base
  if (!mWinloadBase) {
    return EFI_SUCCESS;
  }

  // Unprotect Winload Memory
  Status = SetWinloadProtection (mWinloadBase, mWinloadLength, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Unprotect Winload Memory! Status = %r\n", Status));
    goto restore;
  }

  // Apply Platform Specific Patches
  ApplyPlatformErrataPatches (mWinloadBase, mWinloadLength);

  // Reprotect Winload Memory
  Status = SetWinloadProtection (mWinloadBase, mWinloadLength, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Reprotect Winload Memory! Status = %r\n", Status));
  }

restore:
  // Restore Original "AllocatePages"
  gBS->AllocatePages = mOrigAllocatePages;

  // Calculate new CRC32
  gBS->Hdr.CRC32 = 0;
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

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
  gBS->Hdr.CRC32        = 0;

  // Calculate new CRC32
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Unprotect Winload Memory
  Status = SetWinloadProtection (mWinloadBase, mWinloadLength, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Unprotect Winload Memory! Status = %r\n", Status));
    goto exit;
  }

  // Get Platform Shell Code
  GetPlatformShellCode (&ShellCode, &ShellCodeSize);

  // Verify Platform Shell Code
  if (ShellCode != NULL && ShellCodeSize != 0) {
    // Patch Transfer to Kernel
    PatchTransferToKernel (mWinloadBase, mWinloadLength, ShellCode, ShellCodeSize);
  }

  // Reprotect Winload Memory
  Status = SetWinloadProtection (mWinloadBase, mWinloadLength, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[KEP] Failed to Reprotect Winload Memory! Status = %r\n", Status));
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
