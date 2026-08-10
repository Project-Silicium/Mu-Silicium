/**
  Copyright (c) 2020 Intel Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ASL_UPDATE_LIB_H_
#define _ASL_UPDATE_LIB_H_

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

EFI_STATUS
EFIAPI
LocateTableBySignature (
  IN  UINT32                        Signature,
  OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  OUT UINTN                        *Handle
  );

EFI_STATUS
EFIAPI
LocateTableByOemTableId (
  IN  UINT8                        *TableId,
  IN  UINT8                         TableIdSize,
  OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  OUT UINTN                        *Handle
  );

EFI_STATUS
EFIAPI
AslUpdateName (
  IN EFI_ACPI_DESCRIPTION_HEADER *Table,
  IN UINT32                       NameSignature,
  IN VOID                        *Buffer,
  IN UINTN                        Length
  );

EFI_STATUS
EFIAPI
ReinstallTable (
  IN     EFI_ACPI_DESCRIPTION_HEADER *Table,
  IN OUT UINTN                       *TableHandle
  );

#endif /* _ASL_UPDATE_LIB_H_ */
