/**
  Copyright (C) DuoWoA authors. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/SecureBootHandlerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MuSecureBootKeySelectorLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>

#include <Protocol/SimpleFileSystem.h>

#include <Guid/FileInfo.h>

#include "SecureBootHandler.h"

//
// Global Variables
//
STATIC VOID *RegisteredSfsProtocol = NULL;

#if ENABLE_SECUREBOOT == 1
BOOLEAN
ReplaceSiPolicy (
  IN UINT8 *CurrentFile,
  IN UINTN  CurrentFileSize,
  IN UINT8 *NewFile,
  IN UINTN  NewFileSize)
{
  BOOLEAN Status;
  UINT8   CurrentHash[SHA256_DIGEST_SIZE];
  UINT8   NewHash[SHA256_DIGEST_SIZE];

  // Verify Arguments
  if (CurrentFile == NULL || NewFile == NULL) {
    return TRUE;
  }

  // Get Current SiPolicy Hash
  Status = Sha256HashAll (CurrentFile, CurrentFileSize, CurrentHash);
  if (!Status) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get SHA-256 Hash of Current SiPolicy!\n", __FUNCTION__));
    return TRUE;
  }

  // Get New SiPolicy Hash
  Status = Sha256HashAll (NewFile, NewFileSize, NewHash);
  if (!Status) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get SHA-256 Hash of New SiPolicy!\n", __FUNCTION__));
    return TRUE;
  }

  // Compare SiPolicy Hashes
  if (CompareMem (CurrentHash, NewHash, SHA256_DIGEST_SIZE) != 0) {
    return TRUE;
  }

  return FALSE;
}

VOID
GetSiPolicyContent (
  IN  EFI_FILE_PROTOCOL  *FsVolume,
  IN  EFI_FILE_HANDLE     File,
  OUT UINT8             **Content,
  OUT UINTN              *ContentSize)
{
  EFI_STATUS     Status       = EFI_SUCCESS;
  EFI_FILE_INFO *FileInfo     = NULL;
  UINTN          FileInfoSize = 0;

  // Verify Parameters
  if (FsVolume == NULL || File == NULL || Content == NULL || ContentSize == NULL) {
    return;
  }

  // Get File Info Size
  Status = FsVolume->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, NULL);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get File Info Size! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Allocate Memory
  FileInfo = AllocateZeroPool (FileInfoSize);
  if (FileInfo == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for File Info!\n", __FUNCTION__));
    goto cleanup;
  }

  // Get SiPolicy File Infos
  Status = FsVolume->GetInfo (File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get SiPolicy File Info! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Save File Size
  *ContentSize = FileInfo->FileSize;

  // Allocate Memory
  *Content = AllocateZeroPool (*ContentSize);
  if (*Content == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for SiPolicy File Content!\n", __FUNCTION__));
    goto cleanup;
  }

  // Get SiPolicy File Content
  Status = FsVolume->Read (File, ContentSize, *Content);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get SiPolicy File Content! Status = %r\n", __FUNCTION__, Status));
  }

cleanup:
  // Free File Info Buffer
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }
}
#endif

VOID
ManageSiPolicy (
  IN EFI_FILE_PROTOCOL *FsVolume,
  IN EFI_FILE_HANDLE    File)
{
  EFI_STATUS  Status;

  // Verify Parameter
  if (FsVolume == NULL || File == NULL) {
    return;
  }

#if ENABLE_SECUREBOOT == 1
  UINT8 *FileContent           = NULL;
  UINT8 *CustomFileContent     = NULL;
  UINTN  FileContentSize       = 0;
  UINTN  CustomFileContentSize = 0;

  // Get Custom "SiPolicy.p7b" File
  Status = GetSectionFromAnyFv (FixedPcdGetPtr (PcdSiPolicyFile), EFI_SECTION_RAW, 0, (VOID *)&CustomFileContent, &CustomFileContentSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get Custom \"SiPolicy.p7b\" from any FV! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Get current "SiPolicy.p7b" File Content
  GetSiPolicyContent (FsVolume, File, &FileContent, &FileContentSize);

  // Check SiPolicy Hash
  if (ReplaceSiPolicy (FileContent, FileContentSize, CustomFileContent, CustomFileContentSize)) {
    // Write new SiPolicy Data
    Status = FsVolume->Write (File, &CustomFileContentSize, CustomFileContent);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Write new SiPolicy Data! Status = %r\n", __FUNCTION__, Status));
      goto cleanup;
    }
  }

  // Close SiPolicy
  FsVolume->Close (File);

cleanup:
  // Free Custom SiPolicy Content Buffer
  if (CustomFileContent != NULL) {
    FreePool (CustomFileContent);
  }

  // Free SiPolicy Content Buffer
  if (FileContent != NULL) {
    FreePool (FileContent);
  }
#else
  // Delete "SiPolicy.p7b" File
  Status = FsVolume->Delete (File);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Delete \"SiPolicy.p7b\"! Status = %r\n", __FUNCTION__, Status));
    return;
  }
#endif
}

VOID
ProcessFileSystem (IN EFI_FILE_PROTOCOL *FsVolume)
{
  EFI_STATUS      Status;
  EFI_FILE_HANDLE BootmgfwFile;
  EFI_FILE_HANDLE SiPolicyFile;

  // Verify Parameter
  if (FsVolume == NULL) {
    return;
  }

  // Verify "bootmgfw.efi" Existence
  Status = FsVolume->Open (FsVolume, &BootmgfwFile, EFI_BOOTMGFW_FILE_PATH, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_SYSTEM);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Close "bootmgfw.efi"
  FsVolume->Close (BootmgfwFile);

  // Open current "SiPolicy.p7b" File
  Status = FsVolume->Open (FsVolume, &SiPolicyFile, EFI_SIPOLICY_FILE_PATH, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Open \"SiPolicy.p7b\"! Status = %r\n", __FUNCTION__, Status));
    return;
  }

#if ENABLE_SECUREBOOT == 1
  // Create "SiPolicy.p7b" File
  if (Status == EFI_NOT_FOUND) {
    Status = FsVolume->Open (FsVolume, &SiPolicyFile, EFI_SIPOLICY_FILE_PATH, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Create \"SiPolicy.p7b\"! Status = %r\n", __FUNCTION__, Status));
      return;
    }
  }
#else
  // Check SiPolicy Presence
  if (Status == EFI_NOT_FOUND) {
    return;
  }
#endif

  // Manage SiPolicy
  ManageSiPolicy (FsVolume, SiPolicyFile);
}

VOID
EFIAPI
FileSystemCallback (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  EFI_STATUS  Status;
  EFI_HANDLE *SfsBuffer;
  UINTN       SfsCount;

  while (TRUE) {
    // Locate SFS Protocol Handles
    Status = gBS->LocateHandleBuffer (ByRegisterNotify, NULL, RegisteredSfsProtocol, &SfsCount, &SfsBuffer);
    if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SFS Protocol Handles! Status = %r\n", __FUNCTION__, Status));
      continue;
    }

    // Check SFS Handle Presense
    if (Status == EFI_NOT_FOUND) {
      break;
    }

    // Go thru each SFS Handle
    for (UINTN i = 0; i < SfsCount; i++) {
      EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SfsProtocol;
      EFI_FILE_PROTOCOL               *FsVolume;

      // Get SFS Protocol
      Status = gBS->HandleProtocol (SfsBuffer[i], &gEfiSimpleFileSystemProtocolGuid, (VOID *)&SfsProtocol);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to get SFS Protocol! Status = %r\n", __FUNCTION__, Status));
        continue;
      }

      // Open FS Volume
      Status = SfsProtocol->OpenVolume (SfsProtocol, &FsVolume);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Open FS Volume! Status = %r\n", __FUNCTION__, Status));
        continue;
      }

      // Process File System
      ProcessFileSystem (FsVolume);
    }

    // Free Buffer
    FreePool (SfsBuffer);
  }
}

EFI_STATUS
SetupSecureBoot ()
{
  EFI_STATUS  Status;
  EFI_EVENT   CallbackEvent;

#if ENABLE_SECUREBOOT == 1
  // Enroll Secure Boot Keys
  Status = SetSecureBootConfig (0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enroll Secure Boot Keys!\n", __FUNCTION__));
    return Status;
  }
#endif

  // Create Callback Event
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, FileSystemCallback, NULL, &CallbackEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Callback Event!\n", __FUNCTION__));
    return Status;
  }

  // Register Callback Event
  Status = gBS->RegisterProtocolNotify (&gEfiSimpleFileSystemProtocolGuid, CallbackEvent, &RegisteredSfsProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register Callback Event!\n", __FUNCTION__));
    return Status;
  }

  // Process Already Present File Systems
  gBS->SignalEvent (CallbackEvent);

  return EFI_SUCCESS;
}
