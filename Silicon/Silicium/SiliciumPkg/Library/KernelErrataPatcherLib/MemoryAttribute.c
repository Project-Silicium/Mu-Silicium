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

EFI_STATUS
SetWinloadProtection (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length,
  IN BOOLEAN              Enable)
{
  // Verify Memory Attribute Protocol
  if (mMemoryAttributeProtocol == NULL) {
    return EFI_SUCCESS;
  }

  // Verify Parameters
  if (Base == 0 || Length == 0) {
    return EFI_INVALID_PARAMETER;
  }

  // Set / Clear Read-Only Memory Attribute
  return Enable ? mMemoryAttributeProtocol->SetMemoryAttributes   (mMemoryAttributeProtocol, Base, Length, EFI_MEMORY_RO)
                : mMemoryAttributeProtocol->ClearMemoryAttributes (mMemoryAttributeProtocol, Base, Length, EFI_MEMORY_RO);
}

EFI_STATUS
LocateMemoryAttributeProtocol ()
{
  // Locate Memory Attribute Protocol
  gBS->LocateProtocol (&gEfiMemoryAttributeProtocolGuid, NULL, (VOID *)&mMemoryAttributeProtocol);

  return EFI_SUCCESS;
}
