/** @file
  Diffie-Hellman Wrapper Implementation over OpenSSL.

Copyright (c) 2010 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "InternalCryptLib.h"
#include <openssl/bn.h>
// MU_CHANGE [BEGIN]
#include <openssl/evp.h>
#include <openssl/param_build.h>
#include <openssl/core_names.h>
// MU_CHANGE [END]
#include <openssl/dh.h>

// MU_CHANGE [BEGIN]
///
/// Internal context structure wrapping EVP_PKEY-based DH state.
///
typedef struct {
  BIGNUM      *BnP;  ///< Prime p (NULL until DhGenerateParameter or DhSetParameter)
  BIGNUM      *BnG;  ///< Generator g (NULL until DhGenerateParameter or DhSetParameter)
  EVP_PKEY    *Pkey; ///< NULL until DhGenerateKey()
} DH_PKEY_CTX;

// MU_CHANGE [END]

/**
  Allocates and Initializes one Diffie-Hellman Context for subsequent use.

  @return  Pointer to the Diffie-Hellman Context that has been initialized.
           If the allocations fails, DhNew() returns NULL.

**/
VOID *
EFIAPI
DhNew (
  VOID
  )
{
  return (VOID *)AllocateZeroPool (sizeof (DH_PKEY_CTX));  // MU_CHANGE
}

/**
  Release the specified DH context.

  If DhContext is NULL, then return FALSE.

  @param[in]  DhContext  Pointer to the DH context to be released.

**/
VOID
EFIAPI
DhFree (
  IN  VOID  *DhContext
  )
{
  // MU_CHANGE [BEGIN]
  DH_PKEY_CTX  *Ctx;

  if (DhContext == NULL) {
    return;
  }

  Ctx = (DH_PKEY_CTX *)DhContext;

  if (Ctx->BnP != NULL) {
    BN_free (Ctx->BnP);
  }

  if (Ctx->BnG != NULL) {
    BN_free (Ctx->BnG);
  }

  if (Ctx->Pkey != NULL) {
    EVP_PKEY_free (Ctx->Pkey);
  }

  FreePool (Ctx);
  // MU_CHANGE [END]
}

/**
  Generates DH parameter.

  Given generator g, and length of prime number p in bits, this function generates p,
  and sets DH context according to value of g and p.

  Before this function can be invoked, pseudorandom number generator must be correctly
  initialized by RandomSeed().

  If DhContext is NULL, then return FALSE.
  If Prime is NULL, then return FALSE.

  @param[in, out]  DhContext    Pointer to the DH context.
  @param[in]       Generator    Value of generator.
  @param[in]       PrimeLength  Length in bits of prime to be generated.
  @param[out]      Prime        Pointer to the buffer to receive the generated prime number.

  @retval TRUE   DH parameter generation succeeded.
  @retval FALSE  Value of Generator is not supported.
  @retval FALSE  PRNG fails to generate random prime number with PrimeLength.

**/
BOOLEAN
EFIAPI
DhGenerateParameter (
  IN OUT  VOID   *DhContext,
  IN      UINTN  Generator,
  IN      UINTN  PrimeLength,
  OUT     UINT8  *Prime
  )
{
  // MU_CHANGE [BEGIN]
  BOOLEAN       RetVal;
  DH_PKEY_CTX   *Ctx;
  EVP_PKEY_CTX  *PgenCtx;
  EVP_PKEY      *ParamsPkey;
  BIGNUM        *BnP;
  BIGNUM        *BnG;

  RetVal     = FALSE;
  PgenCtx    = NULL;
  ParamsPkey = NULL;
  BnP        = NULL;
  BnG        = NULL;
  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((DhContext == NULL) || (Prime == NULL) || (PrimeLength > INT_MAX)) {
    return FALSE;
  }

  if ((Generator != DH_GENERATOR_2) && (Generator != DH_GENERATOR_5)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  Ctx = (DH_PKEY_CTX *)DhContext;

  //
  // Generate DH parameters using EVP_PKEY paramgen.
  //
  PgenCtx = EVP_PKEY_CTX_new_from_name (NULL, "DH", NULL);
  if (PgenCtx == NULL) {
    goto Fail;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  if (EVP_PKEY_paramgen_init (PgenCtx) <= 0) {
    goto Fail;
  }

  // MU_CHANGE [END]

  // MU_CHANGE [BEGIN]
  if (EVP_PKEY_CTX_set_dh_paramgen_prime_len (PgenCtx, (INT32)PrimeLength) <= 0) {
    goto Fail;
  }

  if (EVP_PKEY_CTX_set_dh_paramgen_generator (PgenCtx, (INT32)Generator) <= 0) {
    goto Fail;
  }

  if (EVP_PKEY_paramgen (PgenCtx, &ParamsPkey) <= 0) {
    goto Fail;
  }

  //
  // Extract the generated prime p.
  //
  if (EVP_PKEY_get_bn_param (ParamsPkey, OSSL_PKEY_PARAM_FFC_P, &BnP) <= 0) {
    goto Fail;
  }

  //
  // Build a new BnG from the generator value.
  //
  BnG = BN_new ();
  if (BnG == NULL) {
    goto Fail;
  }

  if (!BN_set_word (BnG, (BN_ULONG)Generator)) {
    goto Fail;
  }

  //
  // Update context, releasing any previous params and key.
  //
  if (Ctx->BnP != NULL) {
    BN_free (Ctx->BnP);
  }

  if (Ctx->BnG != NULL) {
    BN_free (Ctx->BnG);
  }

  if (Ctx->Pkey != NULL) {
    EVP_PKEY_free (Ctx->Pkey);
    Ctx->Pkey = NULL;
  }

  Ctx->BnP = BnP;
  Ctx->BnG = BnG;
  BnP      = NULL;
  BnG      = NULL;

  BN_bn2bin (Ctx->BnP, Prime);
  RetVal = TRUE;

Fail:
  if (BnP != NULL) {
    BN_free (BnP);
  }

  if (BnG != NULL) {
    BN_free (BnG);
  }

  if (ParamsPkey != NULL) {
    EVP_PKEY_free (ParamsPkey);
  }

  if (PgenCtx != NULL) {
    EVP_PKEY_CTX_free (PgenCtx);
  }

  return RetVal;
  // MU_CHANGE [END]
}

/**
  Sets generator and prime parameters for DH.

  Given generator g, and prime number p, this function and sets DH
  context accordingly.

  If DhContext is NULL, then return FALSE.
  If Prime is NULL, then return FALSE.

  @param[in, out]  DhContext    Pointer to the DH context.
  @param[in]       Generator    Value of generator.
  @param[in]       PrimeLength  Length in bits of prime to be generated.
  @param[in]       Prime        Pointer to the prime number.

  @retval TRUE   DH parameter setting succeeded.
  @retval FALSE  Value of Generator is not supported.
  @retval FALSE  Value of Generator is not suitable for the Prime.
  @retval FALSE  Value of Prime is not a prime number.
  @retval FALSE  Value of Prime is not a safe prime number.

**/
BOOLEAN
EFIAPI
DhSetParameter (
  IN OUT  VOID         *DhContext,
  IN      UINTN        Generator,
  IN      UINTN        PrimeLength,
  IN      CONST UINT8  *Prime
  )
{
  // MU_CHANGE [BEGIN]
  DH_PKEY_CTX  *Ctx;
  BIGNUM       *BnP;
  BIGNUM       *BnG;

  BnP = NULL;
  BnG = NULL;
  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((DhContext == NULL) || (Prime == NULL) || (PrimeLength > INT_MAX)) {
    return FALSE;
  }

  if ((Generator != DH_GENERATOR_2) && (Generator != DH_GENERATOR_5)) {
    return FALSE;
  }

  //
  // Convert prime bytes to BIGNUM and build generator BIGNUM.  // MU_CHANGE
  //
  // MU_CHANGE [BEGIN]
  BnP = BN_bin2bn ((const unsigned char *)Prime, (INT32)(PrimeLength / 8), NULL);
  BnG = BN_new ();
  if ((BnP == NULL) || (BnG == NULL)) {
    // MU_CHANGE [END]
    goto Error;
  }

  // MU_CHANGE [BEGIN]
  if (!BN_set_word (BnG, (BN_ULONG)Generator)) {
    goto Error;
  }

  //
  // Store into context, releasing any previous state.
  //
  Ctx = (DH_PKEY_CTX *)DhContext;

  if (Ctx->BnP != NULL) {
    BN_free (Ctx->BnP);
  }

  if (Ctx->BnG != NULL) {
    BN_free (Ctx->BnG);
  }

  if (Ctx->Pkey != NULL) {
    EVP_PKEY_free (Ctx->Pkey);
    Ctx->Pkey = NULL;
  }

  Ctx->BnP = BnP;
  Ctx->BnG = BnG;

  // MU_CHANGE [END]
  return TRUE;

Error:
  if (BnP != NULL) {
    BN_free (BnP);
  }

  if (BnG != NULL) {
    BN_free (BnG);
  }

  return FALSE;
}

/**
  Generates DH public key.

  This function generates random secret exponent, and computes the public key, which is
  returned via parameter PublicKey and PublicKeySize. DH context is updated accordingly.
  If the PublicKey buffer is too small to hold the public key, FALSE is returned and
  PublicKeySize is set to the required buffer size to obtain the public key.

  If DhContext is NULL, then return FALSE.
  If PublicKeySize is NULL, then return FALSE.
  If PublicKeySize is large enough but PublicKey is NULL, then return FALSE.

  @param[in, out]  DhContext      Pointer to the DH context.
  @param[out]      PublicKey      Pointer to the buffer to receive generated public key.
  @param[in, out]  PublicKeySize  On input, the size of PublicKey buffer in bytes.
                                  On output, the size of data returned in PublicKey buffer in bytes.

  @retval TRUE   DH public key generation succeeded.
  @retval FALSE  DH public key generation failed.
  @retval FALSE  PublicKeySize is not large enough.

**/
BOOLEAN
EFIAPI
DhGenerateKey (
  IN OUT  VOID   *DhContext,
  OUT     UINT8  *PublicKey,
  IN OUT  UINTN  *PublicKeySize
  )
{
  // MU_CHANGE [BEGIN]
  BOOLEAN         RetVal;
  DH_PKEY_CTX     *Ctx;
  OSSL_PARAM_BLD  *Bld;
  OSSL_PARAM      *Params;
  EVP_PKEY_CTX    *ParamCtx;
  EVP_PKEY        *DhParamsPkey;
  EVP_PKEY_CTX    *KeygenCtx;
  BIGNUM          *BnPubKey;
  INTN            Size;

  RetVal       = FALSE;
  Bld          = NULL;
  Params       = NULL;
  ParamCtx     = NULL;
  DhParamsPkey = NULL;
  KeygenCtx    = NULL;
  BnPubKey     = NULL;
  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((DhContext == NULL) || (PublicKeySize == NULL)) {
    return FALSE;
  }

  if ((PublicKey == NULL) && (*PublicKeySize != 0)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  Ctx = (DH_PKEY_CTX *)DhContext;

  if ((Ctx->BnP == NULL) || (Ctx->BnG == NULL)) {
    return FALSE;
  }

  //
  // Release any previously generated key.
  //
  if (Ctx->Pkey != NULL) {
    EVP_PKEY_free (Ctx->Pkey);
    Ctx->Pkey = NULL;
  }

  //
  // Build an EVP_PKEY carrying only the DH domain parameters (p, g).
  //
  Bld = OSSL_PARAM_BLD_new ();
  if (Bld == NULL) {
    goto Fail;
  }

  if (!OSSL_PARAM_BLD_push_BN (Bld, OSSL_PKEY_PARAM_FFC_P, Ctx->BnP)) {
    goto Fail;
  }

  if (!OSSL_PARAM_BLD_push_BN (Bld, OSSL_PKEY_PARAM_FFC_G, Ctx->BnG)) {
    goto Fail;
  }

  Params = OSSL_PARAM_BLD_to_param (Bld);
  if (Params == NULL) {
    goto Fail;
  }

  ParamCtx = EVP_PKEY_CTX_new_from_name (NULL, "DH", NULL);
  if (ParamCtx == NULL) {
    goto Fail;
  }

  if (EVP_PKEY_fromdata_init (ParamCtx) <= 0) {
    goto Fail;
  }

  if (EVP_PKEY_fromdata (ParamCtx, &DhParamsPkey, EVP_PKEY_KEY_PARAMETERS, Params) <= 0) {
    goto Fail;
  }

  // MU_CHANGE [END]

  // MU_CHANGE [BEGIN]
  //
  // Generate the DH key pair from the domain parameters.
  //
  KeygenCtx = EVP_PKEY_CTX_new (DhParamsPkey, NULL);
  if (KeygenCtx == NULL) {
    goto Fail;
  }

  // MU_CHANGE [END]

  // MU_CHANGE [BEGIN]
  if (EVP_PKEY_keygen_init (KeygenCtx) <= 0) {
    goto Fail;
  }

  // MU_CHANGE [END]

  // MU_CHANGE [BEGIN]
  if (EVP_PKEY_keygen (KeygenCtx, &Ctx->Pkey) <= 0) {
    goto Fail;
  }

  //
  // Extract the public key as a BIGNUM.
  //
  if (EVP_PKEY_get_bn_param (Ctx->Pkey, OSSL_PKEY_PARAM_PUB_KEY, &BnPubKey) <= 0) {
    goto Fail;
  }

  Size = BN_num_bytes (BnPubKey);
  if ((Size > 0) && (*PublicKeySize < (UINTN)Size)) {
    // MU_CHANGE [END]
    *PublicKeySize = Size;
    // MU_CHANGE [BEGIN]
    //
    // Keep Ctx->Pkey; caller may retry with a larger buffer, but free the
    // temporary BnPubKey extracted for this attempt to avoid leaking it.
    //
    BN_free (BnPubKey);
    BnPubKey = NULL;
    goto Fail;
  }

  if (PublicKey != NULL) {
    BN_bn2bin (BnPubKey, PublicKey);
  }

  *PublicKeySize = Size;
  RetVal         = TRUE;

Fail:
  if (BnPubKey != NULL) {
    BN_free (BnPubKey);
  }

  if (KeygenCtx != NULL) {
    EVP_PKEY_CTX_free (KeygenCtx);
  }

  if (DhParamsPkey != NULL) {
    EVP_PKEY_free (DhParamsPkey);
  }

  if (ParamCtx != NULL) {
    EVP_PKEY_CTX_free (ParamCtx);
  }

  if (Params != NULL) {
    OSSL_PARAM_free (Params);
  }

  if (Bld != NULL) {
    OSSL_PARAM_BLD_free (Bld);
    // MU_CHANGE [END]
  }

  return RetVal;
}

/**
  Computes exchanged common key.

  Given peer's public key, this function computes the exchanged common key, based on its own
  context including value of prime modulus and random secret exponent.

  If DhContext is NULL, then return FALSE.
  If PeerPublicKey is NULL, then return FALSE.
  If KeySize is NULL, then return FALSE.
  If Key is NULL, then return FALSE.
  If KeySize is not large enough, then return FALSE.

  @param[in, out]  DhContext          Pointer to the DH context.
  @param[in]       PeerPublicKey      Pointer to the peer's public key.
  @param[in]       PeerPublicKeySize  Size of peer's public key in bytes.
  @param[out]      Key                Pointer to the buffer to receive generated key.
  @param[in, out]  KeySize            On input, the size of Key buffer in bytes.
                                      On output, the size of data returned in Key buffer in bytes.

  @retval TRUE   DH exchanged key generation succeeded.
  @retval FALSE  DH exchanged key generation failed.
  @retval FALSE  KeySize is not large enough.

**/
BOOLEAN
EFIAPI
DhComputeKey (
  IN OUT  VOID         *DhContext,
  IN      CONST UINT8  *PeerPublicKey,
  IN      UINTN        PeerPublicKeySize,
  OUT     UINT8        *Key,
  IN OUT  UINTN        *KeySize
  )
{
  // MU_CHANGE [BEGIN]
  BOOLEAN         RetVal;
  DH_PKEY_CTX     *Ctx;
  BIGNUM          *BnPeerPubKey;
  OSSL_PARAM_BLD  *Bld;
  OSSL_PARAM      *Params;
  EVP_PKEY_CTX    *FromdataCtx;
  EVP_PKEY        *PeerPkey;
  EVP_PKEY_CTX    *DeriveCtx;
  UINTN           SharedKeyLen;

  RetVal       = FALSE;
  BnPeerPubKey = NULL;
  Bld          = NULL;
  Params       = NULL;
  FromdataCtx  = NULL;
  PeerPkey     = NULL;
  DeriveCtx    = NULL;
  SharedKeyLen = 0;
  // MU_CHANGE [END]

  //
  // Check input parameters.
  //
  if ((DhContext == NULL) || (PeerPublicKey == NULL) || (KeySize == NULL) || (Key == NULL)) {
    return FALSE;
  }

  if (PeerPublicKeySize > INT_MAX) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  Ctx = (DH_PKEY_CTX *)DhContext;

  if ((Ctx->Pkey == NULL) || (Ctx->BnP == NULL) || (Ctx->BnG == NULL)) {
    // MU_CHANGE [END]
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  //
  // Convert peer's public key bytes to BIGNUM.
  //
  BnPeerPubKey = BN_bin2bn ((const unsigned char *)PeerPublicKey, (INT32)PeerPublicKeySize, NULL);
  if (BnPeerPubKey == NULL) {
    goto Fail;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  //
  // Build a peer EVP_PKEY with p, g, and the peer's public key.
  //
  Bld = OSSL_PARAM_BLD_new ();
  if (Bld == NULL) {
    goto Fail;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  if (!OSSL_PARAM_BLD_push_BN (Bld, OSSL_PKEY_PARAM_FFC_P, Ctx->BnP)) {
    goto Fail;
  }

  if (!OSSL_PARAM_BLD_push_BN (Bld, OSSL_PKEY_PARAM_FFC_G, Ctx->BnG)) {
    goto Fail;
  }

  if (!OSSL_PARAM_BLD_push_BN (Bld, OSSL_PKEY_PARAM_PUB_KEY, BnPeerPubKey)) {
    goto Fail;
  }

  Params = OSSL_PARAM_BLD_to_param (Bld);
  if (Params == NULL) {
    goto Fail;
  }

  FromdataCtx = EVP_PKEY_CTX_new_from_name (NULL, "DH", NULL);
  if (FromdataCtx == NULL) {
    goto Fail;
  }

  if (EVP_PKEY_fromdata_init (FromdataCtx) <= 0) {
    goto Fail;
  }

  if (EVP_PKEY_fromdata (FromdataCtx, &PeerPkey, EVP_PKEY_PUBLIC_KEY, Params) <= 0) {
    goto Fail;
  }

  //
  // Derive the shared secret.
  //
  DeriveCtx = EVP_PKEY_CTX_new (Ctx->Pkey, NULL);
  if (DeriveCtx == NULL) {
    goto Fail;
  }

  if (EVP_PKEY_derive_init (DeriveCtx) <= 0) {
    goto Fail;
  }

  if (EVP_PKEY_derive_set_peer (DeriveCtx, PeerPkey) <= 0) {
    goto Fail;
  }

  //
  // First, query the required shared key length.
  //
  if (EVP_PKEY_derive (DeriveCtx, NULL, &SharedKeyLen) <= 0) {
    goto Fail;
  }

  //
  // If the caller-provided buffer is too small, report the required size
  // while returning FALSE, preserving the previous behavior.
  //
  if (*KeySize < SharedKeyLen) {
    *KeySize = SharedKeyLen;
    goto Fail;
  }

  //
  // Derive the shared key into the caller-provided buffer.
  //
  if (EVP_PKEY_derive (DeriveCtx, Key, &SharedKeyLen) <= 0) {
    goto Fail;
  }

  *KeySize = SharedKeyLen;
  RetVal   = TRUE;

Fail:
  if (DeriveCtx != NULL) {
    EVP_PKEY_CTX_free (DeriveCtx);
  }

  if (PeerPkey != NULL) {
    EVP_PKEY_free (PeerPkey);
  }

  if (FromdataCtx != NULL) {
    EVP_PKEY_CTX_free (FromdataCtx);
  }

  if (Params != NULL) {
    OSSL_PARAM_free (Params);
  }

  if (Bld != NULL) {
    OSSL_PARAM_BLD_free (Bld);
  }

  if (BnPeerPubKey != NULL) {
    BN_free (BnPeerPubKey);
  }

  return RetVal;
  // MU_CHANGE [END]
}
