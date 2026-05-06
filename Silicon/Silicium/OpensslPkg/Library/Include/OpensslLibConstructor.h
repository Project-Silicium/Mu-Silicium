/** @file

Library to initialize manually OpensslLib

Copyright (c) Microsoft Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef OPENSSL_LIB_CONSTRUCTOR_H__
#define OPENSSL_LIB_CONSTRUCTOR_H__

#include <Uefi.h>

/**
  Constructor routine for OpensslLib.

  This function initializes the OpenSSL library by fetching a local copy of the
  hardware capability flags, which are used to enable native crypto instructions
  for optimized cryptographic operations.

  Note: Most callers who use the OpenSSL libraries do not need to call this
  function directly. This constructor is typically invoked automatically by the
  UEFI build system during library initialization. Only call this explicitly if
  you are managing library initialization manually.

  @param  None

  @retval EFI_SUCCESS         The construction succeeded.
  @retval Others              An error occurred during initialization.

**/
RETURN_STATUS
EFIAPI
OpensslLibConstructor (
  VOID
  );

#endif // OPENSSL_LIB_CONSTRUCTOR_H__
