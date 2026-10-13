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

//
// Winload Memory
//
extern EFI_PHYSICAL_ADDRESS mWinloadBase;
extern EFI_PHYSICAL_ADDRESS mWinloadEnd;

EFI_STATUS
SetWinloadProtection (IN BOOLEAN Enable)
{
  // Verify Memory Attribute Protocol
  if (mMemoryAttributeProtocol == NULL) {
    return EFI_SUCCESS;
  }

  // Verify Winload Memory Range
  if (mWinloadBase == 0 || mWinloadEnd == 0) {
    return EFI_INVALID_PARAMETER;
  }

  // Set Memory Length
  UINTN WinloadLength = ALIGN_VALUE ((mWinloadEnd - mWinloadBase), EFI_PAGE_SIZE);

  // Set / Clear Read-Only Memory Attribute
  return Enable ? mMemoryAttributeProtocol->SetMemoryAttributes   (mMemoryAttributeProtocol, mWinloadBase, WinloadLength, EFI_MEMORY_RO)
                : mMemoryAttributeProtocol->ClearMemoryAttributes (mMemoryAttributeProtocol, mWinloadBase, WinloadLength, EFI_MEMORY_RO);
}

EFI_STATUS
LocateMemoryAttributeProtocol (VOID)
{
  // Locate Memory Attribute Protocol
  gBS->LocateProtocol (&gEfiMemoryAttributeProtocolGuid, NULL, (VOID *)&mMemoryAttributeProtocol);

  return EFI_SUCCESS;
}
