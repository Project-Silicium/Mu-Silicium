/**@file Console Message Library

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/UefiLib.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/MuUefiVersionLib.h>

/* Used to read chip serial number */
#include <Protocol/EFIChipInfo.h>

/**
Display the platform specific debug messages
**/
VOID EFIAPI ConsoleMsgLibDisplaySystemInfoOnConsole(VOID)
{
  EFI_STATUS             Status;
  CHAR8                 *uefiDate             = NULL;
  CHAR8                 *uefiVersion          = NULL;
  EFIChipInfoVersionType SoCID                = 0;
  UINT16                 chipInfoMajorVersion = 0;
  UINT16                 chipInfoMinorVersion = 0;
  UINT32                 RamSizeInMB          = 0;
  UINT32                 RamSizeInGB          = 0;
  UINTN                  DateBufferLength     = 0;
  UINTN                  VersionBufferLength  = 0;

  EFI_CHIPINFO_PROTOCOL *mBoardProtocol = NULL;

  Print(L"Firmware information:\n");

  Status = GetBuildDateStringAscii(NULL, &DateBufferLength);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    uefiDate = (CHAR8 *)AllocateZeroPool(DateBufferLength);
    if (uefiDate != NULL) {
      Status = GetBuildDateStringAscii(uefiDate, &DateBufferLength);
      if (Status == EFI_SUCCESS) {
        Print(L"  UEFI build date: %a\n", uefiDate);
      }
      FreePool(uefiDate);
    }
  }

  Status = GetUefiVersionStringAscii(NULL, &VersionBufferLength);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    uefiVersion = (CHAR8 *)AllocateZeroPool(VersionBufferLength);
    if (uefiVersion != NULL) {
      Status = GetUefiVersionStringAscii(uefiVersion, &VersionBufferLength);
      if (Status == EFI_SUCCESS) {
        Print(L"  UEFI version:    %a\n", uefiVersion);
      }
      FreePool(uefiVersion);
    }
  }

  Print(L"  UEFI flavor:     %a\n", (VOID *)PcdGetPtr(PcdSmbiosSystemRetailModel));

  Print(L"Hardware information:\n");

  // Locate Qualcomm Board Protocol
  Status = gBS->LocateProtocol(
      &gEfiChipInfoProtocolGuid, NULL, (VOID *)&mBoardProtocol);

  if (mBoardProtocol != NULL) {
    mBoardProtocol->GetChipVersion(mBoardProtocol, &SoCID);
    chipInfoMajorVersion = (UINT16)((SoCID >> 16) & 0xFFFF);
    chipInfoMinorVersion = (UINT16)(SoCID & 0xFFFF);
    Print(L"  SoC ID:    %d.%d\n", chipInfoMajorVersion, chipInfoMinorVersion);
  }

  // Calc Memory
  RamSizeInMB = PcdGet64(PcdSystemMemorySize) / 0x100000;
  RamSizeInGB = RamSizeInMB / 1024;
  Print(L"  Memory ID: 0 (%dGB)\n", RamSizeInGB);
}
