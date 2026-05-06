/** @file
  Internal header for RSA EVP_PKEY context shared between CryptRsaBasic.c
  and CryptRsaExt.c.

  Defines the RSA_PKEY_CTX structure that replaces the deprecated OpenSSL RSA
  object, and declares helper functions for building and managing EVP_PKEY
  instances from stored BIGNUM key components.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CRYPT_RSA_PKEY_CTX_H_
#define CRYPT_RSA_PKEY_CTX_H_
#pragma once

#include <openssl/evp.h>
#include <openssl/bn.h>

///
/// Internal RSA key context that holds individual BIGNUM key components
/// and a cached EVP_PKEY built from those components.
///
typedef struct {
  EVP_PKEY    *Pkey;
  BIGNUM      *N;     ///< Public modulus
  BIGNUM      *E;     ///< Public exponent
  BIGNUM      *D;     ///< Private exponent
  BIGNUM      *P;     ///< Secret prime factor p
  BIGNUM      *Q;     ///< Secret prime factor q
  BIGNUM      *Dp;    ///< p's CRT exponent (d mod (p-1))
  BIGNUM      *Dq;    ///< q's CRT exponent (d mod (q-1))
  BIGNUM      *QInv;  ///< CRT coefficient (1/q mod p)
} RSA_PKEY_CTX;

/**
  Build (or return cached) EVP_PKEY from the stored BIGNUM components.

  If the EVP_PKEY is already cached and valid, return it directly.
  Otherwise, construct a new EVP_PKEY using OSSL_PARAM_BLD and
  EVP_PKEY_fromdata.

  @param[in,out]  RsaPkeyCtx  Pointer to RSA_PKEY_CTX holding key components.

  @return  Pointer to EVP_PKEY on success, or NULL on failure.
**/
EVP_PKEY *
RsaBuildEvpPkey (
  IN OUT  RSA_PKEY_CTX  *RsaPkeyCtx
  );

/**
  Extract all RSA BIGNUM key components from an EVP_PKEY into RSA_PKEY_CTX.

  Any previously stored BIGNUMs in the context are freed before extraction.

  @param[in,out]  RsaPkeyCtx  Pointer to RSA_PKEY_CTX to populate.
  @param[in]      Pkey        EVP_PKEY from which to extract components.

  @retval  TRUE   Components extracted successfully.
  @retval  FALSE  Extraction failed.
**/
BOOLEAN
RsaExtractBigNums (
  IN OUT  RSA_PKEY_CTX  *RsaPkeyCtx,
  IN      EVP_PKEY      *Pkey
  );

/**
  Invalidate (free) the cached EVP_PKEY in the RSA context.

  Called when key components change so the EVP_PKEY will be rebuilt
  on next use.

  @param[in,out]  RsaPkeyCtx  Pointer to RSA_PKEY_CTX whose cache to invalidate.
**/
VOID
RsaInvalidatePkey (
  IN OUT  RSA_PKEY_CTX  *RsaPkeyCtx
  );

#endif // CRYPT_RSA_PKEY_CTX_H_
