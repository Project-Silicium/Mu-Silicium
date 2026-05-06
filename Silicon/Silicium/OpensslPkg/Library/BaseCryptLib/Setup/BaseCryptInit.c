/** @file
  Base Cryptographic Setup Library Implementation.

  This library provides initialization functions for the cryptographic library.
  It serves as a wrapper around the OpenSSL library initialization.

Copyright (c), Microsoft Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <OpensslLibConstructor.h>
#include <Library/BaseCryptLib.h>

/**
  Initialize the cryptographic library.

  @retval EFI_SUCCESS  The library was initialized successfully.
**/
EFI_STATUS
EFIAPI
BaseCryptInit (
  VOID
  )
{
  return OpensslLibConstructor ();
}
