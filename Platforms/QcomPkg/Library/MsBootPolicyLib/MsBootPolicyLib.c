/*++
  This module will provide access to platform information needed to implement
  the MsBootPolicy.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>                                     // UEFI base types

#include <DfciSystemSettingTypes.h>
#include <Protocol/ButtonServices.h>
#include <Protocol/LoadFile.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DfciAuthentication.h>
#include <Library/UefiBootServicesTableLib.h>         // gBS
#include <Library/UefiRuntimeServicesTableLib.h>      // gRT
#include <Library/DebugLib.h>                         // DEBUG tracing
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsBootPolicyLib.h>                  // Current runtime mode.
#include <Library/DevicePathLib.h>
#include <Protocol/DfciSettingAccess.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsPlatformDevicesLib.h>

#include <Settings/BootMenuSettings.h>
#include <Settings/DfciSettings.h>

static BOOT_SEQUENCE  BootSequenceUPH[] = {
  MsBootUSB,
  MsBootPXE4,
  MsBootPXE6,
  MsBootHDD,
  MsBootDone
};

static BOOT_SEQUENCE  BootSequenceHUP[] = {
  // HddUsbPxe
  MsBootHDD,
  MsBootUSB,
  MsBootPXE4,
  MsBootPXE6,
  MsBootDone
};

static MS_BUTTON_SERVICES_PROTOCOL  *gButtonService = NULL;
static EFI_IMAGE_LOAD               gSystemLoadImage;

/**
 * GetButtonServiceProtocol
 *
 * @param
 *
 * @return VOID - sets gButtonService
 */
VOID
GetButtonServiceProtocol (
  VOID
  )
{
  EFI_STATUS  Status;

  if (gButtonService == NULL) {
    Status = gBS->LocateProtocol (&gMsButtonServicesProtocolGuid, NULL, (VOID **)&gButtonService);
    if (EFI_ERROR (Status)) {
      gButtonService = NULL;
    }
  }
}

EFI_STATUS
EFIAPI
LocalLoadImage (
  IN  BOOLEAN                   BootPolicy,
  IN  EFI_HANDLE                ParentImageHandle,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN  VOID                      *SourceBuffer OPTIONAL,
  IN  UINTN                     SourceSize,
  OUT EFI_HANDLE                *ImageHandle
  )
{
  if (NULL != DevicePath) {
    if (!MsBootPolicyLibIsDevicePathBootable (DevicePath)) {
      return EFI_ACCESS_DENIED;
    }
  }

  // Pass LoadImage call to system LoadImage;
  return gSystemLoadImage (
           BootPolicy,
           ParentImageHandle,
           DevicePath,
           SourceBuffer,
           SourceSize,
           ImageHandle
           );
}

/**
  Constructor
*/
EFI_STATUS
EFIAPI
MsBootPolicyLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT32   Crc;
  EFI_TPL  OldTpl;

  //
  // If linked with BDS, take over gBS->LoadImage.
  // The current design doesn't allow for BDS to be terminated.
  //
  if (PcdGetBool (PcdBdsBootPolicy)) {
    // If linked with BDS, take over gBS->LoadImage.  BDS can never
    OldTpl           = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gSystemLoadImage = gBS->LoadImage;
    gBS->LoadImage   = LocalLoadImage;
    gBS->Hdr.CRC32   = 0;
    Crc              = 0;
    gBS->CalculateCrc32 ((UINT8 *)&gBS->Hdr, gBS->Hdr.HeaderSize, &Crc);
    gBS->Hdr.CRC32 = Crc;
    gBS->RestoreTPL (OldTpl);
  }

  return EFI_SUCCESS;    // Library constructors have to return EFI_SUCCESS.
}

/**
  Print the device path
*/
static VOID
PrintDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  CHAR16  *ToText = NULL;

  if (DevicePath != NULL) {
    ToText = ConvertDevicePathToText (DevicePath, TRUE, TRUE);
  }

  DEBUG ((DEBUG_INFO, "%s", ToText)); // Output NewLine separately in case string is too long
  DEBUG ((DEBUG_INFO, "\n"));

  if (NULL != ToText) {
    FreePool (ToText);
  }

  return;
}

/**
 *Ask if the platform is requesting Settings Change

 *@retval TRUE     System is requesting Settings Change
 *@retval FALSE    System is not requesting Changes.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsSettingsBoot (
  VOID
  )
{
  BOOLEAN     BootToSetup = FALSE;
  EFI_STATUS  Status      = EFI_SUCCESS;

  // Locate the Button Services protocol
  GetButtonServiceProtocol ();
  if (gButtonService == NULL) {
    DEBUG ((DEBUG_WARN, "%a failed to locate ButtonServices protocol, assuming no presses.\n", __FUNCTION__));
  } else {
    // Check if volume up was pressed before the power button when the system powered on
    Status = gButtonService->PreBootVolumeUpButtonThenPowerButtonCheck (gButtonService, &BootToSetup);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a failed to get volume up state on power on. %r\n", __FUNCTION__, Status));

      BootToSetup = FALSE;          // not sure of its state after the Bsp call failure
    }
  }

  return BootToSetup;
}

/**
 *Ask if the platform is requesting UFP Change

 *@retval TRUE     System is requesting UFP Change
 *@retval FALSE    System is not requesting Changes.
**/
BOOLEAN
EFIAPI
SdBootPolicyLibIsUFPBoot (
  VOID
  )
{
  BOOLEAN     BootToUFP = FALSE;
  EFI_STATUS  Status    = EFI_SUCCESS;

  // Locate the Button Services protocol
  GetButtonServiceProtocol ();
  if (gButtonService == NULL) {
    DEBUG ((DEBUG_WARN, "%a failed to locate ButtonServices protocol, assuming no presses.\n", __FUNCTION__));
  } else {
    // Check if volume down was pressed before the power button when the system powered on
    Status = gButtonService->PreBootVolumeDownButtonThenPowerButtonCheck (gButtonService, &BootToUFP);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a failed to get volume down state on power on. %r\n", __FUNCTION__, Status));

      BootToUFP = FALSE;          // not sure of its state after the Bsp call failure
    }
  }

  return BootToUFP;
}

/**
 *Ask if the platform is requesting an Alternate Boot

 *@retval TRUE     System is requesting Alternate Boot
 *@retval FALSE    System is not requesting AltBoot.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsAltBoot (
  VOID
  )
{
  EFI_STATUS                    Status;
  BOOLEAN                       AltBoot       = FALSE;
  BOOLEAN                       EnableAltBoot = TRUE;
  DFCI_SETTING_ACCESS_PROTOCOL  *SettingsAccess;
  UINTN                         ValueSize;

  Status = gBS->LocateProtocol (
                  &gDfciSettingAccessProtocolGuid,
                  NULL,
                  (VOID **)&SettingsAccess
                  );
  if (!EFI_ERROR (Status)) {
    ValueSize = sizeof (EnableAltBoot);
    Status    = SettingsAccess->Get (
                                  SettingsAccess,
                                  DFCI_SETTING_ID__ALT_BOOT,
                                  NULL,    // Auth token.
                                  DFCI_SETTING_TYPE_ENABLE,
                                  &ValueSize,
                                  &EnableAltBoot,
                                  NULL     // Flags
                                  );
  }

  if (!EFI_ERROR (Status) && !EnableAltBoot) {
    DEBUG ((DEBUG_WARN, "%a EnableAltBoot is disabled.\n", __FUNCTION__));
    return FALSE;
  }

  GetButtonServiceProtocol ();
  if (gButtonService == NULL) {
    DEBUG ((DEBUG_WARN, "%a failed to locate ButtonServices protocol, assuming no presses.\n", __FUNCTION__));
  } else {
    // Check if volume down was pressed before the power button when the system powered on
    Status = gButtonService->PreBootVolumeDownButtonThenPowerButtonCheck (gButtonService, &AltBoot);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a failed to get volume down state on power on. %r\n", __FUNCTION__, Status));
    }
  }

  return AltBoot;
}

EFI_STATUS
EFIAPI
MsBootPolicyLibClearBootRequests (
  VOID
  )
{
  EFI_STATUS  Status;

  GetButtonServiceProtocol ();
  if (gButtonService == NULL) {
    Status = EFI_DEVICE_ERROR;
  } else {
    Status = gButtonService->PreBootClearVolumeButtonState (gButtonService);
  }

  return Status;
}

/**

 *Ask if the platform allows booting this controller

 *@retval TRUE     System is requesting Alternate Boot
 *@retval FALSE    System is not requesting AltBoot.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsDeviceBootable (
  EFI_HANDLE  ControllerHandle
  )
{
  return MsBootPolicyLibIsDevicePathBootable (DevicePathFromHandle (ControllerHandle));
}

/**

 *Ask if the platform allows booting this controller

 *@retval TRUE     Device is not excluded from booting
 *@retval FALSE    Device is excluded from booting.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathBootable (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_STATUS                    Status;
  BOOLEAN                       rc = TRUE;
  EFI_DEVICE_PATH_PROTOCOL      *SdCardDevicePath;
  EFI_DEVICE_PATH_PROTOCOL      *Node;
  UINTN                         Size;
  UINTN                         SdSize;
  BOOLEAN                       EnableUsbBoot = TRUE;
  DFCI_SETTING_ACCESS_PROTOCOL  *SettingsAccess;
  UINTN                         ValueSize;

  // There are two tests.
  // 1. The device is not an SdCard - SdCards are not bootable if the platform
  //    returns an SdCard device path
  // 2. THe platform setting for EnableUsbBoot can prevent USB devices from booting

  DEBUG ((DEBUG_INFO, "%a Checking if the following device path is permitted to boot:\n", __FUNCTION__));

  if (NULL == DevicePath) {
    DEBUG ((DEBUG_ERROR, "NULL device path\n"));
    return TRUE;            // Don't know where this device is, so it is not "excluded"
  }

 #ifdef EFI_DEBUG
  #define MAX_DEVICE_PATH_SIZE  0x100000// Arbitrary 1 Meg max device path size.
 #else
  #define MAX_DEVICE_PATH_SIZE  0      // Don't check length on retail builds.
 #endif

  PrintDevicePath (DevicePath);
  if (!IsDevicePathValid (DevicePath, MAX_DEVICE_PATH_SIZE)) {
    // Arbitrary 1 Meg max device path size
    DEBUG ((DEBUG_ERROR, "Invalid device path\n"));
    return FALSE;
  }

  Size = GetDevicePathSize (DevicePath);

  SdCardDevicePath = GetSdCardDevicePath ();

  if (NULL != SdCardDevicePath) {
    PrintDevicePath (SdCardDevicePath);
    SdSize =  GetDevicePathSize (SdCardDevicePath);
    if (Size > SdSize) {
      // Compare the first part of the device path to the known path of the SDCARD.
      if (0 == CompareMem (DevicePath, SdCardDevicePath, SdSize - END_DEVICE_PATH_LENGTH)) {
        DEBUG ((DEBUG_ERROR, "Boot from SD Card is not allowed.\n"));
        rc = FALSE;
      }
    }
  } else {
    DEBUG ((DEBUG_INFO, "No SD Card check enabled.\n"));
  }

  if (rc) {
    EnableUsbBoot = TRUE;
    Status        = gBS->LocateProtocol (
                           &gDfciSettingAccessProtocolGuid,
                           NULL,
                           (VOID **)&SettingsAccess
                           );
    if (!EFI_ERROR (Status)) {
      ValueSize = sizeof (EnableUsbBoot);
      Status    = SettingsAccess->Get (
                                    SettingsAccess,
                                    DFCI_SETTING_ID__ENABLE_USB_BOOT,
                                    NULL,      // Auth token.
                                    DFCI_SETTING_TYPE_ENABLE,
                                    &ValueSize,
                                    &EnableUsbBoot,
                                    NULL       // Flags
                                    );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Unable to get access to ENABLE_USB_BOOT. Code=%r\n", Status));
      }
    }

    if (!EnableUsbBoot) {
      // Boot from USB is disabled
      Node = DevicePath;
      while (!IsDevicePathEnd (Node)) {
        if (MESSAGING_DEVICE_PATH == Node->Type) {
          // If any type of USB device
          if ((MSG_USB_DP       == Node->SubType) ||             // don't allow booting
              (MSG_USB_WWID_DP  == Node->SubType) ||
              (MSG_USB_CLASS_DP == Node->SubType))
          {
            rc = FALSE;
            break;
          }
        }

        Node = NextDevicePathNode (Node);
      }
    }
  }

  if (rc) {
    DEBUG ((DEBUG_INFO, "Boot from this device is enabled\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "Boot from this device has been prevented\n"));
  }

  return rc;
}

/**
  Asks the platform if the DevicePath provided is a valid bootable 'USB' device.
  USB here indicates the physical port connection type not the device protocol.
  With TBT or USB4 support PCIe storage devices are valid 'USB' boot options.

  Default implementation:
    The platform alone determines if DevicePath is valid for USB boot support.

  @param DevicePath Pointer to DevicePath to check

  @retval TRUE     Device is a valid USB boot option
  @retval FALSE    Device is not a valid USB boot option
 **/
BOOLEAN
EFIAPI
MsBootPolicyLibIsDevicePathUsb (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return PlatformIsDevicePathUsb (DevicePath);
}

/**
 *Ask if the platform for the boot sequence

 *@retval EFI_SUCCESS  BootSequence pointer returned
 *@retval Other        Error getting boot sequence

 BootSequence is assumed to be a pointer to constant data, and
 is not freed by the caller.

**/
EFI_STATUS
EFIAPI
MsBootPolicyLibGetBootSequence (
  BOOT_SEQUENCE  **BootSequence,
  BOOLEAN        AltBootRequest
  )
{
  if (BootSequence == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (AltBootRequest) {
    *BootSequence = BootSequenceUPH;
    DEBUG ((DEBUG_INFO, "%a - returing alt boot sequence\n", __FUNCTION__));
  } else {
    DEBUG ((DEBUG_INFO, "%a - returing normal sequence\n", __FUNCTION__));
    *BootSequence = BootSequenceHUP;
  }

  return EFI_SUCCESS;
}
