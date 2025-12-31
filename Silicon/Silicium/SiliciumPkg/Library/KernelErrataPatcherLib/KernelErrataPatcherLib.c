/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ErrataPatchesLib.h>
#include <Library/PerformanceLib.h>

#include "KernelErrataPatcherLib.h"

//
// Global Variables
//
STATIC EFI_EXIT_BOOT_SERVICES mOriginalEfiExitBootServices;

EFI_STATUS
EFIAPI
KernelErrataPatcherExitBootServices (
  IN EFI_HANDLE           ImageHandle,
  IN UINTN                MapKey,
  IN EFI_PHYSICAL_ADDRESS fwpKernelSetupPhase1)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS WinloadBase;
  UINTN                WinloadLength;
  UINTN                TransferToKernelShellCodeSize;

  // Restore Original EBS
  gBS->ExitBootServices = mOriginalEfiExitBootServices;
  gBS->Hdr.CRC32        = 0;

  // Calculate new CRC32
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Get winload.efi Base Address
  WinloadBase = LocateWinloadBase (fwpKernelSetupPhase1, &WinloadLength);
  if (!WinloadBase) {
    goto exit;
  }

  // Unprotect winload.efi
  Status = UnprotectWinload (WinloadBase + EFI_PAGE_SIZE, WinloadLength);
  if (EFI_ERROR (Status)) {
    goto exit;
  }

  // Apply Platform Errata Patches
  Status = ApplyPlatformErrataPatches (WinloadBase, SCAN_MAX);
  if (EFI_ERROR (Status)) {
    goto reprotect;
  }

  // Get Platform Shell Code
  UINT8 *TransferToKernelShellCode = GetPlatformTransferToKernelShellCode (&TransferToKernelShellCodeSize);

  // Apply Platform Shell Code
  if (TransferToKernelShellCode != NULL && TransferToKernelShellCodeSize) {
    PatchOsLoaderArm64TransferToKernel (WinloadBase + 0xC00, TransferToKernelShellCode, TransferToKernelShellCodeSize);
  }

reprotect:
  // Re-Protect winload.efi
  ReProtectWinload (WinloadBase + EFI_PAGE_SIZE, WinloadLength);

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
  PERF_CALLBACK_BEGIN (&gEfiEventReadyToBootGuid);

  // Save Original EBS
  mOriginalEfiExitBootServices = gBS->ExitBootServices;

  // Hook into EBS
  gBS->ExitBootServices = ExitBootServicesWrapper;
  gBS->Hdr.CRC32        = 0;

  // Calculate new CRC32
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Close Event
  gBS->CloseEvent (Event);

  PERF_CALLBACK_END (&gEfiEventReadyToBootGuid);
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
    return Status;
  }

  // Locate Memory Attribute Protocol
  return LocateMemoryAttributeProtocol ();
}
