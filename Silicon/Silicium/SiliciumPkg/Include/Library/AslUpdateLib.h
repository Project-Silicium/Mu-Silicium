/**
  Copyright (c) 2020 Intel Corporation. All rights reserved. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ASL_UPDATE_LIB_H_
#define _ASL_UPDATE_LIB_H_

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

EFI_STATUS
EFIAPI
UpdateNameAslCode (
  IN UINT32  AslSignature,
  IN VOID   *Buffer,
  IN UINTN   Length
  );

EFI_STATUS
EFIAPI
UpdateSsdtNameAslCode (
  IN UINT8  *TableId,
  IN UINT8   TableIdSize,
  IN UINT32  AslSignature,
  IN VOID   *Buffer,
  IN UINTN   Length
  );

EFI_STATUS
EFIAPI
UpdateMethodAslCode (
  IN UINT32  AslSignature,
  IN VOID   *Buffer,
  IN UINTN   Length
  );

EFI_STATUS
EFIAPI
LocateAcpiTableBySignature (
  IN     UINT32                        Signature,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER **Table,
  IN OUT UINTN                        *Handle
  );

#endif /* _ASL_UPDATE_LIB_H_ */
