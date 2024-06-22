/** @file

  Patches NTOSKRNL to not cause a SError when reading/writing ACTLR_EL1
  Patches NTOSKRNL to not cause a SError when reading/writing AMCNTENSET0_EL0
  Patches NTOSKRNL to not cause a bugcheck when attempting to use
  PSCI_MEMPROTECT Due to an issue in QHEE

  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT

**/
#ifndef _KERNEL_ERRATA_PATCHER_LIB_H_
#define _KERNEL_ERRATA_PATCHER_LIB_H_

#include <PiDxe.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PerformanceLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/MemoryAttribute.h>

#define FirmwarePrint(x, ...)                                            DEBUG((EFI_D_ERROR, x, __VA_ARGS__));

#define NT_OS_LOADER_ARM64_TRANSFER_TO_KERNEL_FUNCTION_OFFSET            0x400
#define NT_OS_LOADER_ARM64_TRANSFER_TO_KERNEL_FUNCTION_OFFSET_GERMANIUM  0x480
#define ARM64_INSTRUCTION_LENGTH                                         4
#define ARM64_TOTAL_INSTRUCTION_LENGTH(x)                               (ARM64_INSTRUCTION_LENGTH * x)

#define ARM64_BRANCH_LOCATION_INSTRUCTION(CurrentOffset, TargetOffset)  (0x94000000u | ((UINT32)((TargetOffset - CurrentOffset) / ARM64_INSTRUCTION_LENGTH) & 0x7FFFFFFu))

#define SCAN_MAX                                                         0x300000

#define IN_RANGE(x, a, b)                                               (x >= a && x <= b)
#define GET_BITS(x)                                                     (IN_RANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (IN_RANGE(x, '0', '9') ? x - '0' : 0))
#define GET_BYTE(a, b)                                                  (GET_BITS(a) << 4 | GET_BITS(b))

EFI_STATUS
EFIAPI
KernelErrataPatcherExitBootServices(
  IN EFI_HANDLE           ImageHandle,
  IN UINTN                MapKey,
  IN EFI_PHYSICAL_ADDRESS fwpKernelSetupPhase1
  );

EFI_STATUS
EFIAPI
ExitBootServicesWrapper(
  IN EFI_HANDLE ImageHandle,
  IN UINTN      MapKey);

EFI_PHYSICAL_ADDRESS
LocateWinloadBase(
  EFI_PHYSICAL_ADDRESS base,
  UINTN               *size
  );

VOID
CopyMemory(
  EFI_PHYSICAL_ADDRESS destination,
  EFI_PHYSICAL_ADDRESS source,
  UINTN                size
  );

UINT64
FindPattern(
  EFI_PHYSICAL_ADDRESS baseAddress,
  UINT64               size,
  CONST CHAR8         *pattern
  );

EFI_STATUS
EFIAPI
UnprotectWinload(
  EFI_PHYSICAL_ADDRESS WinloadBase,
  UINTN                WinloadLength);

EFI_STATUS
EFIAPI
ReProtectWinload(
  EFI_PHYSICAL_ADDRESS WinloadBase,
  UINTN                WinloadLength);

EFI_STATUS
EFIAPI
InitMemoryAttributeProtocol();

#endif /* _KERNEL_ERRATA_PATCHER_LIB_H_ */