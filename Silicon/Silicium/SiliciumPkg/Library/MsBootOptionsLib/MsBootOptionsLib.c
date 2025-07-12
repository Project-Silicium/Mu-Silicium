/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>

EFI_STATUS
BuildFwLoadOption (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  IN     EFI_GUID                     *FwFile,
  IN     CHAR8                        *Parameter)
{
  EFI_STATUS                         Status               = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath           = NULL;
  EFI_LOADED_IMAGE_PROTOCOL         *mLoadedImageProtocol = NULL;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode             = {0};
  CHAR16                            *Description          = NULL;
  UINTN                              DescriptionLength    = 0;

  // Get Defined Application Name from FV
  Status = GetSectionFromFv (FwFile, EFI_SECTION_USER_INTERFACE, 0, (VOID *)&Description, &DescriptionLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get Application Name of %a! Status = %r\n", __FUNCTION__, FwFile, Status));
    return Status;
  }

  // Init FW Volume Device Path Node
  EfiInitializeFwVolDevicepathNode (&FileNode, FwFile);

  // Locate Loaded Image Protocol Handle
  Status = gBS->HandleProtocol (gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID *)&mLoadedImageProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Loaded Image Protocol Handle! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Device Path
  DevicePath = AppendDevicePathNode (DevicePathFromHandle (mLoadedImageProtocol->DeviceHandle), (EFI_DEVICE_PATH_PROTOCOL *)&FileNode);
  if (DevicePath == NULL) {
    return EFI_NOT_FOUND;
  }

  // Init Boot Load Option
  Status = EfiBootManagerInitializeLoadOption (BootOption, LoadOptionNumberUnassigned, LoadOptionTypeBoot, LOAD_OPTION_CATEGORY_APP | LOAD_OPTION_ACTIVE | LOAD_OPTION_HIDDEN, Description, DevicePath, (UINT8 *)Parameter, (Parameter != NULL) ? (UINT32)AsciiStrSize (Parameter) : 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Initialize Boot Load Option! Status = %r\n", __FUNCTION__, Status));

    // Free Buffer
    FreePool (DevicePath);

    return Status;
  }

  // Free Buffers
  FreePool (DevicePath);
  FreePool (Description);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MsBootOptionsLibGetDefaultBootApp (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  IN     CHAR8                        *Parameter)
{
  // Return Special App Boot Ooption
  return BuildFwLoadOption (BootOption, FixedPcdGetPtr (PcdSpecialApp), Parameter);
}

EFI_STATUS
EFIAPI
MsBootOptionsLibGetBootManagerMenu (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  IN     CHAR8                        *Parameter)
{
  // Return UEFI Menu Boot Option
  return BuildFwLoadOption (BootOption, PcdGetPtr (PcdBootManagerMenuFile), Parameter);
}

EFI_STATUS
CreateFvBootOption (
  IN     EFI_GUID                     *FileGuid,
  IN     CHAR16                       *Description,
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  IN     UINT32                        Attributes,
  IN     UINT8                        *OptionalData,
  IN     UINT32                        OptionalDataSize OPTIONAL)
{
  EFI_STATUS                         Status               = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath           = NULL;
  EFI_LOADED_IMAGE_PROTOCOL         *mLoadedImageProtocol = NULL;
  EFI_FIRMWARE_VOLUME2_PROTOCOL     *mFv2Protocol         = NULL;
  VOID                              *Buffer               = NULL;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode             = {0};
  UINTN                              Size                 = 0;
  UINT32                             AuthenticationStatus = 0;

  // Check Parameters
  if ((BootOption == NULL) || (FileGuid == NULL) || (Description == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Init FW Volume Device Path Node
  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);

  // Locate Loaded Image Handle Protocol
  Status = gBS->HandleProtocol (gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID *)&mLoadedImageProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Loaded Image Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate FV2 Handle Protocol
  Status = gBS->HandleProtocol (mLoadedImageProtocol->DeviceHandle, &gEfiFirmwareVolume2ProtocolGuid, (VOID *)&mFv2Protocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate FV2 Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read EFI Application Section
  Status = mFv2Protocol->ReadSection (mFv2Protocol, FileGuid, EFI_SECTION_PE32, 0, &Buffer, &Size, &AuthenticationStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read EFI Application Section from %a! Status = %r\n", __FUNCTION__, FileGuid, Status));
    return Status;
  }

  // Get Device Path
  DevicePath = AppendDevicePathNode (DevicePathFromHandle (mLoadedImageProtocol->DeviceHandle), (EFI_DEVICE_PATH_PROTOCOL *)&FileNode);
  if (DevicePath == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Device Path of EFI Application! Status = %r\n", __FUNCTION__, Status));

    // Free Buffer
    FreePool (Buffer);

    return EFI_OUT_OF_RESOURCES;
  }

  // Init Boot Option
  Status = EfiBootManagerInitializeLoadOption (BootOption, LoadOptionNumberUnassigned, LoadOptionTypeBoot, Attributes, Description, DevicePath, OptionalData, OptionalDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Init Boot Option for %a! Status = %r\n", __FUNCTION__, FileGuid, Status));

    // Free Buffers
    FreePool (Buffer);
    FreePool (DevicePath);

    return Status;
  }

  // Free Buffers
  FreePool (Buffer);
  FreePool (DevicePath);

  return EFI_SUCCESS;
}

STATIC
UINTN
RegisterFvBootOption (
  IN EFI_GUID *FileGuid,
  IN CHAR16   *Description,
  IN UINTN     Position,
  IN UINT32    Attributes,
  IN UINT8    *OptionalData, 
  IN UINT32    OptionalDataSize OPTIONAL)
{
  EFI_STATUS                    Status          = EFI_SUCCESS;
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions     = NULL;
  EFI_BOOT_MANAGER_LOAD_OPTION  NewOption       = {0};
  UINTN                         OptionIndex     = 0;
  UINTN                         BootOptionCount = 0;

  // Set Dummy Option Number
  NewOption.OptionNumber = LoadOptionNumberUnassigned;

  // Create a New FV Boot Option
  Status = CreateFvBootOption (FileGuid, Description, &NewOption, Attributes, OptionalData, OptionalDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create a New FV Boot Option! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Boot Load Options
  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

  // Find Boot Load Option
  OptionIndex = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);

  // Check Boot Option Index
  if (OptionIndex == -1) {
    // Update Boot Attribute
    NewOption.Attributes ^= LOAD_OPTION_ACTIVE;

    // Get new Boot Option Index
    OptionIndex = EfiBootManagerFindLoadOption (&NewOption, BootOptions, BootOptionCount);

    // Update Boot Attribute
    NewOption.Attributes ^= LOAD_OPTION_ACTIVE;
  }

  // Check Boot Option Index
  if (OptionIndex == -1) {
    // Add Boot Load Option Variable
    Status = EfiBootManagerAddLoadOptionVariable (&NewOption, Position);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Add New Boot Load Option Variable! Status = %r\n", __FUNCTION__, Status));

      // Free Buffers
      EfiBootManagerFreeLoadOption  (&NewOption);
      EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);

      return Status;
    }
  } else {
    // Update Boot Option Number
    NewOption.OptionNumber = BootOptions[OptionIndex].OptionNumber;
  }

  // Free Buffers
  EfiBootManagerFreeLoadOption  (&NewOption);
  EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);

  return NewOption.OptionNumber;
}

VOID
EFIAPI
MsBootOptionsLibRegisterDefaultBootOptions ()
{
  // Register Internal Storage Boot Option
  RegisterFvBootOption (&gMsBootPolicyFileGuid, L"Internal Storage", (UINTN)-1, LOAD_OPTION_ACTIVE, (UINT8 *)"SSD", sizeof ("SSD"));

  // Register USB Storage Boot Ooption
  RegisterFvBootOption (&gMsBootPolicyFileGuid, L"USB Storage", (UINTN)-1, LOAD_OPTION_ACTIVE, (UINT8 *)"USB", sizeof ("USB"));
}

EFI_BOOT_MANAGER_LOAD_OPTION*
EFIAPI
MsBootOptionsLibGetDefaultOptions (OUT UINTN *OptionCount)
{
  EFI_STATUS                    Status           = EFI_SUCCESS;
  EFI_BOOT_MANAGER_LOAD_OPTION *Option           = NULL;
  UINTN                         LocalOptionCount = 2;

  // Allocate Memory
  Option = (EFI_BOOT_MANAGER_LOAD_OPTION *)AllocateZeroPool (sizeof (EFI_BOOT_MANAGER_LOAD_OPTION) * LocalOptionCount);
  if (Option == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for Boot Options!\n", __FUNCTION__));
    return NULL;
  }

  // Create FV Boot Options
  Status  = CreateFvBootOption (&gMsBootPolicyFileGuid, L"Internal Storage", &Option[0], LOAD_OPTION_ACTIVE, (UINT8 *)"SSD", sizeof ("SSD"));
  Status |= CreateFvBootOption (&gMsBootPolicyFileGuid, L"USB Storage",      &Option[1], LOAD_OPTION_ACTIVE, (UINT8 *)"USB", sizeof ("USB"));
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create FV Boot Options! Status = %r\n", __FUNCTION__, Status));

    // Free Buffer
    FreePool (Option);

    // Reset Option Count
    *OptionCount = 0;

    return NULL;
  }

  // Pass Option Count
  *OptionCount = LocalOptionCount;

  return Option;
}
