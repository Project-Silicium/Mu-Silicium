/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#ifndef _KERNEL_ERRATA_PATCHER_LIB_H_
#define _KERNEL_ERRATA_PATCHER_LIB_H_

//
// Functions
//
EFI_STATUS
EFIAPI
KernelErrataPatcherExitBootServices (
  IN EFI_HANDLE           ImageHandle,
  IN UINTN                MapKey,
  IN EFI_PHYSICAL_ADDRESS fwpKernelSetupPhase1
  );

EFI_STATUS
EFIAPI
ExitBootServicesWrapper (
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey
  );

EFI_STATUS
LocateMemoryAttributeProtocol ();

EFI_STATUS
SetWinloadProtection (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length,
  IN BOOLEAN              Enable
  );

EFI_STATUS
LocateWinloadMemoryRange (
  IN  EFI_PHYSICAL_ADDRESS  fwpKernelSetupPhase1,
  OUT EFI_PHYSICAL_ADDRESS *Base,
  OUT UINTN                *Length
  );

EFI_STATUS
PatchOsLoaderArm64TransferToKernel (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINT64                Length,
  IN UINT8                *ShellCode,
  IN UINTN                 ShellCodeSize
  );

#endif /* _KERNEL_ERRATA_PATCHER_LIB_H_ */
