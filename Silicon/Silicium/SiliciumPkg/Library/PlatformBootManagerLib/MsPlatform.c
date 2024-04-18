/**
  PlatformBootManager - Ms Extensions to BdsDxe.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DeviceBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>

VOID
EFIAPI
PlatformBootManagerOnDemandConInConnect ()
{
  EFI_DEVICE_PATH_PROTOCOL **PlatformConnectDeviceList;
  EFI_HANDLE                 DeviceHandle;
  EFI_HANDLE                *HandleBuffer;
  CHAR16                    *TmpStr;
  UINTN                      HandleCount;

  // Get Platform Console In Devices
  PlatformConnectDeviceList = DeviceBootManagerOnDemandConInConnect ();

  if (PlatformConnectDeviceList != NULL) {
    while (*PlatformConnectDeviceList != NULL) {
      // Convert Device Path to Text
      TmpStr = ConvertDevicePathToText (*PlatformConnectDeviceList, FALSE, FALSE);

      if (TmpStr != NULL) {
        FreePool (TmpStr);
      }

      // Connect Device Path
      EfiBootManagerConnectDevicePath (*PlatformConnectDeviceList, &DeviceHandle);

      PlatformConnectDeviceList++;
    }
  }

  // Locate Mouse Handle Buffers
  gBS->LocateHandleBuffer (ByProtocol, &gEfiAbsolutePointerProtocolGuid, NULL, &HandleCount, &HandleBuffer);

  // Connect Mouse Controllers
  for (UINTN Index = 0; Index < HandleCount; Index++) {
    gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
  }

  return;
}

VOID
EFIAPI
PlatformBootManagerBdsEntry ()
{
  DeviceBootManagerBdsEntry ();
}

VOID
EFIAPI
PlatformBootManagerProcessBootCompletion (IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  STATIC BOOLEAN mSecViolation = FALSE;

  if ((EFI_SUCCESS == BootOption->Status) && (mSecViolation)) {
    mSecViolation      = FALSE;
    BootOption->Status = OEM_PREVIOUS_SECURITY_VIOLATION;
  }

  DeviceBootManagerProcessBootCompletion (BootOption);
}

VOID
EFIAPI
PlatformBootManagerPriorityBoot (UINT16 **BootNext)
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootOption;
  EFI_BOOT_MANAGER_LOAD_OPTION BootManagerMenu;

  // Get Priority Boot
  Status = DeviceBootManagerPriorityBoot (&BootOption);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Clear BootNext
  if (*BootNext != NULL) {
    FreePool (*BootNext);
    *BootNext = NULL;
  }

  // Attempt the priority boot option.
  EfiBootManagerBoot (&BootOption);

  Status = BootOption.Status;
  
  // Free Load Option
  EfiBootManagerFreeLoadOption (&BootOption);

  // Get Boot Manager Menu
  if (Status == EFI_SUCCESS) {
    Status = EfiBootManagerGetBootManagerMenu (&BootManagerMenu);
    if (!EFI_ERROR (Status)) {
      EfiBootManagerBoot           (&BootManagerMenu);
      EfiBootManagerFreeLoadOption (&BootManagerMenu);
    }
  }
}

VOID
EFIAPI
PlatformBootManagerUnableToBoot ()
{
  DeviceBootManagerUnableToBoot ();
}

EFI_STATUS
EFIAPI
PlatformBootManagerEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  return EFI_SUCCESS;
}
