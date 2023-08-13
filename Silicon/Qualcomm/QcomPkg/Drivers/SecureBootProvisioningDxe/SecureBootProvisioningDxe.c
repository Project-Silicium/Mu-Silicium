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
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/MuSecureBootKeySelectorLib.h>

#include "SystemIntegrityPolicyDefaultVars.h"

//
// Global variables.
//
VOID                 *mFileSystemRegistration = NULL;
STATIC EFI_IMAGE_LOAD EfiImageLoad            = NULL;

EFI_STATUS
EFIAPI
TryWritePlatformSiPolicy(EFI_HANDLE SfsHandle)
{
  EFI_STATUS Status           = EFI_SUCCESS;
  UINT64 mSiPolicyDefaultSize = sizeof(mSiPolicyDefault);

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *EfiSfsProtocol;
  EFI_FILE_PROTOCOL               *FileProtocol;
  EFI_FILE_PROTOCOL               *PayloadFileProtocol;

  DEBUG((DEBUG_ERROR, "%a: HandleProtocol gEfiSimpleFileSystemProtocolGuid\n", __FUNCTION__));

  Status = gBS->HandleProtocol(
      SfsHandle, &gEfiSimpleFileSystemProtocolGuid,
      (VOID **)&EfiSfsProtocol);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to invoke HandleProtocol.\n"));
    goto exit;
  }

  DEBUG((DEBUG_ERROR, "%a: OpenVolume EfiSfsProtocol\n", __FUNCTION__));

  Status = EfiSfsProtocol->OpenVolume(EfiSfsProtocol, &FileProtocol);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Fail to get file protocol handle\n"));
    goto exit;
  }

  DEBUG((DEBUG_ERROR, "%a: FileProtocol \\EFI\\Microsoft\\Boot\\bootmgfw.efi\n", __FUNCTION__));

  Status = FileProtocol->Open(
      FileProtocol, &PayloadFileProtocol,
      L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi", EFI_FILE_MODE_READ,
      EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to open bootmgfw.efi: %r\n", Status));
    goto exit;
  }

  DEBUG((DEBUG_ERROR, "%a: Close \\EFI\\Microsoft\\Boot\\bootmgfw.efi\n", __FUNCTION__));

  Status = PayloadFileProtocol->Close(PayloadFileProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to close bootmgfw.efi: %r\n", Status));
  }

  DEBUG((DEBUG_ERROR, "%a: Open \\EFI\\Microsoft\\Boot\\SiPolicy.p7b\n", __FUNCTION__));

  Status = FileProtocol->Open(
      FileProtocol, &PayloadFileProtocol,
      L"\\EFI\\Microsoft\\Boot\\SiPolicy.p7b",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);

  if (EFI_ERROR(Status) && Status != EFI_NOT_FOUND) {
    DEBUG((DEBUG_ERROR, "Failed to open SiPolicy.p7b: %r\n", Status));
    goto exit;
  }

  if (Status != EFI_NOT_FOUND) {
    PayloadFileProtocol->Delete(PayloadFileProtocol);
  }

  Status = FileProtocol->Open(
      FileProtocol, &PayloadFileProtocol,
      L"\\EFI\\Microsoft\\Boot\\SiPolicy.p7b",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to open SiPolicy.p7b: %r\n", Status));
    goto exit;
  }

  DEBUG((DEBUG_ERROR, "%a: Write \\EFI\\Microsoft\\Boot\\SiPolicy.p7b\n", __FUNCTION__));

  Status = PayloadFileProtocol->Write(
      PayloadFileProtocol, &mSiPolicyDefaultSize, mSiPolicyDefault);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to write SiPolicy.p7b: %r\n", Status));
  }

  DEBUG((DEBUG_ERROR, "%a: Close \\EFI\\Microsoft\\Boot\\SiPolicy.p7b\n", __FUNCTION__));

  Status = PayloadFileProtocol->Close(PayloadFileProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to close SiPolicy.p7b: %r\n", Status));
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
VOID
EFIAPI
OnFileSystemNotification (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  UINTN       HandleCount;
  EFI_HANDLE  *HandleBuffer;
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a: Entry...\n", __FUNCTION__));

  for ( ; ;) {
    //
    // Get the next handle
    //
    Status = gBS->LocateHandleBuffer (
                    ByRegisterNotify,
                    NULL,
                    mFileSystemRegistration,
                    &HandleCount,
                    &HandleBuffer
                    );
    //
    // If not found, or any other error, we're done
    //
    if (EFI_ERROR (Status)) {
      break;
    }

    // Spec says we only get one at a time using ByRegisterNotify
    ASSERT (HandleCount == 1);

    DEBUG ((DEBUG_INFO, "%a: processing a potential efiesp device on handle %p\n", __FUNCTION__, HandleBuffer[0]));

    TryWritePlatformSiPolicy (HandleBuffer[0]);

    FreePool (HandleBuffer);
  }
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
ProcessFileSystemRegistration (
  VOID
  )
{
  EFI_EVENT   FileSystemCallBackEvent;
  EFI_STATUS  Status;

  //
  // Always register for file system notifications.  They may arrive at any time.
  //
  DEBUG ((DEBUG_INFO, "Registering for file systems notifications\n"));
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnFileSystemNotification,
                  NULL,
                  &FileSystemCallBackEvent
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to create callback event (%r)\n", __FUNCTION__, Status));
    goto Cleanup;
  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiSimpleFileSystemProtocolGuid,
                  FileSystemCallBackEvent,
                  &mFileSystemRegistration
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to register for file system notifications (%r)\n", __FUNCTION__, Status));
    gBS->CloseEvent (FileSystemCallBackEvent);
    goto Cleanup;
  }

  //
  // Process any existing File System that were present before the registration.
  //
  OnFileSystemNotification (FileSystemCallBackEvent, NULL);

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
  Status  = SetSecureBootConfig(0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Failed to enroll SecureBoot config %r!\n", __FUNCTION__, Status));
    goto exit;
  }

  Status =  ProcessFileSystemRegistration();

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Failed to process file system registration %r!\n", __FUNCTION__, Status));
    goto exit;
  }

exit:
  return Status;
}