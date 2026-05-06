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
#include <Protocol/Hash.h>

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
  if (CurrentFile == NULL || !CurrentFileSize) {
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

VOID
ManageSiPolicy (
  IN EFI_FILE_PROTOCOL *FsVolume,
  IN EFI_FILE_HANDLE    File)
{
  EFI_STATUS  Status                = EFI_SUCCESS;
  UINT8      *FileContent           = NULL;
  UINT8      *CustomFileContent     = NULL;
  UINTN       FileContentSize       = 0;
  UINTN       CustomFileContentSize = 0;

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

    // Show Progress
    DEBUG ((EFI_D_WARN, "%a: Successfully Wrote SiPolicy Data\n", __FUNCTION__));
  }

cleanup:
  // Free Custom SiPolicy Content Buffer
  if (CustomFileContent != NULL) {
    FreePool (CustomFileContent);
  }

  // Free SiPolicy Content Buffer
  if (FileContent != NULL) {
    FreePool (FileContent);
  }
}
#endif

VOID
EFIAPI
FileSystemCallback (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  EFI_STATUS                       Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *mSfsProtocol;
  EFI_FILE_PROTOCOL               *FsVolume;
  EFI_FILE_HANDLE                  BootmgfwFile;
  EFI_FILE_HANDLE                  SiPolicyFile;
  EFI_HANDLE                      *SfsProtocolHandle;
  UINTN                            NumOfHandles;

  // Locate SFS Protocol Handle
  Status = gBS->LocateHandleBuffer (ByRegisterNotify, NULL, RegisteredSfsProtocol, &NumOfHandles, &SfsProtocolHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SFS Protocol Handle! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Locate SFS Protocol
  Status = gBS->HandleProtocol (SfsProtocolHandle[0], &gEfiSimpleFileSystemProtocolGuid, (VOID *)&mSfsProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate SFS Protocol! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Open FS Volume
  Status = mSfsProtocol->OpenVolume (mSfsProtocol, &FsVolume);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Open FS Volume! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Verify "bootmgfw.efi" Existence
  Status = FsVolume->Open (FsVolume, &BootmgfwFile, EFI_BOOTMGFW_FILE_PATH, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_SYSTEM);
  if (EFI_ERROR (Status)) {
    goto cleanup;
  }

  // Close "bootmgfw.efi"
  FsVolume->Close (BootmgfwFile);

  // Open current "SiPolicy.p7b" File
  Status = FsVolume->Open (FsVolume, &SiPolicyFile, EFI_SIPOLICY_FILE_PATH, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Open \"SiPolicy.p7b\"! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

#if ENABLE_SECUREBOOT == 1
  // Create "SiPolicy.p7b" File
  if (Status == EFI_NOT_FOUND) {
    Status = FsVolume->Open (FsVolume, &SiPolicyFile, EFI_SIPOLICY_FILE_PATH, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Create \"SiPolicy.p7b\"! Status = %r\n", __FUNCTION__, Status));
      goto cleanup;
    }
  }

  // Manage SiPolicy
  ManageSiPolicy (FsVolume, SiPolicyFile);

  // Close SiPolicy
  FsVolume->Close (SiPolicyFile);
#else
  // Check SiPolicy Presence
  if (Status == EFI_NOT_FOUND) {
    goto cleanup;
  }

  // Delete "SiPolicy.p7b" File
  Status = FsVolume->Delete (SiPolicyFile);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Delete \"SiPolicy.p7b\"! Status = %r\n", __FUNCTION__, Status));
    goto cleanup;
  }

  // Show Progress
  DEBUG ((EFI_D_WARN, "%a: Successfully Deleted SiPolicy\n", __FUNCTION__));
#endif

cleanup:
  // Free Buffer
  if (SfsProtocolHandle != NULL) {
    FreePool (SfsProtocolHandle);
  }
}

EFI_STATUS
SetupSecureBoot ()
{
  EFI_STATUS Status;
  EFI_EVENT  CallbackEvent;

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

  return EFI_SUCCESS;
}
