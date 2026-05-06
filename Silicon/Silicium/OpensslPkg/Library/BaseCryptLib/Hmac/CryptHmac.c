/** @file
  HMAC-SHA256/SHA384 Wrapper Implementation over OpenSSL.

Copyright (c) 2016 - 2022, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "InternalCryptLib.h"
// MU_CHANGE [BEGIN]
#include <openssl/evp.h>
#include <openssl/params.h>
#include <openssl/core_names.h>

//
// Wrapper structure to hold EVP_MAC_CTX so that HmacMdDuplicate can
// replace the inner context while preserving the outer pointer.
//
typedef struct {
  EVP_MAC_CTX    *Ctx;
} HMAC_CTX_WRAPPER;

/**
  Allocates and initializes one EVP_MAC_CTX context for subsequent HMAC-MD use.  // MU_CHANGE

  @return  Pointer to the HMAC_CTX_WRAPPER that has been initialized.
           If the allocation fails, HmacMdNew() returns NULL.

**/
// MU_CHANGE [END]
STATIC
VOID *
HmacMdNew (
  VOID
  )
{
  // MU_CHANGE [BEGIN]
  EVP_MAC           *Mac;
  HMAC_CTX_WRAPPER  *Wrapper;

  //
  // Fetch the HMAC algorithm from the default provider.
  //
  Mac = EVP_MAC_fetch (NULL, "HMAC", NULL);
  if (Mac == NULL) {
    return NULL;
  }

  Wrapper = AllocateZeroPool (sizeof (HMAC_CTX_WRAPPER));
  if (Wrapper == NULL) {
    EVP_MAC_free (Mac);
    return NULL;
  }

  //
  // Allocate EVP_MAC_CTX.  The context holds its own reference to the
  // EVP_MAC object, so we can free it immediately after ctx creation.
  //
  Wrapper->Ctx = EVP_MAC_CTX_new (Mac);
  EVP_MAC_free (Mac);

  if (Wrapper->Ctx == NULL) {
    FreePool (Wrapper);
    return NULL;
  }

  return (VOID *)Wrapper;
  // MU_CHANGE [END]
}

/**
  Release the specified HMAC_CTX_WRAPPER context.  // MU_CHANGE

  @param[in]  HmacMdCtx  Pointer to the HMAC_CTX_WRAPPER context to be released.  // MU_CHANGE

**/
STATIC
VOID
HmacMdFree (
  IN  VOID  *HmacMdCtx
  )
{
  // MU_CHANGE [BEGIN]
  HMAC_CTX_WRAPPER  *Wrapper;

  if (HmacMdCtx != NULL) {
    Wrapper = (HMAC_CTX_WRAPPER *)HmacMdCtx;
    EVP_MAC_CTX_free (Wrapper->Ctx);
    FreePool (Wrapper);
  }

  // MU_CHANGE [END]
}

/**
  Set user-supplied key for subsequent use. It must be done before any
  calling to HmacMdUpdate().

  If HmacMdContext is NULL, then return FALSE.

  @param[in]   MdName             Digest algorithm name (e.g. "SHA256").  // MU_CHANGE
  @param[out]  HmacMdContext      Pointer to HMAC-MD context.
  @param[in]   Key                Pointer to the user-supplied key.
  @param[in]   KeySize            Key size in bytes.

  @retval TRUE   The Key is set successfully.
  @retval FALSE  The Key is set unsuccessfully.

**/
STATIC
BOOLEAN
HmacMdSetKey (
  // MU_CHANGE [BEGIN]
  IN      CONST CHAR8  *MdName,
  IN OUT  VOID         *HmacMdContext,
  IN      CONST UINT8  *Key,
  IN      UINTN        KeySize
  // MU_CHANGE [END]
  )
{
  // MU_CHANGE [BEGIN]
  HMAC_CTX_WRAPPER  *Wrapper;
  OSSL_PARAM        Params[2];

  //
  // Check input parameters.
  //
  if ((HmacMdContext == NULL) || (KeySize > INT_MAX) || (Key == NULL) || (MdName == NULL)) {
    return FALSE;
  }

  Wrapper = (HMAC_CTX_WRAPPER *)HmacMdContext;

  Params[0] = OSSL_PARAM_construct_utf8_string (
                OSSL_MAC_PARAM_DIGEST,
                (char *)MdName,
                0
                );
  Params[1] = OSSL_PARAM_construct_end ();

  if (EVP_MAC_init (Wrapper->Ctx, Key, (size_t)KeySize, Params) != 1) {
    return FALSE;
  }

  // MU_CHANGE [END]

  return TRUE;
}

/**
  Makes a copy of an existing HMAC-MD context.

  If HmacMdContext is NULL, then return FALSE.
  If NewHmacMdContext is NULL, then return FALSE.

  @param[in]  HmacMdContext     Pointer to HMAC-MD context being copied.
  @param[out] NewHmacMdContext  Pointer to new HMAC-MD context.

  @retval TRUE   HMAC-MD context copy succeeded.
  @retval FALSE  HMAC-MD context copy failed.

**/
STATIC
BOOLEAN
HmacMdDuplicate (
  IN   CONST VOID  *HmacMdContext,
  OUT  VOID        *NewHmacMdContext
  )
{
  // MU_CHANGE [BEGIN]
  HMAC_CTX_WRAPPER  *SrcWrapper;
  HMAC_CTX_WRAPPER  *DstWrapper;
  EVP_MAC_CTX       *NewCtx;

  //
  // Check input parameters.
  //
  if ((HmacMdContext == NULL) || (NewHmacMdContext == NULL)) {
    return FALSE;
  }

  SrcWrapper = (HMAC_CTX_WRAPPER *)HmacMdContext;
  DstWrapper = (HMAC_CTX_WRAPPER *)NewHmacMdContext;

  NewCtx = EVP_MAC_CTX_dup (SrcWrapper->Ctx);
  if (NewCtx == NULL) {
    return FALSE;
  }

  //
  // Replace the destination's inner context with the duplicated one.
  //
  EVP_MAC_CTX_free (DstWrapper->Ctx);
  DstWrapper->Ctx = NewCtx;

  // MU_CHANGE [END]
  return TRUE;
}

/**
  Digests the input data and updates HMAC-MD context.

  This function performs HMAC-MD digest on a data buffer of the specified size.
  It can be called multiple times to compute the digest of long or discontinuous data streams.
  HMAC-MD context should be initialized by HmacMdNew(), and should not be finalized
  by HmacMdFinal(). Behavior with invalid context is undefined.

  If HmacMdContext is NULL, then return FALSE.

  @param[in, out]  HmacMdContext     Pointer to the HMAC-MD context.
  @param[in]       Data              Pointer to the buffer containing the data to be digested.
  @param[in]       DataSize          Size of Data buffer in bytes.

  @retval TRUE   HMAC-MD data digest succeeded.
  @retval FALSE  HMAC-MD data digest failed.

**/
STATIC
BOOLEAN
HmacMdUpdate (
  IN OUT  VOID        *HmacMdContext,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  )
{
  // MU_CHANGE [BEGIN]
  HMAC_CTX_WRAPPER  *Wrapper;

  //
  // Check input parameters.
  //
  if (HmacMdContext == NULL) {
    return FALSE;
  }

  //
  // Check invalid parameters, in case that only DataLength was checked in OpenSSL
  //
  if ((Data == NULL) && (DataSize != 0)) {
    return FALSE;
  }

  Wrapper = (HMAC_CTX_WRAPPER *)HmacMdContext;

  //
  // OpenSSL HMAC-MD digest update
  //
  if (EVP_MAC_update (Wrapper->Ctx, Data, DataSize) != 1) {
    return FALSE;
  }

  // MU_CHANGE [END]

  return TRUE;
}

/**
  Completes computation of the HMAC-MD digest value.

  This function completes HMAC-MD hash computation and retrieves the digest value into
  the specified memory. After this function has been called, the HMAC-MD context cannot
  be used again.
  HMAC-MD context should be initialized by HmacMdNew(), and should not be finalized
  by HmacMdFinal(). Behavior with invalid HMAC-MD context is undefined.

  If HmacMdContext is NULL, then return FALSE.
  If HmacValue is NULL, then return FALSE.

  @param[in, out]  HmacMdContext      Pointer to the HMAC-MD context.
  @param[out]      HmacValue          Pointer to a buffer that receives the HMAC-MD digest
                                      value.

  @retval TRUE   HMAC-MD digest computation succeeded.
  @retval FALSE  HMAC-MD digest computation failed.

**/
STATIC
BOOLEAN
HmacMdFinal (
  IN OUT  VOID   *HmacMdContext,
  OUT     UINT8  *HmacValue
  )
{
  // MU_CHANGE [BEGIN]
  HMAC_CTX_WRAPPER  *Wrapper;
  size_t            MacSize;
  size_t            Length;

  //
  // Check input parameters.
  //
  if ((HmacMdContext == NULL) || (HmacValue == NULL)) {
    return FALSE;
  }

  Wrapper = (HMAC_CTX_WRAPPER *)HmacMdContext;

  //
  // OpenSSL HMAC-MD digest finalization
  //
  MacSize = EVP_MAC_CTX_get_mac_size (Wrapper->Ctx);
  if (EVP_MAC_final (Wrapper->Ctx, HmacValue, &Length, MacSize) != 1) {
    return FALSE;
  }

  // MU_CHANGE [END]

  return TRUE;
}

/**
  Computes the HMAC-MD digest of a input data buffer.

  This function performs the HMAC-MD digest of a given data buffer, and places
  the digest value into the specified memory.

  If this interface is not supported, then return FALSE.
  If HmacValue is NULL, then return FALSE.

  @param[in]   MdName      Digest algorithm name (e.g. "SHA256").  // MU_CHANGE
  @param[in]   Data        Pointer to the buffer containing the data to be digested.
  @param[in]   DataSize    Size of Data buffer in bytes.
  @param[in]   Key         Pointer to the user-supplied key.
  @param[in]   KeySize     Key size in bytes.
  @param[out]  HmacValue   Pointer to a buffer that receives the HMAC-MD digest
                           value.

  @retval TRUE   HMAC-MD digest computation succeeded.
  @retval FALSE  HMAC-MD digest computation failed.
  @retval FALSE  This interface is not supported.

**/
STATIC
BOOLEAN
HmacMdAll (
  // MU_CHANGE [BEGIN]
  IN   CONST CHAR8  *MdName,
  IN   CONST VOID   *Data,
  IN   UINTN        DataSize,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  OUT  UINT8        *HmacValue
  // MU_CHANGE [END]
  )
{
  // MU_CHANGE [BEGIN]
  EVP_MAC      *Mac;
  EVP_MAC_CTX  *Ctx;
  OSSL_PARAM   Params[2];
  size_t       MacSize;
  size_t       Length;
  BOOLEAN      RetVal;

  //
  // Check input parameters.
  //
  if ((MdName == NULL) || (Data == NULL) || (Key == NULL) || (HmacValue == NULL) || (KeySize > INT_MAX) || (DataSize > INT_MAX)) {
    return FALSE;
  }

  Mac = EVP_MAC_fetch (NULL, "HMAC", NULL);
  if (Mac == NULL) {
    return FALSE;
  }

  Ctx = EVP_MAC_CTX_new (Mac);
  EVP_MAC_free (Mac);
  if (Ctx == NULL) {
    return FALSE;
  }

  Params[0] = OSSL_PARAM_construct_utf8_string (
                OSSL_MAC_PARAM_DIGEST,
                (char *)MdName,
                0
                );
  Params[1] = OSSL_PARAM_construct_end ();

  RetVal = (BOOLEAN)(EVP_MAC_init (Ctx, Key, (UINTN)KeySize, Params) == 1);  // MU_CHANGE
  if (!RetVal) {
    goto Done;
  }

  RetVal = (BOOLEAN)(EVP_MAC_update (Ctx, Data, (UINTN)DataSize) == 1);  // MU_CHANGE
  if (!RetVal) {
    goto Done;
  }

  MacSize = EVP_MAC_CTX_get_mac_size (Ctx);
  RetVal  = (BOOLEAN)(EVP_MAC_final (Ctx, HmacValue, &Length, MacSize) == 1);
  // MU_CHANGE [END]

Done:
  EVP_MAC_CTX_free (Ctx);  // MU_CHANGE

  return RetVal;
}

/**
  Allocates and initializes one HMAC context for subsequent HMAC-SHA256 use.  // MU_CHANGE

  @return  Pointer to the HMAC context that has been initialized.  // MU_CHANGE
           If the allocations fails, HmacSha256New() returns NULL.

**/
VOID *
EFIAPI
HmacSha256New (
  VOID
  )
{
  return HmacMdNew ();
}

/**
  Release the specified HMAC context.  // MU_CHANGE

  @param[in]  HmacSha256Ctx  Pointer to the HMAC context to be released.  // MU_CHANGE

**/
VOID
EFIAPI
HmacSha256Free (
  IN  VOID  *HmacSha256Ctx
  )
{
  HmacMdFree (HmacSha256Ctx);
}

/**
  Set user-supplied key for subsequent use. It must be done before any
  calling to HmacSha256Update().

  If HmacSha256Context is NULL, then return FALSE.

  @param[out]  HmacSha256Context  Pointer to HMAC-SHA256 context.
  @param[in]   Key                Pointer to the user-supplied key.
  @param[in]   KeySize            Key size in bytes.

  @retval TRUE   The Key is set successfully.
  @retval FALSE  The Key is set unsuccessfully.

**/
BOOLEAN
EFIAPI
HmacSha256SetKey (
  OUT  VOID         *HmacSha256Context,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize
  )
{
  return HmacMdSetKey ("SHA256", HmacSha256Context, Key, KeySize);  // MU_CHANGE
}

/**
  Makes a copy of an existing HMAC-SHA256 context.

  If HmacSha256Context is NULL, then return FALSE.
  If NewHmacSha256Context is NULL, then return FALSE.

  @param[in]  HmacSha256Context     Pointer to HMAC-SHA256 context being copied.
  @param[out] NewHmacSha256Context  Pointer to new HMAC-SHA256 context.

  @retval TRUE   HMAC-SHA256 context copy succeeded.
  @retval FALSE  HMAC-SHA256 context copy failed.

**/
BOOLEAN
EFIAPI
HmacSha256Duplicate (
  IN   CONST VOID  *HmacSha256Context,
  OUT  VOID        *NewHmacSha256Context
  )
{
  return HmacMdDuplicate (HmacSha256Context, NewHmacSha256Context);
}

/**
  Digests the input data and updates HMAC-SHA256 context.

  This function performs HMAC-SHA256 digest on a data buffer of the specified size.
  It can be called multiple times to compute the digest of long or discontinuous data streams.
  HMAC-SHA256 context should be initialized by HmacSha256New(), and should not be finalized
  by HmacSha256Final(). Behavior with invalid context is undefined.

  If HmacSha256Context is NULL, then return FALSE.

  @param[in, out]  HmacSha256Context Pointer to the HMAC-SHA256 context.
  @param[in]       Data              Pointer to the buffer containing the data to be digested.
  @param[in]       DataSize          Size of Data buffer in bytes.

  @retval TRUE   HMAC-SHA256 data digest succeeded.
  @retval FALSE  HMAC-SHA256 data digest failed.

**/
BOOLEAN
EFIAPI
HmacSha256Update (
  IN OUT  VOID        *HmacSha256Context,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  )
{
  return HmacMdUpdate (HmacSha256Context, Data, DataSize);
}

/**
  Completes computation of the HMAC-SHA256 digest value.

  This function completes HMAC-SHA256 hash computation and retrieves the digest value into
  the specified memory. After this function has been called, the HMAC-SHA256 context cannot
  be used again.
  HMAC-SHA256 context should be initialized by HmacSha256New(), and should not be finalized
  by HmacSha256Final(). Behavior with invalid HMAC-SHA256 context is undefined.

  If HmacSha256Context is NULL, then return FALSE.
  If HmacValue is NULL, then return FALSE.

  @param[in, out]  HmacSha256Context  Pointer to the HMAC-SHA256 context.
  @param[out]      HmacValue          Pointer to a buffer that receives the HMAC-SHA256 digest
                                      value (32 bytes).

  @retval TRUE   HMAC-SHA256 digest computation succeeded.
  @retval FALSE  HMAC-SHA256 digest computation failed.

**/
BOOLEAN
EFIAPI
HmacSha256Final (
  IN OUT  VOID   *HmacSha256Context,
  OUT     UINT8  *HmacValue
  )
{
  return HmacMdFinal (HmacSha256Context, HmacValue);
}

/**
  Computes the HMAC-SHA256 digest of a input data buffer.

  This function performs the HMAC-SHA256 digest of a given data buffer, and places
  the digest value into the specified memory.

  If this interface is not supported, then return FALSE.

  @param[in]   Data        Pointer to the buffer containing the data to be digested.
  @param[in]   DataSize    Size of Data buffer in bytes.
  @param[in]   Key         Pointer to the user-supplied key.
  @param[in]   KeySize     Key size in bytes.
  @param[out]  HmacValue   Pointer to a buffer that receives the HMAC-SHA256 digest
                           value (32 bytes).

  @retval TRUE   HMAC-SHA256 digest computation succeeded.
  @retval FALSE  HMAC-SHA256 digest computation failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
HmacSha256All (
  IN   CONST VOID   *Data,
  IN   UINTN        DataSize,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  OUT  UINT8        *HmacValue
  )
{
  return HmacMdAll ("SHA256", Data, DataSize, Key, KeySize, HmacValue);  // MU_CHANGE
}

/**
  Allocates and initializes one HMAC context for subsequent HMAC-SHA384 use.  // MU_CHANGE

  @return  Pointer to the HMAC context that has been initialized.  // MU_CHANGE
           If the allocations fails, HmacSha384New() returns NULL.

**/
VOID *
EFIAPI
HmacSha384New (
  VOID
  )
{
  return HmacMdNew ();
}

/**
  Release the specified HMAC context.  // MU_CHANGE

  @param[in]  HmacSha384Ctx  Pointer to the HMAC context to be released.  // MU_CHANGE

**/
VOID
EFIAPI
HmacSha384Free (
  IN  VOID  *HmacSha384Ctx
  )
{
  HmacMdFree (HmacSha384Ctx);
}

/**
  Set user-supplied key for subsequent use. It must be done before any
  calling to HmacSha384Update().

  If HmacSha384Context is NULL, then return FALSE.
  If this interface is not supported, then return FALSE.

  @param[out]  HmacSha384Context  Pointer to HMAC-SHA384 context.
  @param[in]   Key                Pointer to the user-supplied key.
  @param[in]   KeySize            Key size in bytes.

  @retval TRUE   The Key is set successfully.
  @retval FALSE  The Key is set unsuccessfully.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
HmacSha384SetKey (
  OUT  VOID         *HmacSha384Context,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize
  )
{
  return HmacMdSetKey ("SHA384", HmacSha384Context, Key, KeySize);  // MU_CHANGE
}

/**
  Makes a copy of an existing HMAC-SHA384 context.

  If HmacSha384Context is NULL, then return FALSE.
  If NewHmacSha384Context is NULL, then return FALSE.
  If this interface is not supported, then return FALSE.

  @param[in]  HmacSha384Context     Pointer to HMAC-SHA384 context being copied.
  @param[out] NewHmacSha384Context  Pointer to new HMAC-SHA384 context.

  @retval TRUE   HMAC-SHA384 context copy succeeded.
  @retval FALSE  HMAC-SHA384 context copy failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
HmacSha384Duplicate (
  IN   CONST VOID  *HmacSha384Context,
  OUT  VOID        *NewHmacSha384Context
  )
{
  return HmacMdDuplicate (HmacSha384Context, NewHmacSha384Context);
}

/**
  Digests the input data and updates HMAC-SHA384 context.

  This function performs HMAC-SHA384 digest on a data buffer of the specified size.
  It can be called multiple times to compute the digest of long or discontinuous data streams.
  HMAC-SHA384 context should be initialized by HmacSha384New(), and should not be finalized
  by HmacSha384Final(). Behavior with invalid context is undefined.

  If HmacSha384Context is NULL, then return FALSE.
  If this interface is not supported, then return FALSE.

  @param[in, out]  HmacSha384Context Pointer to the HMAC-SHA384 context.
  @param[in]       Data              Pointer to the buffer containing the data to be digested.
  @param[in]       DataSize          Size of Data buffer in bytes.

  @retval TRUE   HMAC-SHA384 data digest succeeded.
  @retval FALSE  HMAC-SHA384 data digest failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
HmacSha384Update (
  IN OUT  VOID        *HmacSha384Context,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  )
{
  return HmacMdUpdate (HmacSha384Context, Data, DataSize);
}

/**
  Completes computation of the HMAC-SHA384 digest value.

  This function completes HMAC-SHA384 hash computation and retrieves the digest value into
  the specified memory. After this function has been called, the HMAC-SHA384 context cannot
  be used again.
  HMAC-SHA384 context should be initialized by HmacSha384New(), and should not be finalized
  by HmacSha384Final(). Behavior with invalid HMAC-SHA384 context is undefined.

  If HmacSha384Context is NULL, then return FALSE.
  If HmacValue is NULL, then return FALSE.
  If this interface is not supported, then return FALSE.

  @param[in, out]  HmacSha384Context  Pointer to the HMAC-SHA384 context.
  @param[out]      HmacValue          Pointer to a buffer that receives the HMAC-SHA384 digest
                                      value (48 bytes).

  @retval TRUE   HMAC-SHA384 digest computation succeeded.
  @retval FALSE  HMAC-SHA384 digest computation failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
HmacSha384Final (
  IN OUT  VOID   *HmacSha384Context,
  OUT     UINT8  *HmacValue
  )
{
  return HmacMdFinal (HmacSha384Context, HmacValue);
}

/**
  Computes the HMAC-SHA384 digest of a input data buffer.

  This function performs the HMAC-SHA384 digest of a given data buffer, and places
  the digest value into the specified memory.

  If this interface is not supported, then return FALSE.

  @param[in]   Data        Pointer to the buffer containing the data to be digested.
  @param[in]   DataSize    Size of Data buffer in bytes.
  @param[in]   Key         Pointer to the user-supplied key.
  @param[in]   KeySize     Key size in bytes.
  @param[out]  HmacValue   Pointer to a buffer that receives the HMAC-SHA384 digest
                           value (48 bytes).

  @retval TRUE   HMAC-SHA384 digest computation succeeded.
  @retval FALSE  HMAC-SHA384 digest computation failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
HmacSha384All (
  IN   CONST VOID   *Data,
  IN   UINTN        DataSize,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  OUT  UINT8        *HmacValue
  )
{
  return HmacMdAll ("SHA384", Data, DataSize, Key, KeySize, HmacValue);  // MU_CHANGE
}
