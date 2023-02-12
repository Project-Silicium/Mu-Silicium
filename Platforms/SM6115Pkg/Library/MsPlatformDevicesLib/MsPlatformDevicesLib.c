/** @file
 *MsPlatformDevicesLib  - Device specific library.
Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>

#include <Library/AcpiPlatformUpdateLib.h>
#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsPlatformDevicesLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Library/RFSProtectionLib.h>
#include <Configuration/BootDevices.h>

//
// Predefined platform default console device path
//
BDS_CONSOLE_CONNECT_ENTRY gPlatformConsoles[] =
{
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

EFI_DEVICE_PATH_PROTOCOL *gPlatformConInDeviceList[] = {NULL};

/**
Library function used to provide the platform SD Card device path
**/
EFI_DEVICE_PATH_PROTOCOL *EFIAPI GetSdCardDevicePath(VOID) { return NULL; }

/**
  Library function used to determine if the DevicePath is a valid bootable 'USB'
device. USB here indicates the port connection type not the device protocol.
  With TBT or USB4 support PCIe storage devices are valid 'USB' boot options.
**/
BOOLEAN
EFIAPI
PlatformIsDevicePathUsb(IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  return FALSE;
}

/**
Library function used to provide the list of platform devices that MUST be
connected at the beginning of BDS
**/
EFI_DEVICE_PATH_PROTOCOL **EFIAPI GetPlatformConnectList(VOID) 
{ 
  PlatformUpdateAcpiTables();
  return NULL; 
}

EFI_DEVICE_PATH_PROTOCOL **EFIAPI
GetDevicePathsFromProtocolGuid(IN EFI_GUID *ProtocolGuid, OUT UINTN *Count)
{
  EFI_STATUS                 Status;
  EFI_HANDLE                *Handles;
  UINTN                      NoHandles;
  UINTN                      Idx;
  EFI_DEVICE_PATH_PROTOCOL **pDevicePaths;
  EFI_DEVICE_PATH_PROTOCOL  *pDevicePath;

  Status = gBS->LocateHandleBuffer(
      ByProtocol, ProtocolGuid, NULL /* SearchKey */, &NoHandles, &Handles);
  if (EFI_ERROR(Status)) {
    //
    // This is not an error, just an informative condition.
    //
    DEBUG((EFI_D_VERBOSE, "%a: %g: %r\n", __FUNCTION__, ProtocolGuid, Status));
    return NULL;
  }

  ASSERT(NoHandles > 0);

  pDevicePaths = (EFI_DEVICE_PATH_PROTOCOL **)AllocateZeroPool(
      (NoHandles + 1) * sizeof(EFI_DEVICE_PATH_PROTOCOL *));

  *Count = 0;
  for (Idx = 0; Idx < NoHandles; ++Idx) {
    pDevicePath = DevicePathFromHandle(Handles[Idx]);
    if (pDevicePath != NULL) {
      *(pDevicePaths + *Count) = pDevicePath;
      *Count                   = *Count + 1;
    }
  }

  gBS->FreePool(Handles);

  return pDevicePaths;
}

/**
 * Library function used to provide the list of platform console devices.
 */
BDS_CONSOLE_CONNECT_ENTRY *EFIAPI GetPlatformConsoleList(VOID)
{
  return (BDS_CONSOLE_CONNECT_ENTRY *)&gPlatformConsoles;
}

/**
Library function used to provide the list of platform devices that MUST be
connected to support ConsoleIn activity.  This call occurs on the ConIn connect
event, and allows platforms to do enable specific devices ConsoleIn support.
**/
EFI_DEVICE_PATH_PROTOCOL **EFIAPI GetPlatformConnectOnConInList(VOID)
{
  return NULL;
}

/**
Library function used to provide the console type.  For ConType == DisplayPath,
device path is filled in to the exact controller to use.  For other ConTypes,
DisplayPath must NULL. The device path must NOT be freed.
**/
EFI_HANDLE
EFIAPI
GetPlatformPreferredConsole(OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath)
{
  return NULL;
}
