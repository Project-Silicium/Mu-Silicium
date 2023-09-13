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
#include "KernelErrataPatcher.h"

STATIC UINT64                         WinloadAttributes        = 0;
STATIC EFI_MEMORY_ATTRIBUTE_PROTOCOL *mMemoryAttributeProtocol = NULL;

EFI_STATUS
EFIAPI
UnprotectWinload(EFI_PHYSICAL_ADDRESS WinloadBase, UINTN WinloadLength)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //
  // No Memory Attribute protocol is installed, winload is thus not protected
  //
  if (mMemoryAttributeProtocol == NULL) {
    return Status;
  }

  FirmwarePrint(
      "UnprotectWinload: winload.efi .text -> (phys) 0x%p (size) 0x%p (attrib) "
      "0x%p\n",
      WinloadBase, WinloadLength, WinloadAttributes);

  Status = mMemoryAttributeProtocol->GetMemoryAttributes(
      mMemoryAttributeProtocol, WinloadBase, WinloadLength, &WinloadAttributes);

  if (EFI_ERROR(Status)) {
    FirmwarePrint(
        "UnprotectWinload: Could not find memory attributes for winload -> "
        "(phys) 0x%p (size) 0x%p %r\n",
        WinloadBase, WinloadLength, Status);
    return Status;
  }

  FirmwarePrint(
      "UnprotectWinload: winload.efi .text -> (phys) 0x%p (size) 0x%p (attrib) "
      "0x%p\n",
      WinloadBase, WinloadLength, WinloadAttributes);

  if (WinloadAttributes & EFI_MEMORY_RO) {
    Status = mMemoryAttributeProtocol->ClearMemoryAttributes(
        mMemoryAttributeProtocol, WinloadBase, WinloadLength, EFI_MEMORY_RO);
  }

  return Status;
}

EFI_STATUS
EFIAPI
ReProtectWinload(EFI_PHYSICAL_ADDRESS WinloadBase, UINTN WinloadLength)
{
  EFI_STATUS Status = EFI_SUCCESS;

  //
  // No Memory Attribute protocol is installed, winload is thus not protected
  //
  if (mMemoryAttributeProtocol == NULL) {
    return Status;
  }

  if (WinloadAttributes & EFI_MEMORY_RO) {
    Status = mMemoryAttributeProtocol->SetMemoryAttributes(
        mMemoryAttributeProtocol, WinloadBase, WinloadLength, EFI_MEMORY_RO);
  }

  return Status;
}

EFI_STATUS
EFIAPI
InitMemoryAttributeProtocol()
{
  return gBS->LocateProtocol(
      &gEfiMemoryAttributeProtocolGuid, NULL,
      (VOID **)&mMemoryAttributeProtocol);
}