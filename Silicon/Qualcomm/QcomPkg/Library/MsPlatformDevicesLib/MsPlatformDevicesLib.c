/**
  MsPlatformDevicesLib - Qcom Generic Library.

  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/AcpiPlatformUpdateLib.h>
#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MsPlatformDevicesLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/DevicePath.h>

#include <Configuration/BootDevices.h>

EFI_DEVICE_PATH_PROTOCOL *gPlatformConInDeviceList[] = {NULL};

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
GetSdCardDevicePath (VOID)
{
  return (EFI_DEVICE_PATH_PROTOCOL *)&SdcardDevicePath;
}

BOOLEAN
EFIAPI
PlatformIsDevicePathUsb (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  EFI_DEVICE_PATH_PROTOCOL *Node;

  for (Node = DevicePath; !IsDevicePathEnd(Node); Node = NextDevicePathNode(Node)) {
    if ((DevicePathType(Node) == MESSAGING_DEVICE_PATH) && ((DevicePathSubType(Node) == MSG_USB_CLASS_DP) || (DevicePathSubType(Node) == MSG_USB_WWID_DP) || (DevicePathSubType(Node) == MSG_USB_DP))) {
      return TRUE;
    }
  }

  return FALSE;
}

// Dummy Function Needed for Event Notification Callback
STATIC
VOID
DummyNotify(IN EFI_EVENT Event, IN VOID *Context) {}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
GetPlatformConnectList(VOID)
{
  EFI_STATUS Status;
  EFI_EVENT  UsbConfigEvt;
  EFI_EVENT  SDCardEvt;

  // Update the ACPI Tables with SoC Runtime Information
  PlatformUpdateAcpiTables();

  // Notify the USB Controller to Start Now
  Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gUsbControllerInitGuid, &UsbConfigEvt);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Signal USB Controller Start Event! Status = %r\n", __FUNCTION__, Status));
  } else {
    gBS->SignalEvent (UsbConfigEvt);
    gBS->CloseEvent  (UsbConfigEvt);
  }

  // Notify the SDC Controller to Init SD Card Now
  Status = gBS->CreateEventEx(EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gSDCardInitGuid, &SDCardEvt);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Signal SD Card Init Event! Status = %r\n", __FUNCTION__, Status));
  } else {
    gBS->SignalEvent (SDCardEvt);
    gBS->CloseEvent  (SDCardEvt);
  }

  return NULL;
}

BDS_CONSOLE_CONNECT_ENTRY*
EFIAPI
GetPlatformConsoleList (VOID)
{
  return (BDS_CONSOLE_CONNECT_ENTRY *)&gPlatformConsoles;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
GetPlatformConnectOnConInList (VOID)
{
  return (EFI_DEVICE_PATH_PROTOCOL **)&gPlatformConInDeviceList;
}

EFI_HANDLE
EFIAPI
GetPlatformPreferredConsole (OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath)
{
  EFI_STATUS                Status;
  EFI_HANDLE                Handle = NULL;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath = NULL;

  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath;

  // Locate the GOP Protocol
  Status = gBS->LocateDevicePath (&gEfiGraphicsOutputProtocolGuid, &TempDevicePath, &Handle);
  if (EFI_ERROR(Status) && !IsDevicePathEnd(TempDevicePath)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Device Path! Status = %r\n", __FUNCTION__, Status));
  }

  if (Handle != NULL) {
    // Connect the GOP Driver
    gBS->ConnectController(Handle, NULL, NULL, TRUE);

    // Get the GOP Device Path
    TempDevicePath = EfiBootManagerGetGopDevicePath(Handle);
    *DevicePath    = TempDevicePath;
  }

  return Handle;
}
