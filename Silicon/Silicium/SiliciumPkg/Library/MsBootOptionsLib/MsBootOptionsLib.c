/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

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

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>

EFI_STATUS
BuildFwLoadOption (
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  EFI_GUID                     *FwFile,
  CHAR8                        *Parameter)
{
  EFI_STATUS                         Status            = EFI_SUCCESS;
  CHAR16                            *Description       = NULL;
  UINTN                              DescriptionLength = 0;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath        = NULL;
  EFI_LOADED_IMAGE_PROTOCOL         *LoadedImage       = NULL;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;

  // Get Defined Application from FV
  Status = GetSectionFromFv (FwFile, EFI_SECTION_USER_INTERFACE, 0, (VOID **)&Description, &DescriptionLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get Description from %a! Status = %r\n", __FUNCTION__, FwFile, Status));
    Description = NULL;
  }

  // Init FW Volume Device Path Node
  EfiInitializeFwVolDevicepathNode (&FileNode, FwFile);

  // Locate Loaded Image Handle Protocol
  Status = gBS->HandleProtocol (gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&LoadedImage);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Loaded Image Handle Protocol!\n", __FUNCTION__));
    goto exit;
  }

  DevicePath = AppendDevicePathNode (DevicePathFromHandle (LoadedImage->DeviceHandle), (EFI_DEVICE_PATH_PROTOCOL *)&FileNode);

  // MU_CHANGE [BEGIN] - CodeQL change
  if (DevicePath == NULL) {
    ASSERT (DevicePath != NULL);
    Status = EFI_NOT_FOUND;
    goto exit;
  }
  // MU_CHANGE [END] - CodeQL change

  // Init Boot Load Option
  Status = EfiBootManagerInitializeLoadOption (BootOption, LoadOptionNumberUnassigned, LoadOptionTypeBoot, LOAD_OPTION_CATEGORY_APP | LOAD_OPTION_ACTIVE | LOAD_OPTION_HIDDEN, (Description != NULL) ? Description : L"Boot Manager Menu", DevicePath, (UINT8 *)Parameter, (Parameter != NULL)  ? (UINT32)AsciiStrSize (Parameter) : 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Initialize Boot Load Option!\n"));
    goto exit;
  }

  FreePool (DevicePath);

  if (Description != NULL) {
    FreePool (Description);
  }

exit:
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
EFIAPI
MsBootOptionsLibGetDefaultBootApp (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  IN     CHAR8                        *Parameter)
{
  return BuildFwLoadOption (BootOption, FixedPcdGetPtr (PcdSpecialApp), Parameter);
}

EFI_STATUS
EFIAPI
MsBootOptionsLibGetBootManagerMenu (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  IN     CHAR8                        *Parameter)
{
  return BuildFwLoadOption (BootOption, PcdGetPtr (PcdBootManagerMenuFile), Parameter);
}

STATIC
EFI_STATUS
CreateFvBootOption (
  EFI_GUID                     *FileGuid,
  CHAR16                       *Description,
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  UINT32                        Attributes,
  UINT8                        *OptionalData,
  UINT32                        OptionalDataSize OPTIONAL)
{
  EFI_STATUS                         Status               = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath           = NULL;
  EFI_LOADED_IMAGE_PROTOCOL         *LoadedImage          = NULL;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;
  EFI_FIRMWARE_VOLUME2_PROTOCOL     *Fv                   = NULL;
  UINT32                             AuthenticationStatus = 0;
  VOID                              *Buffer               = NULL;
  UINTN                              Size                 = 0;

  if ((BootOption == NULL) || (FileGuid == NULL) || (Description == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Init FW Volume Device Path Node
  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);

  // Locate Loaded Image Handle Protocol
  Status = gBS->HandleProtocol (gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&LoadedImage);

  if (!EFI_ERROR (Status)) {
    // Locate FV2 Handle Protocol
    Status = gBS->HandleProtocol (LoadedImage->DeviceHandle, &gEfiFirmwareVolume2ProtocolGuid, (VOID **)&Fv);

    if (!EFI_ERROR (Status)) {
      Buffer = NULL;
      Size   = 0;

      // Read EFI Application EFI File
      Status = Fv->ReadSection (Fv, FileGuid, EFI_SECTION_PE32, 0, &Buffer, &Size, &AuthenticationStatus);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Read EFI Section from %a! Status = %r\n", __FUNCTION__, FileGuid, Status));
        goto exit;
      }
    } else {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate FV2 Handle Protocol! Status = %r\n", __FUNCTION__, Status));
      goto exit;
    }
  } else {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Loaded Image Handle Protocol! Status = %r\n", __FUNCTION__, Status));
    goto exit;
  }

  DevicePath = AppendDevicePathNode (DevicePathFromHandle (LoadedImage->DeviceHandle), (EFI_DEVICE_PATH_PROTOCOL *)&FileNode);
  if (DevicePath == NULL) {
    ASSERT (DevicePath != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    goto exit;
  }

  // Init Boot Option
  Status = EfiBootManagerInitializeLoadOption (BootOption, LoadOptionNumberUnassigned, LoadOptionTypeBoot, Attributes, Description, DevicePath, OptionalData, OptionalDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Init Boot Option for %a! Status = %r\n", __FUNCTION__, FileGuid, Status));
  }

exit:
  if (DevicePath != NULL) {
    FreePool (DevicePath);
  }

  if (Buffer != NULL) {
    FreePool (Buffer);
  }

  return Status;
}

STATIC
UINTN
RegisterFvBootOption (
  EFI_GUID *FileGuid,
  CHAR16   *Description,
  UINTN     Position,
  UINT32    Attributes,
  UINT8    *OptionalData, 
  UINT32    OptionalDataSize OPTIONAL)
{
  EFI_STATUS                    Status          = EFI_SUCCESS;
  UINTN                         OptionIndex     = 0;
  EFI_BOOT_MANAGER_LOAD_OPTION  NewOption;
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions     = NULL;
  UINTN                         BootOptionCount = 0;

  NewOption.OptionNumber = LoadOptionNumberUnassigned;

  // Create a New Fv Boot Option
  Status = CreateFvBootOption (FileGuid, Description, &NewOption, Attributes, OptionalData, OptionalDataSize);
  if (!EFI_ERROR (Status)) {
    BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);
    OptionIndex = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);

    if (OptionIndex == -1) {
      NewOption.Attributes ^= LOAD_OPTION_ACTIVE;
      OptionIndex           = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);
      NewOption.Attributes ^= LOAD_OPTION_ACTIVE;
    }

    if (OptionIndex == -1) {
      // Add Boot Load Option Variable
      Status = EfiBootManagerAddLoadOptionVariable (&NewOption, Position);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Add a New Boot Load Option Variable!\n", __FUNCTION__));
        goto exit;
      }
    } else {
      NewOption.OptionNumber = BootOptions[OptionIndex].OptionNumber;
    }

    EfiBootManagerFreeLoadOption  (&NewOption);
    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
  } else {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create a New FV Boot Option! Status = %r\n", __FUNCTION__, Status));
  }

exit:
  ASSERT_EFI_ERROR (Status);

  return NewOption.OptionNumber;
}

VOID
EFIAPI
MsBootOptionsLibRegisterDefaultBootOptions ()
{
  RegisterFvBootOption (&gMsBootPolicyFileGuid, L"Internal Storage", (UINTN)-1, LOAD_OPTION_ACTIVE, (UINT8 *)"SSD", sizeof ("SSD"));
  RegisterFvBootOption (&gMsBootPolicyFileGuid, L"USB Storage",      (UINTN)-1, LOAD_OPTION_ACTIVE, (UINT8 *)"USB", sizeof ("USB"));
}

EFI_BOOT_MANAGER_LOAD_OPTION*
EFIAPI
MsBootOptionsLibGetDefaultOptions (OUT UINTN *OptionCount)
{
  EFI_STATUS                    Status           = EFI_SUCCESS;
  EFI_BOOT_MANAGER_LOAD_OPTION *Option           = NULL;
  UINTN                         LocalOptionCount = 2;

  Option = (EFI_BOOT_MANAGER_LOAD_OPTION *)AllocateZeroPool (sizeof (EFI_BOOT_MANAGER_LOAD_OPTION) * LocalOptionCount);
  ASSERT (Option != NULL);

  // Create Fv Boot Options
  Status  = CreateFvBootOption (&gMsBootPolicyFileGuid, L"Internal Storage", &Option[0], LOAD_OPTION_ACTIVE, (UINT8 *)"SSD", sizeof ("SSD"));
  Status |= CreateFvBootOption (&gMsBootPolicyFileGuid, L"USB Storage",      &Option[1], LOAD_OPTION_ACTIVE, (UINT8 *)"USB", sizeof ("USB"));

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Boot Options! Status = %r\n", __FUNCTION__, Status));
    FreePool (Option);
    Option           = NULL;
    LocalOptionCount = 0;
  }

  *OptionCount = LocalOptionCount;

  return Option;
}

EFI_STATUS
EFIAPI
MsBootOptionsLibEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  return EFI_SUCCESS;
}
