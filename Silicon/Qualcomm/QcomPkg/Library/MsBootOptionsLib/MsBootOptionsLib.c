/** @file
 *MsBootOptionsLib  - Ms Extensions to BdsDxe.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsBootOptions.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#define MS_SDD_BOOT       L"Internal Storage"
#define MS_SDD_BOOT_PARM  "SDD"
#define MS_USB_BOOT       L"USB Storage"
#define MS_USB_BOOT_PARM  "USB"

typedef struct {
  MEDIA_FW_VOL_DEVICE_PATH    FvDevPath;
  EFI_DEVICE_PATH_PROTOCOL    EndDevPath;
} FV_PIWG_DEVICE_PATH;

FV_PIWG_DEVICE_PATH  mFvPIWGDevicePathTemplate = {
  {
    {
      MEDIA_DEVICE_PATH,
      MEDIA_PIWG_FW_VOL_DP,
      {
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH)),
        (UINT8)(sizeof (MEDIA_FW_VOL_DEVICE_PATH) >> 8)
      }
    },
    { 0 }
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      END_DEVICE_PATH_LENGTH,
      0
    }
  }
};

/**
 * Constructor
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MsBootOptionsLibEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}

/**
 * Build a firmware load option using file and parameter
 *
 * @param BootOption
 * @param FwFile
 * @param Parameter
 *
 * @return EFI_STATUS
 */
static
EFI_STATUS
BuildFwLoadOption (
  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  EFI_GUID                      *FwFile,
  CHAR8                         *Parameter
  )
{
  EFI_STATUS                         Status;
  CHAR16                             *Description;
  UINTN                              DescriptionLength;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;

  Status = GetSectionFromFv (
             FwFile,
             EFI_SECTION_USER_INTERFACE,
             0,
             (VOID **)&Description,
             &DescriptionLength
             );
  if (EFI_ERROR (Status)) {
    Description = NULL;
  }

  EfiInitializeFwVolDevicepathNode (&FileNode, FwFile);
  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **)&LoadedImage
                  );
  ASSERT_EFI_ERROR (Status);
  DevicePath = AppendDevicePathNode (
                 DevicePathFromHandle (LoadedImage->DeviceHandle),
                 (EFI_DEVICE_PATH_PROTOCOL *)&FileNode
                 );
  // MU_CHANGE [BEGIN] - CodeQL change
  if (DevicePath == NULL) {
    ASSERT (DevicePath != NULL);
    return EFI_NOT_FOUND;
  }

  // MU_CHANGE [END] - CodeQL change

  Status = EfiBootManagerInitializeLoadOption (
             BootOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             LOAD_OPTION_CATEGORY_APP | LOAD_OPTION_ACTIVE | LOAD_OPTION_HIDDEN,
             (Description != NULL) ? Description : L"Boot Manager Menu",
             DevicePath,
             (UINT8 *)Parameter,
             (Parameter != NULL)  ? (UINT32)AsciiStrSize (Parameter) : 0
             );
  ASSERT_EFI_ERROR (Status);
  FreePool (DevicePath);
  if (Description != NULL) {
    FreePool (Description);
  }

  return Status;
}

/**
 * GetDefaultBootApp - the application that implements the default boot order
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MsBootOptionsLibGetDefaultBootApp (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  )
{
  return BuildFwLoadOption (BootOption, &gMsBootPolicyFileGuid, Parameter);
}

/**
  Return the boot option corresponding to the Slot Switch App.

  @param BootOption     Return a created Slot Switch App with the parameter passed

  @retval EFI_SUCCESS   The Slot Switch App is successfully returned.
  @retval Status        Return status of gRT->SetVariable (). BootOption still points
                        to the Slot Switch App even the Status is not EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MsBootOptionsLibSlotSwitchApp (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  )
{
  return BuildFwLoadOption (BootOption, PcdGetPtr (PcdSlotSwitchApp), Parameter);
}

/**
  Return the boot option corresponding to the UEFI Shell.

  @param BootOption     Return a created UEFI Shell with the parameter passed

  @retval EFI_SUCCESS   The UEFI Shell is successfully returned.
  @retval Status        Return status of gRT->SetVariable (). BootOption still points
                        to the Slot UEFI Shell even the Status is not EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MsBootOptionsLibUEFIShell (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  )
{
  return BuildFwLoadOption (BootOption, PcdGetPtr (PcdShellFile), Parameter);
}

/**
  Return the boot option corresponding to the Boot Manager Menu.

  @param BootOption    Return a created Boot Manager Menu with the parameter passed
  @param Parameter     The parameter to add to the BootOption

  @retval EFI_SUCCESS   The Boot Manager Menu is successfully returned.
  @retval Status        Return status of gRT->SetVariable (). BootOption still points
                        to the Boot Manager Menu even the Status is not EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MsBootOptionsLibGetBootManagerMenu (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  )
{
  return BuildFwLoadOption (BootOption, PcdGetPtr (PcdBootManagerMenuFile), Parameter);
}

/**
 * Create a boot option
 *
 * @return EFI_STATUS
 */
static
EFI_STATUS
CreateFvBootOption (
  EFI_GUID *FileGuid,
  CHAR16 *Description,
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  UINT32 Attributes,
  UINT8 *OptionalData, OPTIONAL
  UINT32                       OptionalDataSize
  )
{
  EFI_STATUS                         Status;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;
  EFI_FIRMWARE_VOLUME2_PROTOCOL      *Fv;
  UINT32                             AuthenticationStatus;
  VOID                               *Buffer;
  UINTN                              Size;

  if ((BootOption == NULL) || (FileGuid == NULL) || (Description == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);

  if (!CompareGuid (PcdGetPtr (PcdShellFile), FileGuid)) {
    Status = gBS->HandleProtocol (
                    gImageHandle,
                    &gEfiLoadedImageProtocolGuid,
                    (VOID **)&LoadedImage
                    );
    if (!EFI_ERROR (Status)) {
      Status = gBS->HandleProtocol (
                      LoadedImage->DeviceHandle,
                      &gEfiFirmwareVolume2ProtocolGuid,
                      (VOID **)&Fv
                      );
      if (!EFI_ERROR (Status)) {
        Buffer = NULL;
        Size   = 0;
        Status = Fv->ReadSection (
                       Fv,
                       FileGuid,
                       EFI_SECTION_PE32,
                       0,
                       &Buffer,
                       &Size,
                       &AuthenticationStatus
                       );
        if (Buffer != NULL) {
          FreePool (Buffer);
        }
      }
    }

    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    DevicePath = AppendDevicePathNode (
                   DevicePathFromHandle (LoadedImage->DeviceHandle),
                   (EFI_DEVICE_PATH_PROTOCOL *)&FileNode
                   );
    if (DevicePath == NULL) {
      ASSERT (DevicePath != NULL);
      return EFI_OUT_OF_RESOURCES;
    }
  }

  Status = EfiBootManagerInitializeLoadOption (
             BootOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             Attributes,
             Description,
             DevicePath,
             OptionalData,
             OptionalDataSize
             );

  if (DevicePath != NULL) {
    FreePool (DevicePath);
  }

  return Status;
}

/**
 * Register a boot option
 *
 * @param FileGuid          The guid associated with the boot option.
 * @param Description       Description of the boot option.
 * @param Position          The position of the load option to put in the ****Order variable.
 * @param Attributes        The attributes of the boot option.
 * @param OptionalData      Optional data of the boot option.
 * @param OptionalDataSize  Size of the optional data of the boot option.
 *
 * @return UINTN      If boot option number of the registered boot option
 *
 */
static
UINTN
RegisterFvBootOption (
  EFI_GUID *FileGuid,
  CHAR16 *Description,
  UINTN Position,
  UINT32 Attributes,
  UINT8 *OptionalData, OPTIONAL
  UINT32                           OptionalDataSize
  )
{
  EFI_STATUS                    Status;
  UINTN                         OptionIndex;
  EFI_BOOT_MANAGER_LOAD_OPTION  NewOption;
  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOptions;
  UINTN                         BootOptionCount;

  NewOption.OptionNumber = LoadOptionNumberUnassigned;
  Status                 = CreateFvBootOption (FileGuid, Description, &NewOption, Attributes, OptionalData, OptionalDataSize);
  if (!EFI_ERROR (Status)) {
    BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

    if (BootOptions == NULL) {
      DEBUG ((DEBUG_INFO, "No boot options found. Proceeding to add boot options.\n"));
    }

    OptionIndex = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);
    if (OptionIndex == -1) {
      NewOption.Attributes ^= LOAD_OPTION_ACTIVE;
      OptionIndex           = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);
      NewOption.Attributes ^= LOAD_OPTION_ACTIVE;
    }

    if (OptionIndex == -1) {
      Status = EfiBootManagerAddLoadOptionVariable (&NewOption, Position);
      DEBUG ((DEBUG_INFO, "Added   Boot option as Boot%04x - %s\n", NewOption.OptionNumber, Description));
      ASSERT_EFI_ERROR (Status);
    } else {
      NewOption.OptionNumber = BootOptions[OptionIndex].OptionNumber;
      DEBUG ((DEBUG_INFO, "Reusing Boot option as Boot%04x - %s\n", NewOption.OptionNumber, Description));
    }

    EfiBootManagerFreeLoadOption (&NewOption);
    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
  }

  return NewOption.OptionNumber;
}

/**
 * Register Default Boot Options
 *
 * @param
 *
 * @return VOID EFIAPI
 */
VOID
EFIAPI
MsBootOptionsLibRegisterDefaultBootOptions (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  RegisterFvBootOption (&gMsBootPolicyFileGuid, MS_SDD_BOOT, (UINTN)-1, LOAD_OPTION_ACTIVE, (UINT8 *)MS_SDD_BOOT_PARM, sizeof (MS_SDD_BOOT_PARM));
  RegisterFvBootOption (&gMsBootPolicyFileGuid, MS_USB_BOOT, (UINTN)-1, LOAD_OPTION_ACTIVE, (UINT8 *)MS_USB_BOOT_PARM, sizeof (MS_USB_BOOT_PARM));
}

/**
 * Get default boot options
 *
 * @param OptionCount
 *
 * @return EFI_BOOT_MANAGER_LOAD_OPTION*EFIAPI
 */
EFI_BOOT_MANAGER_LOAD_OPTION *
EFIAPI
MsBootOptionsLibGetDefaultOptions (
  OUT UINTN  *OptionCount
  )
{
  UINTN                         LocalOptionCount      = 2;
  EFI_BOOT_MANAGER_LOAD_OPTION  *Option;
  EFI_STATUS                    Status;

  Option = (EFI_BOOT_MANAGER_LOAD_OPTION *)AllocateZeroPool (sizeof (EFI_BOOT_MANAGER_LOAD_OPTION) * LocalOptionCount);
  ASSERT (Option != NULL);
  if (Option == NULL) {
    *OptionCount = 0;
    return NULL;
  }

  Status  = CreateFvBootOption (&gMsBootPolicyFileGuid, MS_SDD_BOOT, &Option[0], LOAD_OPTION_ACTIVE, (UINT8 *)MS_SDD_BOOT_PARM, sizeof (MS_SDD_BOOT_PARM));
  Status |= CreateFvBootOption (&gMsBootPolicyFileGuid, MS_USB_BOOT, &Option[1], LOAD_OPTION_ACTIVE, (UINT8 *)MS_USB_BOOT_PARM, sizeof (MS_USB_BOOT_PARM));

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Error creating defatult boot options\n", __FUNCTION__));
    FreePool (Option);
    Option                = NULL;
    LocalOptionCount      = 0;
  }

  *OptionCount = LocalOptionCount;

  return Option;
}
