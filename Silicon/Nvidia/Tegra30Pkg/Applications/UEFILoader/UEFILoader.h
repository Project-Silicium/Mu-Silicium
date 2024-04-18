// Copyright (c) 2019 - 2020, Bingxing Wang and other project authors. All rights reserved.<BR>
// Copyright (c) 2021 - 2021, Leander Wollersberger. All rights reserved.<BR>

#include <Uefi.h>

#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

EFI_STATUS
UnprotectTZ ();

EFI_STATUS
FixConsoleOutput ();

UINT32
ArmCallSmcHelper (
  UINT32 R0,
  UINT32 R1,
  UINT32 R2,
  UINT32 R3
  );

VOID
memcpy_usr (
  VOID       *dest,
  CONST VOID *src,
  UINTN       n
  );

VOID*
memmem (
  CONST VOID *h0,
  UINTN       k,
  CONST VOID *n0,
  UINTN       
  );
