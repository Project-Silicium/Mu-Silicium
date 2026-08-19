/**
  Copyright (c) 2020 Intel Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ASL_UPDATE_LIB_H_
#define _ASL_UPDATE_LIB_H_

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

/**
  This Function Returns the ACPI Table Based on the Signature.

  @param[in]  Signature                    - The ACPI Table Signature.
  @param[out] Table                        - The ACPI Table.
  @param[out] Handle                       - The ACPI Table Handle.

  @return EFI_SUCCESS                      - The ACPI Table was Found Successfully.
  @return EFI_NOT_READY                    - The ACPI Protocols aren't Installed yet.
  @return EFI_INVALID_PARAMETER            - The "Table" and/or "Handle" Parameter are NULL.
  @return EFI_OUT_OF_RESOURCES             - The Memory Allocation Failed.
  @return EFI_NOT_FOUND                    - The ACPI Table with the Specified Signature does not Exist.
**/
EFI_STATUS
EFIAPI
LocateTableBySignature (
  IN  UINT32                        Signature,
  OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  OUT UINTN                        *Handle
  );

/**
  This Function Returns the ACPI Table Based on the Table ID.

  @param[in]  TableId                      - The ACPI Table ID.
  @param[in]  TableIdSize                  - The ACPI Table ID Length.
  @param[out] Table                        - The ACPI Table.
  @param[out] Handle                       - The ACPI Table Handle.

  @return EFI_SUCCESS                      - The ACPI Table was Found Successfully.
  @return EFI_NOT_READY                    - The ACPI Protocols aren't Installed yet.
  @return EFI_INVALID_PARAMETER            - The "TableId", "Table" and/or "Handle" Parameter are NULL.
  @return EFI_INVALID_PARAMETER            - The "TableIdSize" Value is Larger than Possible.
  @return EFI_OUT_OF_RESOURCES             - The Memory Allocation Failed.
  @return EFI_NOT_FOUND                    - The ACPI Table with the Specified Table ID does not Exist.
**/
EFI_STATUS
EFIAPI
LocateTableByOemTableId (
  IN  UINT8                        *TableId,
  IN  UINT8                         TableIdSize,
  OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  OUT UINTN                        *Handle
  );

/**
  This Function Updates the Value of the Specified ASL Name.

  @param[in] Table                         - The Target ACPI Table.
  @param[in] NameSignature                 - The Signature of the ASL Name.
  @param[in] Buffer                        - The new Value.
  @param[in] Length                        - The Length of the new Value.

  @return EFI_SUCCESS                      - The ASL Name Value was Updated Successfully.
  @return EFI_INVALID_PARAMETER            - The "Table", "Buffer" and/or "Length" Parameter are NULL or 0.
  @return EFI_UNSUPPORTED                  - The Target ACPI Table isn't a DSDT or SSDT Table.
  @return EFI_UNSUPPORTED                  - The Specified ASL Name isn't Supported.
  @return EFI_BUFFER_TOO_SMALL             - The Specified ASL Name is too Small for the new Value.
  @return EFI_NOT_FOUND                    - The Specified ASL Name wasn't Found.
**/
EFI_STATUS
EFIAPI
AslUpdateName (
  IN EFI_ACPI_DESCRIPTION_HEADER *Table,
  IN UINT32                       NameSignature,
  IN VOID                        *Buffer,
  IN UINTN                        Length
  );

/**
  This Function Reinstalls the Specified ACPI Table.

  @param[in]      Table                    - The ACPI Table.
  @param[in, out] TableHandle              - The ACPI Table Handle.

  @return EFI_SUCCESS                      - The ACPI Table was Reinstalled Successfully.
  @return EFI_NOT_READY                    - The ACPI Protocols aren't Installed yet.
  @return EFI_INVALID_PARAMETER            - The "Table" and/or "TableHandle" Parameter are NULL.
**/
EFI_STATUS
EFIAPI
ReinstallTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *Table,
  IN OUT UINTN                       *TableHandle
  );

#endif /* _ASL_UPDATE_LIB_H_ */
