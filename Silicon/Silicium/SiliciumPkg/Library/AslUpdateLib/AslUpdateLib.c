/**
  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AslUpdateLib.h>

//
// Function implementations
//
STATIC EFI_ACPI_SDT_PROTOCOL   *mAcpiSdtProtocol;
STATIC EFI_ACPI_TABLE_PROTOCOL *mAcpiTableProtocol;

EFI_STATUS
LocateAcpiProtocols ()
{
  EFI_STATUS Status;

  // Locate ACPI SDT Protocol
  if (mAcpiSdtProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, (VOID *)&mAcpiSdtProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate ACPI ADT Protocol! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  // Locate ACPI Tables Protocol
  if (mAcpiTableProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID *)&mAcpiTableProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate ACPI Table Protocol! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LocateAcpiTableBySignature (
  IN     UINT32                        Signature,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  IN OUT UINTN                        *Handle)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *AcpiTable;

  for (INT8 i = 0; i < 255; i++) {
    EFI_ACPI_TABLE_VERSION Version;

    // Get ACPI Table
    Status = mAcpiSdtProtocol->GetAcpiTable (i, (EFI_ACPI_SDT_HEADER **)&AcpiTable, &Version, Handle);
    if (Status == EFI_NOT_FOUND) {
      return Status;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to get ACPI Table by Index %d! Status = %r\n", __FUNCTION__, i, Status));
      return Status;
    }

    // Compare Signature
    if (AcpiTable->Signature == Signature) {
      // Pass ACPI Table
      *Table = AllocateCopyPool (AcpiTable->Length, AcpiTable);
      if (*Table == NULL) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Pass ACPI Table %d Data!\n", __FUNCTION__, i));
        return EFI_OUT_OF_RESOURCES;
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
UpdateNameAslCode (
  IN UINT32  AslSignature,
  IN VOID   *Buffer,
  IN UINTN   Length)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  UINT32                      *Signature;
  UINT8                       *CurrPtr;
  UINT8                       *EndPtr;
  UINTN                        Handle;

  // Locate ACPI Protocols
  Status = LocateAcpiProtocols ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Locate ACPI Table with Signature
  Status = LocateAcpiTableBySignature (EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE, (EFI_ACPI_DESCRIPTION_HEADER **)&Table, &Handle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Current Pointer
  CurrPtr = (UINT8 *)Table;
  if (CurrPtr == NULL) {
    // Free Buffer
    FreePool (Table);

    return EFI_NOT_FOUND;
  }

  // Set End Pointer
  EndPtr = CurrPtr + ((EFI_ACPI_COMMON_HEADER *)CurrPtr)->Length;

  for (UINT8 *DsdtPointer = CurrPtr; DsdtPointer < EndPtr; DsdtPointer++) {
    // Get ACPI Table Signature
    Signature = (UINT32 *)DsdtPointer;

    // Compare Signature
    if ((*Signature) == AslSignature) {
      // Compare Name Encoding
      if (*(DsdtPointer-1) == AML_NAME_OP) {
        CopyMem (DsdtPointer + 5, Buffer, Length);

        // Remove ACPI Table
        Status = mAcpiTableProtocol->UninstallAcpiTable (mAcpiTableProtocol, Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Remove ACPI Table! Status = %r\n", __FUNCTION__, Status));

          // Free Buffer
          FreePool (Table);

          return Status;
        }

        // Reset Handle
        Handle = 0;

        // Install ACPI Table
        Status = mAcpiTableProtocol->InstallAcpiTable (mAcpiTableProtocol, Table, Table->Length, &Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Install ACPI Table!\n", __FUNCTION__));
          ASSERT_EFI_ERROR (Status);
        }

        // Free Buffer
        FreePool (Table);

        return EFI_SUCCESS;
      }
    }
  }

  // Free Buffer
  FreePool (Table);

  return EFI_NOT_FOUND;
}

EFI_STATUS
LocateAcpiTableByOemTableId (
  IN     UINT8                        *TableId,
  IN     UINT8                         TableIdSize,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  IN OUT UINTN                        *Handle)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *AcpiTable;

  for (INT8 i = 0; i < 255; i++) {
    EFI_ACPI_TABLE_VERSION Version;

    // Get ACPI Table
    Status = mAcpiSdtProtocol->GetAcpiTable (i, (EFI_ACPI_SDT_HEADER **)&AcpiTable, &Version, Handle);
    if (Status == EFI_NOT_FOUND) {
      return Status;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to get ACPI Table by Index %d! Status = %r\n", __FUNCTION__, i, Status));
      return Status;
    }

    // Compare OEM ID
    if (CompareMem (&(AcpiTable->OemTableId), TableId, TableIdSize)) {
      // Pass ACPI Table
      *Table = AcpiTable;
      if (*Table == NULL) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Pass ACPI Table %d Data!\n", __FUNCTION__, i));
        return EFI_NOT_READY;
      }
    }
  }

  return EFI_SUCCESS;
}

VOID
AcpiPlatformChecksum (
  IN VOID  *Buffer,
  IN UINTN  Size,
  IN UINTN  ChecksumOffset)
{
  UINT8 *Ptr  = Buffer;
  UINT8  Sum  = 0;

  // Set Dummy Checksum
  Ptr[ChecksumOffset] = 0;

  // Add all Content of Buffer
  while ((Size--) != 0) {
    Sum = (UINT8)(Sum + (*Ptr++));
  }

  // Set Checksum
  Ptr                 = Buffer;
  Ptr[ChecksumOffset] = (UINT8)(0xFF - Sum + 1);
}

EFI_STATUS
EFIAPI
UpdateSsdtNameAslCode (
  IN UINT8  *TableId,
  IN UINT8   TableIdSize,
  IN UINT32  AslSignature,
  IN VOID   *Buffer,
  IN UINTN   Length)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  UINT32                      *Signature;
  UINT8                       *CurrPtr;
  UINTN                        Handle;

  // Locate ACPI Protocols
  Status = LocateAcpiProtocols ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Locate ACPI Table with OEM ID
  Status = LocateAcpiTableByOemTableId (TableId, TableIdSize, (EFI_ACPI_DESCRIPTION_HEADER **)&Table, &Handle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Current Pointer
  CurrPtr = (UINT8 *)Table;
  if (CurrPtr == NULL) {
    return EFI_NOT_FOUND;
  }

  for (UINT8 *SsdtPointer = CurrPtr; SsdtPointer <= (CurrPtr + ((EFI_ACPI_COMMON_HEADER *)CurrPtr)->Length); SsdtPointer++) {
    // Get ACPI Table Signature
    Signature = (UINT32 *)SsdtPointer;

    // Comapre Signature
    if ((*Signature) == AslSignature) {
      // Compare Name Encoding
      if (*(SsdtPointer - 1) == AML_NAME_OP) {
        // Copy ACPI Table
        CopyMem (SsdtPointer + 5, Buffer, Length);

        // Checksum ACPI Table
        AcpiPlatformChecksum (Table, Table->Length, OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum));

        return EFI_SUCCESS;
      }
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
UpdateMethodAslCode (
  IN UINT32  AslSignature,
  IN VOID   *Buffer,
  IN UINTN   Length)
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  UINT32                      *Signature;
  UINT8                       *CurrPtr;
  UINTN                        Handle;

  // Locate ACPI Protocols
  Status = LocateAcpiProtocols ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Locate ACPI Table with Signature
  Status = LocateAcpiTableBySignature (EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE, (EFI_ACPI_DESCRIPTION_HEADER **)&Table, &Handle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Current Pointer
  CurrPtr = (UINT8 *)Table;
  if (CurrPtr == NULL) {
    // Free Buffer
    FreePool (Table);

    return EFI_NOT_FOUND;
  }

  for (UINT8 *DsdtPointer = CurrPtr; DsdtPointer <= (CurrPtr + ((EFI_ACPI_COMMON_HEADER *)CurrPtr)->Length); DsdtPointer++) {
    // Get ACPI Table Signature
    Signature = (UINT32 *)DsdtPointer;

    // Compare Signature
    if ((*Signature) == AslSignature) {
      // Compare Name Encoding
      if ((*(DsdtPointer - 3) == AML_METHOD_OP) || (*(DsdtPointer - 2) == AML_METHOD_OP)) {
        // Copy ACPI Table
        CopyMem (DsdtPointer, Buffer, Length);

        // Remove ACPI Table
        Status = mAcpiTableProtocol->UninstallAcpiTable (mAcpiTableProtocol, Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Remove ACPI Table! Status = %r\n", __FUNCTION__, Status));

          // Free Buffer
          FreePool (Table);

          return Status;
        }

        // Reset Handle
        Handle = 0;

        // Install ACPI Table
        Status = mAcpiTableProtocol->InstallAcpiTable (mAcpiTableProtocol, Table, Table->Length, &Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Install ACPI Table!\n", __FUNCTION__));
          ASSERT_EFI_ERROR (Status);
        }

        // Free Buffer
        FreePool (Table);

        return EFI_SUCCESS;
      }
    }
  }

  // Free Buffer
  FreePool (Table);

  return EFI_NOT_FOUND;
}
