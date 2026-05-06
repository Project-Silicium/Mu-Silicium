/**
  Copyright (c) 2011 - 2020, Arm Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SEC_H_
#define _SEC_H_

VOID
SecVectorTable ();

VOID
SecCommonExceptionEntry (
  IN UINT32 Type,
  IN UINTN  LR
  );

EFI_STATUS
EFIAPI
MemoryPeim (
  IN EFI_PHYSICAL_ADDRESS  UefiMemoryBase,
  IN UINT64                UefiMemorySize
  );

EFI_STATUS
EFIAPI
PlatformPeim ();

VOID
EFIAPI
ProcessLibraryConstructorList ();

#endif /* _SEC_H_ */
