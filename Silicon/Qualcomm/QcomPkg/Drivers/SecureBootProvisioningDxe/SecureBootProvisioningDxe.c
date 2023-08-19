/** @file SecureBootProvisioningDxe.c

    This file contains functions for provisioning Secure Boot.

    Copyright (C) DuoWoA authors. All rights reserved.
    Copyright (C) Microsoft Corporation. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>

#include <Library/MuSecureBootKeySelectorLib.h>

#include "SystemIntegrityPolicyDefaultVars.h"

//
// Global variables.
//
EFI_EVENT FileSystemCallBackEvent = NULL;

/**
  Gets the SFS protocol handle for the the first disk that has a GPT partition.

  @param[in]  SfsProtocol            A pointer to the protocol

  @retval     EFI_SUCCESS            Everything worked great and protocol is
  valid
  @retval     EFI_INVALID_PARAMETER  SfsProtocol is NULL
  @retval     EFI_NOT_FOUND          We didn't find a proper protocol handle to
  use
  @retval     Others                 Something went wrong.

*/
EFI_STATUS
UefiGetSfsProtocolHandle(IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL **SfsProtocol)
{
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  BOOLEAN                   Found;
  EFI_HANDLE                Handle;
  EFI_HANDLE               *HandleBuffer;
  UINTN                     Index;
  UINTN                     NumHandles;
  EFI_STRING                PathNameStr;
  EFI_GUID                 *DummyInterface;
  EFI_STATUS                Status;

  if (SfsProtocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status       = EFI_SUCCESS;
  NumHandles   = 0;
  HandleBuffer = NULL;

  // Locate all handles that are using the SFS protocol.
  Status = gBS->LocateHandleBuffer(
      ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &NumHandles,
      &HandleBuffer);

  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "%a: failed to locate all handles using the Simple FS protocol (%r)\n",
         __FUNCTION__, Status));
    goto CleanUp;
  }

  //
  // Search the handles to find one that is on a GPT partition on a hard drive.
  //
  Found = FALSE;
  for (Index = 0; (Index < NumHandles) && (Found == FALSE); Index += 1) {
    Handle = HandleBuffer[Index];

    Status = gBS->HandleProtocol(
        Handle, &gEfiPartTypeSystemPartGuid, (VOID **)&DummyInterface);
    if (!EFI_ERROR(Status)) {
      Found = TRUE;

      DevicePath  = DevicePathFromHandle(Handle);
      PathNameStr = ConvertDevicePathToText(DevicePath, TRUE, TRUE);
      DEBUG(
          (DEBUG_VERBOSE, "%a: found ESP device path %d -> %s\n", __FUNCTION__,
           Index, PathNameStr));

      break;
    }
  }

  // If a suitable handle was not found, return error.
  if (Found == FALSE) {
    Status = EFI_NOT_FOUND;
    DEBUG(
        (DEBUG_ERROR,
         "%a: failed to locate a handle with a GPT handle out of %d handles "
         "from the SFS protocol\n",
         __FUNCTION__, NumHandles));
    goto CleanUp;
  }

  Status = gBS->HandleProtocol(
      HandleBuffer[Index], &gEfiSimpleFileSystemProtocolGuid,
      (VOID **)SfsProtocol);

  if (EFI_ERROR(Status) != FALSE) {
    DEBUG(
        (DEBUG_ERROR,
         "%a: Failed to locate Simple FS protocol using the handle to fs0: %r "
         "\n",
         __FUNCTION__, Status));
    goto CleanUp;
  }

CleanUp:
  DEBUG((DEBUG_VERBOSE, "%a: status %r\n", __FUNCTION__, Status));
  if (HandleBuffer != NULL) {
    FreePool(HandleBuffer);
  }

  return Status;
}

EFI_STATUS
EFIAPI
TryWritePlatformSiPolicy()
{
  EFI_STATUS Status               = EFI_SUCCESS;
  UINT64     mSiPolicyDefaultSize = sizeof(mSiPolicyDefault);

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *EfiSfsProtocol      = NULL;
  EFI_FILE_PROTOCOL               *FileProtocol        = NULL;
  EFI_FILE_PROTOCOL               *PayloadFileProtocol = NULL;

  Status = UefiGetSfsProtocolHandle(&EfiSfsProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to invoke UefiGetSfsProtocolHandle.\n"));
    goto exit;
  }

  Status = EfiSfsProtocol->OpenVolume(EfiSfsProtocol, &FileProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Fail to get file protocol handle\n"));
    goto exit;
  }

  //
  // Check if this is a Windows ESP partition first
  //
  Status = FileProtocol->Open(
      FileProtocol, &PayloadFileProtocol,
      L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi", EFI_FILE_MODE_READ,
      EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to open bootmgfw.efi: %r\n", Status));
    goto exit;
  }

  PayloadFileProtocol->Close(PayloadFileProtocol);

  Status = FileProtocol->Open(
      FileProtocol, &PayloadFileProtocol,
      L"\\EFI\\Microsoft\\Boot\\SiPolicy.p7b",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);

  //
  // If the file already exists, do not touch it
  //
  if (!EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR, "SiPolicy.p7b already exists, do not update: %r\n",
         Status));
    PayloadFileProtocol->Close(PayloadFileProtocol);
    goto exit;
  }

  Status = FileProtocol->Open(
      FileProtocol, &PayloadFileProtocol,
      L"\\EFI\\Microsoft\\Boot\\SiPolicy.p7b",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to open SiPolicy.p7b: %r\n", Status));
    goto exit;
  }

  Status = PayloadFileProtocol->Write(
      PayloadFileProtocol, &mSiPolicyDefaultSize, mSiPolicyDefault);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to write SiPolicy.p7b: %r\n", Status));
    goto exit;
  }

  FileProtocol->Close(FileProtocol);

  if (FileSystemCallBackEvent) {
    gBS->CloseEvent(FileSystemCallBackEvent);
  }

exit:
  return Status;
}

/**
  New File System Discovered.

  Register this device as a possible UEFI Log device.

  @param    Event           Not Used.
  @param    Context         Not Used.

  @retval   none

  This may be called for multiple device arrivals, so the Event is not closed.

**/
VOID EFIAPI OnFileSystemNotification(IN EFI_EVENT Event, IN VOID *Context)
{
  TryWritePlatformSiPolicy();
}

/**
    ProcessFileSystemRegistration

    This function registers for FileSystemProtocols, and then
    checks for any existing FileSystemProtocols, and adds them
    to the LOG_DEVICE list.

    @param       VOID

    @retval      EFI_SUCCESS     FileSystem protocol registration successful
    @retval      error code      Something went wrong.

 **/
EFI_STATUS
ProcessFileSystemRegistration(VOID)
{
  VOID      *mFileSystemRegistration = NULL;
  EFI_STATUS Status;

  //
  // Process any existing File System that were present before the registration.
  //
  Status = TryWritePlatformSiPolicy();
  if (!EFI_ERROR(Status)) {
    goto Cleanup;
  }

  DEBUG((DEBUG_INFO, "Registering for file systems notifications\n"));
  Status = gBS->CreateEvent(
      EVT_NOTIFY_SIGNAL, TPL_CALLBACK, OnFileSystemNotification, NULL,
      &FileSystemCallBackEvent);

  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR, "%a: failed to create callback event (%r)\n",
         __FUNCTION__, Status));
    goto Cleanup;
  }

  Status = gBS->RegisterProtocolNotify(
      &gEfiSimpleFileSystemProtocolGuid, FileSystemCallBackEvent,
      &mFileSystemRegistration);

  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR,
         "%a: failed to register for file system notifications (%r)\n",
         __FUNCTION__, Status));
    gBS->CloseEvent(FileSystemCallBackEvent);
    goto Cleanup;
  }

Cleanup:
  return Status;
}

EFI_STATUS
EFIAPI
SecureBootProvisioningDxeInitialize(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Microsoft Plus 3rd Party Plus Windows On Andromeda
  Status = SetSecureBootConfig(0);
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR, "%a - Failed to enroll SecureBoot config %r!\n",
         __FUNCTION__, Status));
    goto exit;
  }

  /*Status = ProcessFileSystemRegistration();
  if (EFI_ERROR(Status)) {
    DEBUG(
        (DEBUG_ERROR, "%a - Failed to process file system registration %r!\n",
         __FUNCTION__, Status));
    goto exit;
  }*/

exit:
  return Status;
}