/** @file
  Cryptographic Library Information Implementation.

  This module provides version information for the underlying OpenSSL library.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <Library/BaseLib.h>
#include "InternalCryptLib.h"

/**
  Gets the cryptographic provider version information.

  This function returns the version string of the cryptographic provider.
  For this OpenSSL implementation, it returns the OpenSSL version string.

  @param[out]     Buffer       Pointer to the buffer to receive the version string.
                               If NULL, the required buffer size is returned in BufferSize.
  @param[in,out]  BufferSize   On input, the size of the buffer in bytes.
                               On output, the size of the data copied to the buffer (including null terminator).
                               If Buffer is NULL, returns the required buffer size.

  @retval  EFI_SUCCESS            The version string was successfully copied to the buffer or
                                  the BufferSize was updated when a NULL buffer was provided.
  @retval  EFI_BUFFER_TOO_SMALL   The buffer is too small. BufferSize contains the required size.
  @retval  EFI_INVALID_PARAMETER  BufferSize is NULL.
**/
EFI_STATUS
EFIAPI
GetCryptoProviderVersionString (
  OUT    CHAR8  *Buffer,
  IN OUT UINTN  *BufferSize
  )
{
  CONST CHAR8  *VersionText;
  UINTN        RequiredSize;

  if (BufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  VersionText  = OPENSSL_VERSION_TEXT;
  RequiredSize = AsciiStrSize (VersionText);

  if (Buffer == NULL) {
    *BufferSize = RequiredSize;
    return EFI_SUCCESS;
  }

  if (*BufferSize < RequiredSize) {
    *BufferSize = RequiredSize;
    return EFI_BUFFER_TOO_SMALL;
  }

  AsciiStrCpyS (Buffer, *BufferSize, VersionText);
  *BufferSize = RequiredSize;

  return EFI_SUCCESS;
}
