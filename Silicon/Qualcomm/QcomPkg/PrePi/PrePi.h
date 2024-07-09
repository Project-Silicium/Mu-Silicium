/**
  Copyright (c) 2011 - 2020, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PREPI_H_
#define _PREPI_H_

STATIC UINT64 mSystemMemoryEnd = FixedPcdGet64 (PcdSystemMemoryBase) + FixedPcdGet64 (PcdSystemMemorySize) - 1;

#define IS_XIP() (((UINT64)FixedPcdGet64 (PcdFdBaseAddress) > mSystemMemoryEnd) || ((FixedPcdGet64 (PcdFdBaseAddress) + FixedPcdGet32 (PcdFdSize)) <= FixedPcdGet64 (PcdSystemMemoryBase)))

RETURN_STATUS
EFIAPI
TimerConstructor ();

EFI_STATUS
EFIAPI
MemoryPeim (
  IN EFI_PHYSICAL_ADDRESS UefiMemoryBase,
  IN UINT64               UefiMemorySize
  );

EFI_STATUS
EFIAPI
PlatformPeim ();

VOID
ArchInitialize ();

VOID
EFIAPI
ProcessLibraryConstructorList ();

#endif /* _PREPI_H_ */
