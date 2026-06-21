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
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  WinloadBase;
  UINTN                 WinloadLength;
  UINT8                *TransferToKernelShellCode;
  UINTN                 TransferToKernelShellCodeSize;

  // Restore Original EBS
  gBS->ExitBootServices = mOriginalEfiExitBootServices;
  gBS->Hdr.CRC32        = 0;

  // Calculate new CRC32
  gBS->CalculateCrc32 (gBS, sizeof (EFI_BOOT_SERVICES), &gBS->Hdr.CRC32);

  // Locate Winload Memory Range
  Status = LocateWinloadMemoryRange (fwpKernelSetupPhase1, &WinloadBase, &WinloadLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Winload Memory Range! Status = %r\n", __FUNCTION__, Status));
    goto exit;
  }

  // Unprotect winload.efi
  Status = SetWinloadProtection (WinloadBase, WinloadLength, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Unprotect Winload! Status = %r\n", __FUNCTION__, Status));
    goto reprotect;
  }

  // Apply Platform Errata Patches
  Status = ApplyPlatformErrataPatches (WinloadBase, WinloadLength);
  if (EFI_ERROR (Status)) {
    goto reprotect;
  }

  // Get Platform Shell Code
  GetPlatformTransferToKernelShellCode (&TransferToKernelShellCode, &TransferToKernelShellCodeSize);
  if (TransferToKernelShellCode == NULL || TransferToKernelShellCodeSize == 0) {
    goto reprotect;
  }

  // Inject Shell Code
  Status = PatchOsLoaderArm64TransferToKernel (WinloadBase, WinloadLength, TransferToKernelShellCode, TransferToKernelShellCodeSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Inject Shell Code! Status = %r\n", __FUNCTION__, Status));
  }

reprotect:
  // Protect winload.efi
  Status = SetWinloadProtection (WinloadBase, WinloadLength, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Protect Winload! Status = %r\n", __FUNCTION__, Status));
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
  // Save Original EBS
  mOriginalEfiExitBootServices = gBS->ExitBootServices;

  // Hook into EBS
  gBS->ExitBootServices = ExitBootServicesWrapper;
  gBS->Hdr.CRC32        = 0;

  // Calculate new CRC32
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
