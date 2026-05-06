/** @file
  Internal header defining EC_PKEY_CTX shared between CryptEc.c and CryptX509.c.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef CRYPT_EC_PKEY_CTX_H_
#define CRYPT_EC_PKEY_CTX_H_
#pragma once

#include <openssl/evp.h>

///
/// Internal EC key context wrapping an EVP_PKEY with the associated curve NID.
///
typedef struct {
  INT32       Nid;   ///< OpenSSL NID (NID_X9_62_prime256v1, NID_secp384r1, etc.)
  EVP_PKEY    *Pkey; ///< NULL until EcGenerateKey() or EcGetPublicKeyFromX509()
} EC_PKEY_CTX;

#endif // CRYPT_EC_PKEY_CTX_H_
