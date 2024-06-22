/**
  This Module will Provide Access to Platform Information needed to Implement the MsBootPolicy.

  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <DfciSystemSettingTypes.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsPlatformDevicesLib.h>

#include <Protocol/ButtonServices.h>
#include <Protocol/LoadFile.h>
#include <Protocol/DfciSettingAccess.h>

#include <Settings/BootMenuSettings.h>
#include <Settings/DfciSettings.h>

#ifdef EFI_DEBUG
#define MAX_DEVICE_PATH_SIZE 0x100000
#else
#define MAX_DEVICE_PATH_SIZE 0
#endif

STATIC BOOT_SEQUENCE DefaultBootSequence[] = {
  MsBootHDD,
  MsBootUSB,
  MsBootDone
};

STATIC MS_BUTTON_SERVICES_PROTOCOL *gButtonService   = NULL;
STATIC EFI_IMAGE_LOAD               gSystemLoadImage = NULL;

EFI_STATUS
EFIAPI
LocalLoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  VOID                     *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE               *ImageHandle)
{
  if (NULL != DevicePath) {
    if (!MsBootPolicyLibIsDevicePathBootable (DevicePath)) {
      return EFI_ACCESS_DENIED;
    }
  }

  // Pass LoadImage Call to System LoadImage
  return gSystemLoadImage (BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);
}

/**
  Ask if the Platform is Requesting Volume Up Application.

  @retval TRUE            - The System is Requesting Volume Up Application.
  @retval FALSE           - The System is not Requesting Volume Up Application.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsSettingsBoot ()
{
  EFI_STATUS Status              = EFI_SUCCESS;
  BOOLEAN    VolumeUpApplication = FALSE;

  if (gButtonService == NULL) {
    // Locate Ms Button Services Protocol
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID *)&gButtonService);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Services Protocol! Status = %r\n", __FUNCTION__, Status));
      goto exit;
    }
  }

  // Check if Volume Up was Pressed
  Status = gButtonService->PreBootVolumeUpButtonThenPowerButtonCheck (gButtonService, &VolumeUpApplication);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Get Volume Up State! Status = %r\n", __FUNCTION__, Status));
  }

exit:
  return VolumeUpApplication;
}

/**
  Ask if the Platform is Requesting Volume Down Application.

  @retval TRUE            - The System is Requesting Volume Down Application.
  @retval FALSE           - The System is not Requesting Volume Down Application.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsAltBoot ()
{
  EFI_STATUS Status                = EFI_SUCCESS;
  BOOLEAN    VolumeDownApplication = FALSE;

  if (gButtonService == NULL) {
    // Locate Ms Button Services Protocol
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID *)&gButtonService);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Services Protocol! Status = %r\n", __FUNCTION__, Status));
      goto exit;
    }
  }

  // Check if Volume Down was Pressed
  Status = gButtonService->PreBootVolumeDownButtonThenPowerButtonCheck (gButtonService, &VolumeDownApplication);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Get Volume Down State! Status = %r\n", __FUNCTION__, Status));
  }

exit:
  return VolumeDownApplication;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibClearBootRequests ()
{
  EFI_STATUS Status;

  if (gButtonService == NULL) {
    // Locate Ms Button Services Protocol
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID **)&gButtonService);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Ms Button Services Protocol! Status = %r\n", __FUNCTION__, Status));
      goto exit;
    }
  }

  // Clear Volume Button States
  Status = gButtonService->PreBootClearVolumeButtonState (gButtonService);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Clear Volume Button States! Status = %r\n", __FUNCTION__, Status));
  }

exit:
  return Status;
}

/**
  Ask if the Platform Allows Booting this Controller.

  @retval TRUE            - Device is not Excluded from Bootingb
  @retval FALSE           - Device is Excluded from Booting.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathBootable (EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  EFI_STATUS                    Status         = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL     *Node           = NULL;
  DFCI_SETTING_ACCESS_PROTOCOL *SettingsAccess = NULL;
  UINTN                         ValueSize      = 0;
  BOOLEAN                       EnableUsbBoot  = TRUE;
  BOOLEAN                       Boot           = TRUE;

  if (DevicePath == NULL) {
    return TRUE;
  }

  if (!IsDevicePathValid (DevicePath, MAX_DEVICE_PATH_SIZE)) {
    return FALSE;
  }

  if (Boot) {
    EnableUsbBoot = TRUE;

    // Locate DFCI Setting Access Protocol
    Status = gBS->LocateProtocol (&gDfciSettingAccessProtocolGuid, NULL, (VOID **)&SettingsAccess);
    if (!EFI_ERROR (Status)) {
      ValueSize = sizeof (EnableUsbBoot);

      // Get ENABLE_USB_BOOT Setting
      Status = SettingsAccess->Get (SettingsAccess, DFCI_SETTING_ID__ENABLE_USB_BOOT, NULL, DFCI_SETTING_TYPE_ENABLE, &ValueSize, &EnableUsbBoot, NULL);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Get ENABLE_USB_BOOT Setting! Status = %r\n", __FUNCTION__, Status));
      }
    } else {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate DFCI Setting Access Protocol! Status = %r", __FUNCTION__, Status));
    }

    if (!EnableUsbBoot) {
      Node = DevicePath;
      while (!IsDevicePathEnd (Node)) {
        if (MESSAGING_DEVICE_PATH == Node->Type) {
          if ((MSG_USB_DP == Node->SubType) || (MSG_USB_WWID_DP == Node->SubType) || (MSG_USB_CLASS_DP == Node->SubType)) {
            Boot = FALSE;
            break;
          }
        }

        Node = NextDevicePathNode (Node);
      }
    }
  }

  return Boot;
}

/**
  Ask if the Platform Allows Booting this Controller.

  @retval TRUE            - Device is not Excluded from Bootingb
  @retval FALSE           - Device is Excluded from Booting.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsDeviceBootable (EFI_HANDLE ControllerHandle)
{
  return MsBootPolicyLibIsDevicePathBootable (DevicePathFromHandle (ControllerHandle));
}

/**
  Asks the Platform if the Device Path Provided is a Valid Bootable USB Device.

  @param DevicePath       - Pointer to DevicePath to check.

  @retval TRUE            - Device is a Valid USB Boot Option.
  @retval FALSE           - Device is not a Valid USB Boot Option.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathUsb (EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  return PlatformIsDevicePathUsb (DevicePath);
}

EFI_STATUS
EFIAPI
MsBootPolicyLibGetBootSequence (
  BOOT_SEQUENCE **BootSequence,
  BOOLEAN         AltBootRequest)
{
  if (BootSequence == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *BootSequence = DefaultBootSequence;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_TPL OldTpl;
  UINT32  Crc;

  //
  // If Linked with BDS, Take over gBS->LoadImage.
  // The Current Design doesn't Allow for BDS to be Terminated.
  //
  if (PcdGetBool (PcdBdsBootPolicy)) {
    OldTpl           = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gSystemLoadImage = gBS->LoadImage;
    gBS->LoadImage   = LocalLoadImage;
    gBS->Hdr.CRC32   = 0;
    Crc              = 0;
    gBS->CalculateCrc32 ((UINT8 *)&gBS->Hdr, gBS->Hdr.HeaderSize, &Crc);
    gBS->Hdr.CRC32 = Crc;
    gBS->RestoreTPL (OldTpl);
  }

  return EFI_SUCCESS;
}
