/**
  ASL dynamic update library definitions.

  This library provides dynamic update to various ASL structures.
  There may be different libraries for different environments (PEI, BS, RT, SMM).
  Make sure you meet the requirements for the library (protocol dependencies, use
  restrictions, etc).

  Note that the current version of the library updates AML.

  Copyright (c) 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ASL_UPDATE_LIB_H_
#define _ASL_UPDATE_LIB_H_

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

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
  IN UINTN  Length
  );

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
  IN UINTN  Length
  );

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
  IN UINTN  Length
  );

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
  IN OUT UINTN                        *Handle
  );

#endif /* _ASL_UPDATE_LIB_H_ */
