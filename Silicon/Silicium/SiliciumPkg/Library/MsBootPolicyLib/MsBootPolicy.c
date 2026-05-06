/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MsBootPolicyLib.h>
#include <Library/DevicePathLib.h>

BOOLEAN
EFIAPI
MsBootPolicyLibIsSettingsBoot ()
{
  return FALSE;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsAltBoot ()
{
  return FALSE;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibClearBootRequests ()
{
  return EFI_SUCCESS;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathBootable (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  // Check Device Path
  if (!IsDevicePathValid (DevicePath, 0) || !DevicePath) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDeviceBootable (IN EFI_HANDLE ControllerHandle)
{
  return MsBootPolicyLibIsDevicePathBootable (DevicePathFromHandle (ControllerHandle));
}

BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathUsb (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  // Go thru each Device Path
  for (EFI_DEVICE_PATH_PROTOCOL *Node = DevicePath; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    // Compare Device Path Types
    if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH) && ((DevicePathSubType (Node) == MSG_USB_CLASS_DP) || (DevicePathSubType (Node) == MSG_USB_WWID_DP) || (DevicePathSubType (Node) == MSG_USB_DP))) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibGetBootSequence (
  OUT BOOT_SEQUENCE **BootSequence,
  IN  BOOLEAN         AltBootRequest)
{
  return EFI_UNSUPPORTED;
}
