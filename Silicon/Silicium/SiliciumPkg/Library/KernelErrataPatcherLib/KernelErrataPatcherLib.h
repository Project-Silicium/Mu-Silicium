/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#ifndef _KERNEL_ERRATA_PATCHER_LIB_H_
#define _KERNEL_ERRATA_PATCHER_LIB_H_

//
// ARM64 Instruction Infos
//
#define ARM64_INSTRUCTION_LENGTH                                        4
#define ARM64_TOTAL_INSTRUCTION_LENGTH(x)                              (ARM64_INSTRUCTION_LENGTH * x)
#define ARM64_BRANCH_LOCATION_INSTRUCTION(CurrentOffset, TargetOffset) (0x94000000U | ((UINT32)((TargetOffset - CurrentOffset) / ARM64_INSTRUCTION_LENGTH) & 0x7FFFFFFU))

//
// Max Scan Size
//
#define SCAN_MAX 0x300000

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

EFI_PHYSICAL_ADDRESS
LocateWinloadBase (
  IN  EFI_PHYSICAL_ADDRESS  Base,
  OUT UINTN                *Length
  );

EFI_STATUS
UnprotectWinload (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length
  );

VOID
PatchOsLoaderArm64TransferToKernel (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN UINT8                *ShellCode,
  IN UINTN                 ShellCodeSize
  );

EFI_STATUS
ReProtectWinload (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length
  );

EFI_STATUS
LocateMemoryAttributeProtocol ();

#endif /* _KERNEL_ERRATA_PATCHER_LIB_H_ */
