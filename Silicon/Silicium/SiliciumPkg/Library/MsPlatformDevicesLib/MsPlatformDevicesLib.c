/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsPlatformDevicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BdsExtensionLib.h>

#include <Configuration/BootDevices.h>

EFI_DEVICE_PATH_PROTOCOL *gPlatformConInDeviceList[] = { NULL };

//
// Predefined Platform Default Console Device Path
//
BDS_CONSOLE_CONNECT_ENTRY gPlatformConsoles[] = {
  {
    (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath,
    CONSOLE_IN
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath,
    CONSOLE_OUT | STD_ERROR
  },
  {
    NULL,
    0
  }
};

EFI_DEVICE_PATH_PROTOCOL*
EFIAPI
GetSdCardDevicePath ()
{
  return (EFI_DEVICE_PATH_PROTOCOL *)&SdcardDevicePath;
}

BOOLEAN
EFIAPI
PlatformIsDevicePathUsb (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  EFI_DEVICE_PATH_PROTOCOL *Node;

  for (Node = DevicePath; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH) && ((DevicePathSubType (Node) == MSG_USB_CLASS_DP) || (DevicePathSubType (Node) == MSG_USB_WWID_DP) || (DevicePathSubType( Node) == MSG_USB_DP))) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
GetPlatformConnectList ()
{
  return NULL;
}

BDS_CONSOLE_CONNECT_ENTRY*
EFIAPI
GetPlatformConsoleList ()
{
  return (BDS_CONSOLE_CONNECT_ENTRY *)&gPlatformConsoles;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
GetPlatformConnectOnConInList ()
{
  return (EFI_DEVICE_PATH_PROTOCOL **)&gPlatformConInDeviceList;
}

EFI_HANDLE
EFIAPI
GetPlatformPreferredConsole (OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath)
{
  EFI_STATUS                Status         = EFI_SUCCESS;
  EFI_HANDLE                Handle         = NULL;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath;

  // Locate the GOP Protocol
  Status = gBS->LocateDevicePath (&gEfiGraphicsOutputProtocolGuid, &TempDevicePath, &Handle);
  if (EFI_ERROR(Status) && !IsDevicePathEnd (TempDevicePath)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Device Path! Status = %r\n", __FUNCTION__, Status));
  }

  if (Handle != NULL) {
    // Connect the GOP Driver
    gBS->ConnectController (Handle, NULL, NULL, TRUE);

    // Get the GOP Device Path
    TempDevicePath = EfiBootManagerGetGopDevicePath (Handle);
    *DevicePath    = TempDevicePath;
  }

  return Handle;
}
