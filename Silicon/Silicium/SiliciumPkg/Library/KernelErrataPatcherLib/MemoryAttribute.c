/**
  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/MemoryAttribute.h>

//
// Global Variables
//
STATIC EFI_MEMORY_ATTRIBUTE_PROTOCOL *mMemoryAttributeProtocol;

EFI_STATUS
SetWinloadProtection (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN EFI_PHYSICAL_ADDRESS End,
  IN BOOLEAN              Enable)
{
  // Verify Memory Attribute Protocol
  if (mMemoryAttributeProtocol == NULL) {
    return EFI_SUCCESS;
  }

  // Set / Clear Read-Only Memory Attribute
  return Enable ? mMemoryAttributeProtocol->SetMemoryAttributes   (mMemoryAttributeProtocol, Base, (End - Base), EFI_MEMORY_RO)
                : mMemoryAttributeProtocol->ClearMemoryAttributes (mMemoryAttributeProtocol, Base, (End - Base), EFI_MEMORY_RO);
}

EFI_STATUS
LocateMemoryAttributeProtocol (VOID)
{
  // Locate Memory Attribute Protocol
  gBS->LocateProtocol (&gEfiMemoryAttributeProtocolGuid, NULL, (VOID *)&mMemoryAttributeProtocol);

  return EFI_SUCCESS;
}
