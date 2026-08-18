/**
  Copyright (c) 2020, Intel Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/BaseLib.h>

//
// Function implementations
//
STATIC EFI_ACPI_SDT_PROTOCOL   *mAcpiSdtProtocol   = NULL;
STATIC EFI_ACPI_TABLE_PROTOCOL *mAcpiTableProtocol = NULL;
STATIC VOID                    *mRegisteredAcpiSdt = NULL;

EFI_STATUS
EFIAPI
LocateTableBySignature (
  IN  UINT32                        Signature,
  OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  OUT UINTN                        *Handle)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *OrgTable;

  // Verify Protocol Presense
  if (mAcpiSdtProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Verify Parameters
  if (Table == NULL || Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Go thru all ACPI Tables
  for (UINTN i = 0;; i++) {
    EFI_ACPI_TABLE_VERSION TableVersion;

    // Get ACPI Table
    Status = mAcpiSdtProtocol->GetAcpiTable (i, (EFI_ACPI_SDT_HEADER **)&OrgTable, &TableVersion, Handle);
    if (Status == EFI_NOT_FOUND) {
      break;
    } else if (EFI_ERROR (Status)) {
      continue;
    }

    // Compare Table Signatures
    if (OrgTable->Signature != Signature) {
      continue;
    }

    // Copy Table
    *Table = AllocateCopyPool (OrgTable->Length, OrgTable);
    if (*Table == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
LocateTableByOemTableId (
  IN  UINT8                        *TableId,
  IN  UINT8                         TableIdSize,
  OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  OUT UINTN                        *Handle)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *OrgTable;

  // Verify Protocol Presense
  if (mAcpiSdtProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Verify Parameters
  if (TableId == NULL || TableIdSize > sizeof (OrgTable->OemTableId) || Table == NULL || Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Go thru all ACPI Tables
  for (UINTN i = 0;; i++) {
    EFI_ACPI_TABLE_VERSION TableVersion;

    // Get ACPI Table
    Status = mAcpiSdtProtocol->GetAcpiTable (i, (EFI_ACPI_SDT_HEADER **)&OrgTable, &TableVersion, Handle);
    if (Status == EFI_NOT_FOUND) {
      break;
    } else if (EFI_ERROR (Status)) {
      continue;
    }

    // Compare Table OEM IDs
    if (CompareMem (&OrgTable->OemTableId, TableId, TableIdSize) != 0) {
      continue;
    }

    // Copy Table
    *Table = AllocateCopyPool (OrgTable->Length, OrgTable);
    if (*Table == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
GetAslNameDataSize (
  IN  UINT8 *Current,
  OUT UINTN *DataSize)
{
  // Get Data OP Code
  UINT8 DataOp = *(Current + 5);

  // Return Original Data Size
  switch (DataOp) {
    case AML_STRING_PREFIX:
      *DataSize = AsciiStrSize ((CHAR8 *)(Current + 6));
      break;

    case AML_BYTE_PREFIX:
      *DataSize = sizeof (UINT8);
      break;

    case AML_WORD_PREFIX:
      *DataSize = sizeof (UINT16);
      break;

    case AML_DWORD_PREFIX:
      *DataSize = sizeof (UINT32);
      break;

    case AML_QWORD_PREFIX:
      *DataSize = sizeof (UINT64);
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AslUpdateName (
  IN EFI_ACPI_DESCRIPTION_HEADER *Table,
  IN UINT32                       NameSignature,
  IN VOID                        *Buffer,
  IN UINTN                        Length)
{
  EFI_STATUS Status;
  UINTN      OrigDataSize;

  // Verify Parameters
  if (Table == NULL || Buffer == NULL || Length == 0) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify Table Signature
  if (Table->Signature != EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE && Table->Signature != EFI_ACPI_3_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    return EFI_UNSUPPORTED;
  }

  // Set Table Memory Area
  UINT8 *TableStart = (UINT8 *)Table + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  UINT8 *TableEnd   = (UINT8 *)Table + Table->Length;

  // Go thru the Table
  for (UINT8 *Current = TableStart; Current < TableEnd - 5; Current++) {
    // Check for Name OP
    if (*Current != AML_NAME_OP) {
      continue;
    }

    // Compare Name Signatures
    if (*(UINT32 *)(Current + 1) != NameSignature) {
      continue;
    }

    // Get Original Name Data Size
    Status = GetAslNameDataSize (Current, &OrigDataSize);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Verify Original Name Data Size
    if (Length > OrigDataSize) {
      return EFI_BUFFER_TOO_SMALL;
    }

    // Overwrite Original Value
    CopyMem (Current + 6, Buffer, Length);

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
ReinstallTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *Table,
  IN OUT UINTN                       *TableHandle)
{
  EFI_STATUS Status;

  // Verify Protocol Presense
  if (mAcpiTableProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Verify Parameters
  if (Table == NULL || TableHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Update Checksum
  Table->Checksum = 0;
  Table->Checksum = (UINT8)(0 - CalculateSum8 ((UINT8 *)Table, Table->Length));

  // Remove ACPI Table
  Status = mAcpiTableProtocol->UninstallAcpiTable (mAcpiTableProtocol, *TableHandle);
  if (EFI_ERROR (Status)) {
    goto exit;
  }

  // Reset Table Handle
  *TableHandle = 0;

  // Reinstall ACPI Table
  Status = mAcpiTableProtocol->InstallAcpiTable (mAcpiTableProtocol, Table, Table->Length, TableHandle);
  ASSERT_EFI_ERROR (Status);

exit:
  // Free Buffer
  FreePool (Table);

  return Status;
}

VOID
EFIAPI
AcpiCallback (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  EFI_STATUS Status;
  EFI_HANDLE AcpiSdtHandle;

  // Set Buffer Size
  UINTN BufferSize = sizeof (EFI_HANDLE);

  // Locate ACPI SDT Protocol Handle
  Status = gBS->LocateHandle (ByRegisterNotify, NULL, mRegisteredAcpiSdt, &BufferSize, &AcpiSdtHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate ACPI SDT Protocol Handle! Status = %r\n", __FUNCTION__, Status));
    goto exit;
  }

  // Get ACPI SDT Protocol from Handle
  Status = gBS->HandleProtocol (AcpiSdtHandle, &gEfiAcpiSdtProtocolGuid, (VOID *)&mAcpiSdtProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get ACPI SDT Protocol from Handle! Status = %r\n", __FUNCTION__, Status));
    goto exit;
  }

  // Locate ACPI Table Protocol
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID *)&mAcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate ACPI Table Protocol!\n", __FUNCTION__));
  }

exit:
  // Close Event
  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
RegisterAcpiProtocolNotify ()
{
  EFI_STATUS Status;
  EFI_EVENT  CallbackEvent;

  // Create Callback Event
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, AcpiCallback, NULL, &CallbackEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create Callback Event! Status = %r\n", __FUNCTION__, Status));
    return EFI_SUCCESS;
  }

  // Register Protocol Notify
  Status = gBS->RegisterProtocolNotify (&gEfiAcpiSdtProtocolGuid, CallbackEvent, &mRegisteredAcpiSdt);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register Protocol Notify! Status = %r\n", __FUNCTION__, Status));

    // Close Event
    gBS->CloseEvent (CallbackEvent);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LocateAcpiProtocols (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate ACPI SDT Protocol
  Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, (VOID *)&mAcpiSdtProtocol);
  if (EFI_ERROR (Status)) {
    return RegisterAcpiProtocolNotify ();
  }

  // Locate ACPI Tables Protocol
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID *)&mAcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate ACPI Table Protocol!\n", __FUNCTION__));
  }

  return EFI_SUCCESS;
}
