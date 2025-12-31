/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/MemoryAttribute.h>

#include "KernelErrataPatcherLib.h"

//
// Global Variables
//
STATIC EFI_MEMORY_ATTRIBUTE_PROTOCOL *mMemoryAttributeProtocol;
STATIC UINT64                         MemoryAttributes;

EFI_STATUS
UnprotectWinload (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  EFI_STATUS Status;

  // Verify Memory Attribute Protocol
  if (mMemoryAttributeProtocol == NULL) {
    return EFI_SUCCESS;
  }

  // Get winload.efi Memory Attributes
  Status = mMemoryAttributeProtocol->GetMemoryAttributes (mMemoryAttributeProtocol, Base, Length, &MemoryAttributes);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get winload.efi Memory Attributes! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Verify Read-Only Memory Attribute
  if (!(MemoryAttributes & EFI_MEMORY_RO)) {
    return EFI_SUCCESS;
  }

  // Clear Ready-Only Memory Attribute
  Status = mMemoryAttributeProtocol->ClearMemoryAttributes (mMemoryAttributeProtocol, Base, Length, EFI_MEMORY_RO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Clear winload.efi Ready-Only Memory Attribute! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ReProtectWinload (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length)
{
  EFI_STATUS Status;

  // Verify Memory Attribute Protocol
  if (mMemoryAttributeProtocol == NULL) {
    return EFI_SUCCESS;
  }

  // Verify Read-Only Memory Attribute
  if (!(MemoryAttributes & EFI_MEMORY_RO)) {
    return EFI_SUCCESS;
  }

  // Set Ready-Only Memory Attribute
  Status = mMemoryAttributeProtocol->SetMemoryAttributes (mMemoryAttributeProtocol, Base, Length, EFI_MEMORY_RO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Set winload.efi Ready-Only Memory Attribute! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
LocateMemoryAttributeProtocol ()
{
  // Locate Memory Attribute Protocol
  gBS->LocateProtocol (&gEfiMemoryAttributeProtocolGuid, NULL, (VOID *)&mMemoryAttributeProtocol);

  return EFI_SUCCESS;
}
