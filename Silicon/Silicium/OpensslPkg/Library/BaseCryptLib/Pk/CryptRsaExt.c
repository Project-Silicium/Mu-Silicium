/** @file
  RSA Asymmetric Cipher Wrapper Implementation over OpenSSL.

  This file implements following APIs which provide more capabilities for RSA:
  1) RsaGetKey
  2) RsaGenerateKey
  3) RsaCheckKey
  4) RsaPkcs1Sign

Copyright (c) 2009 - 2020, Intel Corporation. All rights reserved.<BR>
Copyright (c) Microsoft Corporation.  // MU_CHANGE
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "InternalCryptLib.h"

#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
// MU_CHANGE [BEGIN]
#include <openssl/evp.h>
#include <openssl/param_build.h>
#include <openssl/core_names.h>

#include "CryptRsaPkeyCtx.h"

/**
  Retrieve a pointer to EVP message digest object.

  @param[in]  HashSize  Size of the message digest in bytes.

  @return  Pointer to EVP_MD, or NULL if unsupported size.

**/
STATIC
CONST
EVP_MD *
GetEvpMdFromHashSize (
  IN  UINTN  HashSize
  )
{
  switch (HashSize) {
    case MD5_DIGEST_SIZE:
      return EVP_md5 ();
    case SHA1_DIGEST_SIZE:
      return EVP_sha1 ();
    case SHA256_DIGEST_SIZE:
      return EVP_sha256 ();
    case SHA384_DIGEST_SIZE:
      return EVP_sha384 ();
    case SHA512_DIGEST_SIZE:
      return EVP_sha512 ();
    default:
      return NULL;
  }
}

// MU_CHANGE [END]

/**
  Gets the tag-designated RSA key component from the established RSA context.

  This function retrieves the tag-designated RSA key component from the
  established RSA context as a non-negative integer (octet string format
  represented in RSA PKCS#1).
  If specified key component has not been set or has been cleared, then returned
  BnSize is set to 0.
  If the BigNumber buffer is too small to hold the contents of the key, FALSE
  is returned and BnSize is set to the required buffer size to obtain the key.

  If RsaContext is NULL, then return FALSE.
  If BnSize is NULL, then return FALSE.
  // MU_CHANGE [BEGIN]
  If BnSize is large enough but BigNumber is NULL, then return TRUE with BnSize set to
  the required size.
  // MU_CHANGE [END]

  @param[in, out]  RsaContext  Pointer to RSA context being set.
  @param[in]       KeyTag      Tag of RSA key component being set.
  @param[out]      BigNumber   Pointer to octet integer buffer.
  @param[in, out]  BnSize      On input, the size of big number buffer in bytes.
                               On output, the size of data returned in big number buffer in bytes.

  @retval  TRUE   RSA key component was retrieved successfully.
  @retval  FALSE  Invalid RSA key component tag.
  @retval  FALSE  BnSize is too small.

**/
BOOLEAN
EFIAPI
RsaGetKey (
  IN OUT  VOID         *RsaContext,
  IN      RSA_KEY_TAG  KeyTag,
  OUT     UINT8        *BigNumber,
  IN OUT  UINTN        *BnSize
  )
{
  // MU_CHANGE [BEGIN]
  RSA_PKEY_CTX  *RsaPkeyCtx;
  BIGNUM        *BnKey;
  UINTN         Size;

  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((RsaContext == NULL) || (BnSize == NULL)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  RsaPkeyCtx = (RSA_PKEY_CTX *)RsaContext;
  Size       = *BnSize;
  *BnSize    = 0;
  BnKey      = NULL;
  // MU_CHANGE [END]

  switch (KeyTag) {
    //
    // RSA Public Modulus (N)
    //
    case RsaKeyN:
      BnKey = RsaPkeyCtx->N;  // MU_CHANGE
      break;

    //
    // RSA Public Exponent (e)
    //
    case RsaKeyE:
      BnKey = RsaPkeyCtx->E;  // MU_CHANGE
      break;

    //
    // RSA Private Exponent (d)
    //
    case RsaKeyD:
      BnKey = RsaPkeyCtx->D;  // MU_CHANGE
      break;

    //
    // RSA Secret Prime Factor of Modulus (p)
    //
    case RsaKeyP:
      BnKey = RsaPkeyCtx->P;  // MU_CHANGE
      break;

    //
    // RSA Secret Prime Factor of Modules (q)
    //
    case RsaKeyQ:
      BnKey = RsaPkeyCtx->Q;  // MU_CHANGE
      break;

    //
    // p's CRT Exponent (== d mod (p - 1))
    //
    case RsaKeyDp:
      BnKey = RsaPkeyCtx->Dp;  // MU_CHANGE
      break;

    //
    // q's CRT Exponent (== d mod (q - 1))
    //
    case RsaKeyDq:
      BnKey = RsaPkeyCtx->Dq;  // MU_CHANGE
      break;

    //
    // The CRT Coefficient (== 1/q mod p)
    //
    case RsaKeyQInv:
      BnKey = RsaPkeyCtx->QInv;  // MU_CHANGE
      break;

    default:
      return FALSE;
  }

  if (BnKey == NULL) {
    // MU_CHANGE [BEGIN]
    *BnSize = 0;
    return TRUE;
    // MU_CHANGE [END]
  }

  *BnSize = Size;
  Size    = BN_num_bytes (BnKey);

  if (*BnSize < Size) {
    *BnSize = Size;
    return FALSE;
  }

  if (BigNumber == NULL) {
    *BnSize = Size;
    return TRUE;
  }

  *BnSize = BN_bn2bin (BnKey, BigNumber);

  return TRUE;
}

/**
  Generates RSA key components.

  This function generates RSA key components. It takes RSA public exponent E and
  length in bits of RSA modulus N as input, and generates all key components.
  If PublicExponent is NULL, the default RSA public exponent (0x10001) will be used.

  Before this function can be invoked, pseudorandom number generator must be correctly
  initialized by RandomSeed().

  If RsaContext is NULL, then return FALSE.

  @param[in, out]  RsaContext           Pointer to RSA context being set.
  @param[in]       ModulusLength        Length of RSA modulus N in bits.
  @param[in]       PublicExponent       Pointer to RSA public exponent.
  @param[in]       PublicExponentSize   Size of RSA public exponent buffer in bytes.

  @retval  TRUE   RSA key component was generated successfully.
  @retval  FALSE  Invalid RSA key component tag.

**/
BOOLEAN
EFIAPI
RsaGenerateKey (
  IN OUT  VOID         *RsaContext,
  IN      UINTN        ModulusLength,
  IN      CONST UINT8  *PublicExponent,
  IN      UINTN        PublicExponentSize
  )
{
  // MU_CHANGE [BEGIN]
  RSA_PKEY_CTX  *RsaPkeyCtx;
  EVP_PKEY_CTX  *KeyGenCtx;
  EVP_PKEY      *Pkey;
  BIGNUM        *KeyE;
  BOOLEAN       RetVal;

  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((RsaContext == NULL) || (ModulusLength > INT_MAX) || (PublicExponentSize > INT_MAX)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  RsaPkeyCtx = (RSA_PKEY_CTX *)RsaContext;
  KeyGenCtx  = NULL;
  Pkey       = NULL;
  KeyE       = NULL;
  RetVal     = FALSE;

  //
  // Invalidate any cached key since we are generating a new one.
  //
  RsaInvalidatePkey (RsaPkeyCtx);

  KeyGenCtx = EVP_PKEY_CTX_new_from_name (NULL, "RSA", NULL);
  if (KeyGenCtx == NULL) {
    return FALSE;
  }

  if (EVP_PKEY_keygen_init (KeyGenCtx) != 1) {
    goto _Exit;
  }

  if (EVP_PKEY_CTX_set_rsa_keygen_bits (KeyGenCtx, (INT32)ModulusLength) != 1) {
    goto _Exit;
  }

  //
  // Set public exponent if provided, otherwise OpenSSL defaults to 0x10001.
  //
  if (PublicExponent != NULL) {
    KeyE = BN_new ();
    if (KeyE == NULL) {
      goto _Exit;
    }

    if (BN_bin2bn (PublicExponent, (UINT32)PublicExponentSize, KeyE) == NULL) {
      goto _Exit;
    }

    if (EVP_PKEY_CTX_set1_rsa_keygen_pubexp (KeyGenCtx, KeyE) != 1) {
      goto _Exit;
    }
  }

  if (EVP_PKEY_keygen (KeyGenCtx, &Pkey) != 1) {
    goto _Exit;
  }

  //
  // Extract all key components from the generated EVP_PKEY.
  //
  if (!RsaExtractBigNums (RsaPkeyCtx, Pkey)) {
    EVP_PKEY_free (Pkey);
    goto _Exit;
  }

  //
  // Cache the generated EVP_PKEY.
  //
  RsaPkeyCtx->Pkey = Pkey;
  RetVal           = TRUE;

_Exit:
  if (KeyE != NULL) {
    BN_free (KeyE);
  }

  if (KeyGenCtx != NULL) {
    EVP_PKEY_CTX_free (KeyGenCtx);
  }

  // MU_CHANGE [END]
  return RetVal;
}

/**
  Validates key components of RSA context.
  NOTE: This function performs integrity checks on all the RSA key material, so
        the RSA key structure must contain all the private key data.

  This function validates key components of RSA context in following aspects:
  - Whether p is a prime
  - Whether q is a prime
  - Whether n = p * q
  - Whether d*e = 1  mod lcm(p-1,q-1)

  If RsaContext is NULL, then return FALSE.

  @param[in]  RsaContext  Pointer to RSA context to check.

  @retval  TRUE   RSA key components are valid.
  @retval  FALSE  RSA key components are not valid.

**/
BOOLEAN
EFIAPI
RsaCheckKey (
  IN  VOID  *RsaContext
  )
{
  // MU_CHANGE [BEGIN]
  RSA_PKEY_CTX  *RsaPkeyCtx;
  EVP_PKEY      *Pkey;
  EVP_PKEY_CTX  *PkeyCtx;
  INT32         Result;

  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if (RsaContext == NULL) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  RsaPkeyCtx = (RSA_PKEY_CTX *)RsaContext;
  PkeyCtx    = NULL;

  //
  // Build EVP_PKEY from stored key components.
  //
  Pkey = RsaBuildEvpPkey (RsaPkeyCtx);
  if (Pkey == NULL) {
    return FALSE;
  }

  PkeyCtx = EVP_PKEY_CTX_new_from_pkey (NULL, Pkey, NULL);
  if (PkeyCtx == NULL) {
    return FALSE;
  }

  Result = EVP_PKEY_check (PkeyCtx);

  EVP_PKEY_CTX_free (PkeyCtx);

  if (Result != 1) {
    return FALSE;
    // MU_CHANGE [END]
  }

  return TRUE;
}

/**
  Carries out the RSA-SSA signature generation with EMSA-PKCS1-v1_5 encoding scheme.

  This function carries out the RSA-SSA signature generation with EMSA-PKCS1-v1_5 encoding scheme defined in
  RSA PKCS#1.
  If the Signature buffer is too small to hold the contents of signature, FALSE
  is returned and SigSize is set to the required buffer size to obtain the signature.

  If RsaContext is NULL, then return FALSE.
  If MessageHash is NULL, then return FALSE.
  If HashSize is not equal to the size of MD5, SHA-1, SHA-256, SHA-384 or SHA-512 digest, then return FALSE.
  If SigSize is large enough but Signature is NULL, then return FALSE.

  @param[in]       RsaContext   Pointer to RSA context for signature generation.
  @param[in]       MessageHash  Pointer to octet message hash to be signed.
  @param[in]       HashSize     Size of the message hash in bytes.
  @param[out]      Signature    Pointer to buffer to receive RSA PKCS1-v1_5 signature.
  @param[in, out]  SigSize      On input, the size of Signature buffer in bytes.
                                On output, the size of data returned in Signature buffer in bytes.

  @retval  TRUE   Signature successfully generated in PKCS1-v1_5.
  @retval  FALSE  Signature generation failed.
  @retval  FALSE  SigSize is too small.

**/
BOOLEAN
EFIAPI
RsaPkcs1Sign (
  IN      VOID         *RsaContext,
  IN      CONST UINT8  *MessageHash,
  IN      UINTN        HashSize,
  OUT     UINT8        *Signature,
  IN OUT  UINTN        *SigSize
  )
{
  // MU_CHANGE [BEGIN]
  RSA_PKEY_CTX  *RsaPkeyCtx;
  EVP_PKEY      *Pkey;
  EVP_PKEY_CTX  *PkeyCtx;
  CONST EVP_MD  *Md;
  UINTN         RequiredSize;
  BOOLEAN       Result;

  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((RsaContext == NULL) || (MessageHash == NULL)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  //
  // Determine the message digest algorithm according to digest size.
  //
  Md = GetEvpMdFromHashSize (HashSize);
  if (Md == NULL) {
    return FALSE;
  }

  RsaPkeyCtx = (RSA_PKEY_CTX *)RsaContext;
  PkeyCtx    = NULL;
  Result     = FALSE;

  //
  // Build EVP_PKEY from stored key components.
  //
  Pkey = RsaBuildEvpPkey (RsaPkeyCtx);
  if (Pkey == NULL) {
    return FALSE;
  }

  //
  // Check if the signature buffer is large enough.
  //
  RequiredSize = (UINTN)EVP_PKEY_get_size (Pkey);
  if (*SigSize < RequiredSize) {
    *SigSize = RequiredSize;
    return FALSE;
  }

  if (Signature == NULL) {
    return FALSE;
  }

  PkeyCtx = EVP_PKEY_CTX_new_from_pkey (NULL, Pkey, NULL);
  if (PkeyCtx == NULL) {
    goto _Exit;
  }

  if (EVP_PKEY_sign_init (PkeyCtx) != 1) {
    goto _Exit;
  }

  if (EVP_PKEY_CTX_set_rsa_padding (PkeyCtx, RSA_PKCS1_PADDING) <= 0) {
    goto _Exit;
  }

  if (EVP_PKEY_CTX_set_signature_md (PkeyCtx, Md) <= 0) {
    goto _Exit;
  }

  *SigSize = RequiredSize;
  if (EVP_PKEY_sign (PkeyCtx, Signature, SigSize, MessageHash, HashSize) == 1) {
    Result = TRUE;
  }

_Exit:
  if (PkeyCtx != NULL) {
    EVP_PKEY_CTX_free (PkeyCtx);
  }

  return Result;
  // MU_CHANGE [END]
}
