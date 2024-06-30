/**
  Boot Service DXE ASL Update Library Implementation.
  Note that the Current Version of the Library Updates AML.

  These Functions in this File can be Called during DXE and cannot be Called during Runtime
  or in SMM which should use a RT or SMM Library.

  This Library uses the ACPI Support Protocol.

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AslUpdateLib.h>

//
// Function implementations
//
STATIC EFI_ACPI_SDT_PROTOCOL   *mAcpiSdt   = NULL;
STATIC EFI_ACPI_TABLE_PROTOCOL *mAcpiTable = NULL;

/**
  Initialize the ASL Update Library State.
  This must be Called at the Beginning of the Function Calls in this Library.

  @retval Status                      - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
InitializeAslUpdateLib ()
{
  EFI_STATUS Status;

  // Locate ACPI SDT Protocol
  Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, (VOID **)&mAcpiSdt);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate ACPI ADT Protocol!\n"));
    goto exit;
  }

  // Locate ACPI Tables Protocol
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&mAcpiTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate ACPI Table Protocol!\n"));
    goto exit;
  }

exit:
  return Status;
}

/**
  This Function Calculates and Updates an UINT8 Checksum.

  @param[in] Buffer                   - Pointer to Buffer to Checksum.
  @param[in] Size                     - Number of Bytes to Checksum.
  @param[in] ChecksumOffset           - Offset to Place the Checksum Result in.
**/
VOID
AcpiPlatformChecksum (
  IN VOID *Buffer,
  IN UINTN Size,
  IN UINTN ChecksumOffset)
{
  UINT8  Sum  = 0;
  UINT8 *Ptr  = Buffer;

  // Set Checksum to 0 first
  Ptr[ChecksumOffset] = 0;

  // Add all Content of Buffer
  while ((Size--) != 0) {
    Sum = (UINT8)(Sum + (*Ptr++));
  }

  // Set Checksum
  Ptr                 = Buffer;
  Ptr[ChecksumOffset] = (UINT8)(0xff - Sum + 1);
}

/**
  This Function uses the ACPI SDT Protocol to Locate an ACPI SSDT Table.

  @param[in] TableId                  - Pointer to an ASCII String Containing the OEM Table ID from the ACPI Table Header.
  @param[in] TableIdSize              - Length of the Table ID to Match.
  @param[in, out] Table               - Updated with a Pointer to the Table.
  @param[in, out] Handle              - ACPI Support Protocol Table Handle for the Table found.

  @retval Status                      - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
LocateAcpiTableByOemTableId (
  IN     UINT8                        *TableId,
  IN     UINT8                         TableIdSize,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  IN OUT UINTN                        *Handle)
{
  EFI_STATUS                   Status   = EFI_SUCCESS;
  INTN                         Index    = 0;
  EFI_ACPI_TABLE_VERSION       Version  = 0;
  EFI_ACPI_DESCRIPTION_HEADER *OrgTable = NULL;

  if (mAcpiSdt == NULL) {
    // Init ACPI Protocols
    Status = InitializeAslUpdateLib ();

    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

  do {
    // Locate ACPI Table with matching ID
    Status = mAcpiSdt->GetAcpiTable (Index, (EFI_ACPI_SDT_HEADER **)&OrgTable, &Version, Handle);
    if (Status == EFI_NOT_FOUND) {
      break;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate ACPI Table! Status = %r\n", Status));
      goto exit;
    }

    Index++;
  } while (CompareMem (&(OrgTable->OemTableId), TableId, TableIdSize));

  if (Status != EFI_NOT_FOUND) {
    *Table = OrgTable;
    ASSERT (*Table);
  }

exit:
  return Status;
}

/**
  This Procedure will Update Immediate Value assigned to a Name.

  @param[in] AslSignature             - The Signature of Operation Region that we want to Update.
  @param[in] Buffer                   - Source of Data to be Written over Original AML.
  @param[in] Length                   - Length of Data to be Overwritten.

  @retval Status                      - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
UpdateNameAslCode (
  IN UINT32 AslSignature,
  IN VOID  *Buffer,
  IN UINTN  Length)
{
  EFI_STATUS                   Status      = EFI_SUCCESS;
  EFI_ACPI_DESCRIPTION_HEADER *Table       = NULL;
  UINT8                       *CurrPtr     = 0;
  UINT8                       *EndPtr      = 0;
  UINT32                      *Signature   = 0;
  UINT8                       *DsdtPointer = 0;
  UINTN                        Handle      = 0;

  if (mAcpiTable == NULL) {
    // Init ACPI Protocols
    Status = InitializeAslUpdateLib ();

    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

  // Locate ACPI Table with matching Signature
  Status = LocateAcpiTableBySignature (EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE, (EFI_ACPI_DESCRIPTION_HEADER **)&Table, &Handle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate ACPI Table with its Signature! Status = %r\n", Status));
    goto exit;
  }

  // Point to the Beginning of the DSDT Table
  CurrPtr = (UINT8 *)Table;
  if (CurrPtr == NULL) {
    Status = EFI_NOT_FOUND;
    goto cleanup;
  }

  // EndPtr = beginning of table + length of table
  EndPtr = CurrPtr + ((EFI_ACPI_COMMON_HEADER *)CurrPtr)->Length;

  // Loop through the ASL Looking for Values that we must Fix Up
  for (DsdtPointer = CurrPtr; DsdtPointer < EndPtr; DsdtPointer++) {
    // Get a Pointer to Compare for Signature
    Signature = (UINT32 *)DsdtPointer;

    // Check if this is the Device Object Signature we are Looking for
    if ((*Signature) == AslSignature) {
      // Look for Name Encoding
      if (*(DsdtPointer-1) == AML_NAME_OP) {
        CopyMem (DsdtPointer+5, Buffer, Length);

        // Remove ACPI Table
        Status = mAcpiTable->UninstallAcpiTable (mAcpiTable, Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Remove ACPI Table! Status = %r\n", Status));
          goto cleanup;
        }

        Handle = 0;

        // Install ACPI Table
        Status = mAcpiTable->InstallAcpiTable (mAcpiTable, Table, Table->Length, &Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Install ACPI Table!\n"));
          ASSERT_EFI_ERROR (Status);
        }

        goto cleanup;
      }
    }
  }

  Status = EFI_NOT_FOUND;

cleanup:
  FreePool (Table);

exit:
  return Status;
}

/**
  This Procedure will Update Immediate Value assigned to a Name in SSDT Table.

  @param[in] TableId                  - Pointer to an ASCII String containing the OEM Table ID from the ACPI Table Header.
  @param[in] TableIdSize              - Length of the Table ID to match.
  @param[in] AslSignature             - The Signature of Operation Region that we want to Update.
  @param[in] Buffer                   - Source of Data to be Written over Original AML.
  @param[in] Length                   - Length of Data to be Overwritten.

  @retval Status                      - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
UpdateSsdtNameAslCode (
  IN UINT8 *TableId,
  IN UINT8  TableIdSize,
  IN UINT32 AslSignature,
  IN VOID  *Buffer,
  IN UINTN  Length)
{
  EFI_STATUS                   Status      = EFI_SUCCESS;
  EFI_ACPI_DESCRIPTION_HEADER *Table       = NULL;
  UINT8                       *CurrPtr     = 0;
  UINT32                      *Signature   = 0;
  UINT8                       *SsdtPointer = 0;
  UINTN                        Handle      = 0;

  if (mAcpiTable == NULL) {
    // Init ACPI Protocols
    Status = InitializeAslUpdateLib ();

    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

  // Locate ACPI Table with matching OEM Table ID
  Status = LocateAcpiTableByOemTableId (TableId, TableIdSize, (EFI_ACPI_DESCRIPTION_HEADER **)&Table, &Handle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate ACPI Table with its OEM Table ID! Status = %r\n", Status));
    goto exit;
  }

  // Point to the Beginning of the DSDT Table
  CurrPtr = (UINT8 *)Table;
  if (CurrPtr == NULL) {
    Status = EFI_NOT_FOUND;
    goto exit;
  }

  // Loop through the ASL Looking for Values that we must Fix Up
  for (SsdtPointer = CurrPtr; SsdtPointer <= (CurrPtr + ((EFI_ACPI_COMMON_HEADER *)CurrPtr)->Length); SsdtPointer++) {
    // Get a Pointer to Compare for Signature
    Signature = (UINT32 *)SsdtPointer;

    // Check if this is the Device Object Signature we are Looking for
    if ((*Signature) == AslSignature) {
      // Look for Name Encoding
      if (*(SsdtPointer-1) == AML_NAME_OP) {
        CopyMem (SsdtPointer+5, Buffer, Length);

        AcpiPlatformChecksum (Table, Table->Length, OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum));

        goto exit;
      }
    }
  }

  Status = EFI_NOT_FOUND;

exit:
  return Status;
}

/**
  This Procedure will Update the Name of ASL Method.

  @param[in] AslSignature             - The Signature of Operation Region that we want to Update.
  @param[in] Buffer                   - Source of Data to be Written over Original AML.
  @param[in] Length                   - Length of Data to be Overwritten.

  @retval Status                      - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
UpdateMethodAslCode (
  IN UINT32 AslSignature,
  IN VOID  *Buffer,
  IN UINTN  Length)
{
  EFI_STATUS                   Status      = EFI_SUCCESS;
  EFI_ACPI_DESCRIPTION_HEADER *Table       = NULL;
  UINT8                       *CurrPtr     = 0;
  UINT32                      *Signature   = 0;
  UINT8                       *DsdtPointer = 0;
  UINTN                        Handle      = 0;

  if (mAcpiTable == NULL) {
    // Init ACPI Protocols
    Status = InitializeAslUpdateLib ();

    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

  // Locate ACPI Table with matching Signature
  Status = LocateAcpiTableBySignature (EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE, (EFI_ACPI_DESCRIPTION_HEADER **)&Table, &Handle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate ACPI Table with its Signature! Status = %r\n", Status));
    goto exit;
  }

  // Point to the Beginning of the DSDT Table
  CurrPtr = (UINT8 *)Table;
  if (CurrPtr == NULL) {
    Status = EFI_NOT_FOUND;
    goto cleanup;
  }

  // Loop through the ASL Looking for Values that we must Fix Up.
  for (DsdtPointer = CurrPtr; DsdtPointer <= (CurrPtr + ((EFI_ACPI_COMMON_HEADER *)CurrPtr)->Length); DsdtPointer++) {
    // Get a Pointer to Compare for Signature
    Signature = (UINT32 *)DsdtPointer;

    // Check if this is the Device Object Signature we are Looking for
    if ((*Signature) == AslSignature) {
      // Look for Name Encoding
      if ((*(DsdtPointer-3) == AML_METHOD_OP) || (*(DsdtPointer-2) == AML_METHOD_OP)) {
        CopyMem (DsdtPointer, Buffer, Length);

        // Remove ACPI Table
        Status = mAcpiTable->UninstallAcpiTable (mAcpiTable, Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Remove ACPI Table! Status = %r\n", Status));
          goto cleanup;
        }

        Handle = 0;

        // Install ACPI Table
        Status = mAcpiTable->InstallAcpiTable (mAcpiTable, Table, Table->Length, &Handle);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Install ACPI Table!\n"));
          ASSERT_EFI_ERROR (Status);
        }

        goto cleanup;
      }
    }
  }

  Status = EFI_NOT_FOUND;

cleanup:
  FreePool (Table);

exit:
  return Status;
}

/**
  This Function uses the ACPI Support Protocol to Locate an ACPI Table.
  It is really only useful for Finding Tables that only have a Single Instance,
  e.g. FADT, FACS, MADT, etc.  It is not Good for Locating SSDT, etc.
  Matches are Determined by Finding the Table with ACPI Table that has
  a Matching Signature.

  @param[in] Signature                - Pointer to an ASCII String Containing the Signature to Match.
  @param[in, out] Table               - Updated with a Pointer to the Table.
  @param[in, out] Handle              - AcpiSupport Protocol Table Handle for the Table Found.
                                      - @see AcpiSupport Protocol for Details.

  @retval Status                      - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
LocateAcpiTableBySignature (
  IN     UINT32                        Signature,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  IN OUT UINTN                        *Handle)
{
  EFI_STATUS                   Status   = EFI_SUCCESS;
  INTN                         Index    = 0;
  EFI_ACPI_TABLE_VERSION       Version  = 0;
  EFI_ACPI_DESCRIPTION_HEADER *OrgTable = NULL;

  if (mAcpiTable == NULL) {
    // Init ACPI Protocols
    Status = InitializeAslUpdateLib ();

    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

  // Locate Table with Matching ID
  do {
    Status = mAcpiSdt->GetAcpiTable (Index, (EFI_ACPI_SDT_HEADER **)&OrgTable, &Version, Handle);
    if (Status == EFI_NOT_FOUND) {
      break;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get ACPI Table! Status = %r\n", Status));
      goto exit;
    }


    Index++;
  } while (OrgTable->Signature != Signature);

  if (Status != EFI_NOT_FOUND) {
    *Table = AllocateCopyPool (OrgTable->Length, OrgTable);
    ASSERT (*Table);
  }

exit:
  return Status;
}
