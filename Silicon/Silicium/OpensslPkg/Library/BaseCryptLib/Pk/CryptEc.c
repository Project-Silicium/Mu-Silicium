/** @file
  Elliptic Curve and ECDH API implementation based on OpenSSL

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "InternalCryptLib.h"
#include <openssl/objects.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
// MU_CHANGE [BEGIN]
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/param_build.h>
#include <openssl/core_names.h>
#include "CryptEcPkeyCtx.h"
// MU_CHANGE [END]

// =====================================================================================
//    Basic Elliptic Curve Primitives
// =====================================================================================

/**
  Return the Nid of certain ECC curve.

  @param[in]  CryptoNid   Identifying number for the ECC curve (Defined in
                          BaseCryptLib.h).

  @retval !=-1    On success.
  @retval -1      ECC curve not supported.
**/
STATIC
INT32
CryptoNidToOpensslNid (
  IN UINTN  CryptoNid
  )
{
  INT32  Nid;

  switch (CryptoNid) {
    case CRYPTO_NID_SECP256R1:
      Nid = NID_X9_62_prime256v1;
      break;
    case CRYPTO_NID_SECP384R1:
      Nid = NID_secp384r1;
      break;
    case CRYPTO_NID_SECP521R1:
      Nid = NID_secp521r1;
      break;
    case CRYPTO_NID_BRAINPOOLP512R1:
      Nid = NID_brainpoolP512r1;
      break;
    default:
      return -1;
  }

  return Nid;
}

/**
  Initialize new opaque EcGroup object. This object represents an EC curve and
  and is used for calculation within this group. This object should be freed
  using EcGroupFree() function.

  @param[in]  CryptoNid   Identifying number for the ECC curve (Defined in
                          BaseCryptLib.h).

  @retval EcGroup object  On success.
  @retval NULL            On failure.
**/
VOID *
EFIAPI
EcGroupInit (
  IN UINTN  CryptoNid
  )
{
  INT32  Nid;

  Nid = CryptoNidToOpensslNid (CryptoNid);

  if (Nid < 0) {
    return NULL;
  }

  return EC_GROUP_new_by_curve_name (Nid);
}

/**
  Get EC curve parameters. While elliptic curve equation is Y^2 mod P = (X^3 + AX + B) Mod P.
  This function will set the provided Big Number objects  to the corresponding
  values. The caller needs to make sure all the "out" BigNumber parameters
  are properly initialized.

  @param[in]  EcGroup    EC group object.
  @param[out] BnPrime    Group prime number.
  @param[out] BnA        A coefficient.
  @param[out] BnB        B coefficient..
  @param[in]  BnCtx      BN context.

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcGroupGetCurve (
  IN CONST VOID  *EcGroup,
  OUT VOID       *BnPrime,
  OUT VOID       *BnA,
  OUT VOID       *BnB,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_GROUP_get_curve (EcGroup, BnPrime, BnA, BnB, BnCtx);
}

/**
  Get EC group order.
  This function will set the provided Big Number object to the corresponding
  value. The caller needs to make sure that the "out" BigNumber parameter
  is properly initialized.

  @param[in]  EcGroup   EC group object.
  @param[out] BnOrder   Group prime number.

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcGroupGetOrder (
  IN VOID   *EcGroup,
  OUT VOID  *BnOrder
  )
{
  return (BOOLEAN)EC_GROUP_get_order (EcGroup, BnOrder, NULL);
}

/**
  Free previously allocated EC group object using EcGroupInit().

  @param[in]  EcGroup   EC group object to free.
**/
VOID
EFIAPI
EcGroupFree (
  IN VOID  *EcGroup
  )
{
  EC_GROUP_free (EcGroup);
}

/**
  Initialize new opaque EC Point object. This object represents an EC point
  within the given EC group (curve).

  @param[in]  EC Group, properly initialized using EcGroupInit().

  @retval EC Point object  On success.
  @retval NULL             On failure.
**/
VOID *
EFIAPI
EcPointInit (
  IN CONST VOID  *EcGroup
  )
{
  return EC_POINT_new (EcGroup);
}

/**
  Free previously allocated EC Point object using EcPointInit().

  @param[in]  EcPoint   EC Point to free.
  @param[in]  Clear     TRUE iff the memory should be cleared.
**/
VOID
EFIAPI
EcPointDeInit (
  IN VOID     *EcPoint,
  IN BOOLEAN  Clear
  )
{
  if (Clear) {
    EC_POINT_clear_free (EcPoint);
  } else {
    EC_POINT_free (EcPoint);
  }
}

/**
  Get EC point affine (x,y) coordinates.
  This function will set the provided Big Number objects to the corresponding
  values. The caller needs to make sure all the "out" BigNumber parameters
  are properly initialized.

  @param[in]  EcGroup    EC group object.
  @param[in]  EcPoint    EC point object.
  @param[out] BnX        X coordinate.
  @param[out] BnY        Y coordinate.
  @param[in]  BnCtx      BN context, created with BigNumNewContext().

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointGetAffineCoordinates (
  IN CONST VOID  *EcGroup,
  IN CONST VOID  *EcPoint,
  OUT VOID       *BnX,
  OUT VOID       *BnY,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_POINT_get_affine_coordinates (EcGroup, EcPoint, BnX, BnY, BnCtx);
}

/**
  Set EC point affine (x,y) coordinates.

  @param[in]  EcGroup    EC group object.
  @param[in]  EcPoint    EC point object.
  @param[in]  BnX        X coordinate.
  @param[in]  BnY        Y coordinate.
  @param[in]  BnCtx      BN context, created with BigNumNewContext().

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointSetAffineCoordinates (
  IN CONST VOID  *EcGroup,
  IN VOID        *EcPoint,
  IN CONST VOID  *BnX,
  IN CONST VOID  *BnY,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_POINT_set_affine_coordinates (EcGroup, EcPoint, BnX, BnY, BnCtx);
}

/**
  EC Point addition. EcPointResult = EcPointA + EcPointB.

  @param[in]  EcGroup          EC group object.
  @param[out] EcPointResult    EC point to hold the result. The point should
                               be properly initialized.
  @param[in]  EcPointA         EC Point.
  @param[in]  EcPointB         EC Point.
  @param[in]  BnCtx            BN context, created with BigNumNewContext().

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointAdd (
  IN CONST VOID  *EcGroup,
  OUT VOID       *EcPointResult,
  IN CONST VOID  *EcPointA,
  IN CONST VOID  *EcPointB,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_POINT_add (EcGroup, EcPointResult, EcPointA, EcPointB, BnCtx);
}

/**
  Variable EC point multiplication. EcPointResult = EcPoint * BnPScalar.

  @param[in]  EcGroup          EC group object.
  @param[out] EcPointResult    EC point to hold the result. The point should
                               be properly initialized.
  @param[in]  EcPoint          EC Point.
  @param[in]  BnPScalar        P Scalar.
  @param[in]  BnCtx            BN context, created with BigNumNewContext().

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointMul (
  IN CONST VOID  *EcGroup,
  OUT VOID       *EcPointResult,
  IN CONST VOID  *EcPoint,
  IN CONST VOID  *BnPScalar,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_POINT_mul (EcGroup, EcPointResult, NULL, EcPoint, BnPScalar, BnCtx);
}

/**
  Calculate the inverse of the supplied EC point.

  @param[in]     EcGroup   EC group object.
  @param[in,out] EcPoint   EC point to invert.
  @param[in]     BnCtx     BN context, created with BigNumNewContext().

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointInvert (
  IN CONST VOID  *EcGroup,
  IN OUT VOID    *EcPoint,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_POINT_invert (EcGroup, EcPoint, BnCtx);
}

/**
  Check if the supplied point is on EC curve.

  @param[in]  EcGroup   EC group object.
  @param[in]  EcPoint   EC point to check.
  @param[in]  BnCtx     BN context, created with BigNumNewContext().

  @retval TRUE          On curve.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointIsOnCurve (
  IN CONST VOID  *EcGroup,
  IN CONST VOID  *EcPoint,
  IN VOID        *BnCtx
  )
{
  return EC_POINT_is_on_curve (EcGroup, EcPoint, BnCtx) == 1;
}

/**
  Check if the supplied point is at infinity.

  @param[in]  EcGroup   EC group object.
  @param[in]  EcPoint   EC point to check.

  @retval TRUE          At infinity.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointIsAtInfinity (
  IN CONST VOID  *EcGroup,
  IN CONST VOID  *EcPoint
  )
{
  return EC_POINT_is_at_infinity (EcGroup, EcPoint) == 1;
}

/**
  Check if EC points are equal.

  @param[in]  EcGroup   EC group object.
  @param[in]  EcPointA  EC point A.
  @param[in]  EcPointB  EC point B.
  @param[in]  BnCtx     BN context, created with BigNumNewContext().

  @retval TRUE          A == B.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointEqual (
  IN CONST VOID  *EcGroup,
  IN CONST VOID  *EcPointA,
  IN CONST VOID  *EcPointB,
  IN VOID        *BnCtx
  )
{
  return EC_POINT_cmp (EcGroup, EcPointA, EcPointB, BnCtx) == 0;
}

/**
  Set EC point compressed coordinates. Points can be described in terms of
  their compressed coordinates. For a point (x, y), for any given value for x
  such that the point is on the curve there will only ever be two possible
  values for y. Therefore, a point can be set using this function where BnX is
  the x coordinate and YBit is a value 0 or 1 to identify which of the two
  possible values for y should be used.

  @param[in]  EcGroup    EC group object.
  @param[in]  EcPoint    EC Point.
  @param[in]  BnX        X coordinate.
  @param[in]  YBit       0 or 1 to identify which Y value is used.
  @param[in]  BnCtx      BN context, created with BigNumNewContext().

  @retval TRUE          On success.
  @retval FALSE         Otherwise.
**/
BOOLEAN
EFIAPI
EcPointSetCompressedCoordinates (
  IN CONST VOID  *EcGroup,
  IN VOID        *EcPoint,
  IN CONST VOID  *BnX,
  IN UINT8       YBit,
  IN VOID        *BnCtx
  )
{
  return (BOOLEAN)EC_POINT_set_compressed_coordinates (EcGroup, EcPoint, BnX, YBit, BnCtx);
}

// =====================================================================================
//    Elliptic Curve Diffie Hellman Primitives
// =====================================================================================

// MU_CHANGE [BEGIN]

/**
  Map an OpenSSL NID to the curve name string used by EVP_PKEY_Q_keygen / OSSL_PARAM.

  @param[in]  Nid  OpenSSL NID for the EC curve.

  @return  ASCII curve name string, or NULL if not supported.
**/
STATIC CONST CHAR8 *
OpenSslNidToCurveName (
  INT32  Nid
  )
{
  switch (Nid) {
    case NID_X9_62_prime256v1: return "prime256v1";
    case NID_secp384r1:        return "secp384r1";
    case NID_secp521r1:        return "secp521r1";
    case NID_brainpoolP512r1:  return "brainpoolP512r1";
    default:                   return NULL;
  }
}

/**
  Return the coordinate half-size (in bytes) for a given OpenSSL NID.

  @param[in]  Nid  OpenSSL NID for the EC curve.

  @return  Byte length of one coordinate, or 0 if not supported.
**/
STATIC UINTN
GetHalfSizeFromNid (
  INT32  Nid
  )
{
  switch (Nid) {
    case NID_X9_62_prime256v1: return 32;
    case NID_secp384r1:        return 48;
    case NID_secp521r1:        return 66;
    case NID_brainpoolP512r1:  return 64;
    default:                   return 0;
  }
}

// MU_CHANGE [END]

/**
  Allocates and Initializes one Elliptic Curve Context for subsequent use
  with the NID.

  @param[in]  Nid   Identifying number for the ECC curve (Defined in
                    BaseCryptLib.h).
  @return     Pointer to the Elliptic Curve Context that has been initialized.
              If the allocations fails, EcNewByNid() returns NULL.
**/
VOID *
EFIAPI
EcNewByNid (
  IN UINTN  Nid
  )
{
  // MU_CHANGE [BEGIN]
  EC_PKEY_CTX  *EcPkeyCtx;
  INT32        OpenSslNid;

  // MU_CHANGE [END]

  OpenSslNid = CryptoNidToOpensslNid (Nid);
  if (OpenSslNid < 0) {
    return NULL;
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx = AllocateZeroPool (sizeof (EC_PKEY_CTX));
  if (EcPkeyCtx == NULL) {
    return NULL;
  }

  EcPkeyCtx->Nid  = OpenSslNid;
  EcPkeyCtx->Pkey = NULL;
  return (VOID *)EcPkeyCtx;
  // MU_CHANGE [END]
}

/**
  Release the specified EC context.

  @param[in]  EcContext  Pointer to the EC context to be released.
**/
VOID
EFIAPI
EcFree (
  IN  VOID  *EcContext
  )
{
  // MU_CHANGE [BEGIN]
  EC_PKEY_CTX  *EcPkeyCtx;

  if (EcContext == NULL) {
    return;
  }

  EcPkeyCtx = (EC_PKEY_CTX *)EcContext;
  if (EcPkeyCtx->Pkey != NULL) {
    EVP_PKEY_free (EcPkeyCtx->Pkey);
  }

  FreePool (EcPkeyCtx);
  // MU_CHANGE [END]
}

/**
  Generates EC key and returns EC public key (X, Y), Please note, this function uses
  pseudo random number generator. The caller must make sure RandomSeed()
  function was properly called before.
  The Ec context should be correctly initialized by EcNewByNid.
  This function generates random secret, and computes the public key (X, Y), which is
  returned via parameter Public, PublicSize.
  X is the first half of Public with size being PublicSize / 2,
  Y is the second half of Public with size being PublicSize / 2.
  EC context is updated accordingly.
  If the Public buffer is too small to hold the public X, Y, FALSE is returned and
  PublicSize is set to the required buffer size to obtain the public X, Y.
  For P-256, the PublicSize is 64. First 32-byte is X, Second 32-byte is Y.
  For P-384, the PublicSize is 96. First 48-byte is X, Second 48-byte is Y.
  For P-521, the PublicSize is 132. First 66-byte is X, Second 66-byte is Y.
  If EcContext is NULL, then return FALSE.
  If PublicSize is NULL, then return FALSE.
  If PublicSize is large enough but Public is NULL, then return FALSE.
  @param[in, out]  EcContext      Pointer to the EC context.
  @param[out]      PublicKey      Pointer to t buffer to receive generated public X,Y.
  @param[in, out]  PublicKeySize  On input, the size of Public buffer in bytes.
                                  On output, the size of data returned in Public buffer in bytes.
  @retval TRUE   EC public X,Y generation succeeded.
  @retval FALSE  EC public X,Y generation failed.
  @retval FALSE  PublicKeySize is not large enough.
**/
BOOLEAN
EFIAPI
EcGenerateKey (
  IN OUT  VOID   *EcContext,
  OUT     UINT8  *PublicKey,
  IN OUT  UINTN  *PublicKeySize
  )
{
  // MU_CHANGE [BEGIN]
  EC_PKEY_CTX  *EcPkeyCtx;
  CONST CHAR8  *CurveName;
  UINTN        HalfSize;
  EVP_PKEY     *Pkey;
  UINT8        PubKeyBuf[133];
  UINTN        PubKeyBufLen;

  // MU_CHANGE [END]

  if ((EcContext == NULL) || (PublicKeySize == NULL)) {
    return FALSE;
  }

  if ((PublicKey == NULL) && (*PublicKeySize != 0)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx = (EC_PKEY_CTX *)EcContext;
  HalfSize  = GetHalfSizeFromNid (EcPkeyCtx->Nid);
  if (HalfSize == 0) {
    // MU_CHANGE [END]
    return FALSE;
  }

  if (*PublicKeySize < HalfSize * 2) {
    *PublicKeySize = HalfSize * 2;
    return FALSE;
  }

  *PublicKeySize = HalfSize * 2;

  // MU_CHANGE [BEGIN]
  CurveName = OpenSslNidToCurveName (EcPkeyCtx->Nid);
  if (CurveName == NULL) {
    // MU_CHANGE [END]
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  if (EcPkeyCtx->Pkey != NULL) {
    EVP_PKEY_free (EcPkeyCtx->Pkey);
    EcPkeyCtx->Pkey = NULL;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  Pkey = EVP_PKEY_Q_keygen (NULL, NULL, "EC", CurveName);
  if (Pkey == NULL) {
    return FALSE;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx->Pkey = Pkey;

  PubKeyBufLen = sizeof (PubKeyBuf);
  if (EVP_PKEY_get_octet_string_param (
        Pkey,
        OSSL_PKEY_PARAM_PUB_KEY,
        PubKeyBuf,
        sizeof (PubKeyBuf),
        &PubKeyBufLen
        ) != 1)
  {
    return FALSE;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  // PubKeyBuf is 0x04 || X || Y; skip the leading uncompressed-point prefix
  if (PubKeyBufLen != 1 + HalfSize * 2) {
    return FALSE;
  }

  // MU_CHANGE [END]

  ZeroMem (PublicKey, *PublicKeySize);
  CopyMem (PublicKey, PubKeyBuf + 1, HalfSize * 2);  // MU_CHANGE

  return TRUE;  // MU_CHANGE
}

/**
  Gets the public key component from the established EC context.
  The Ec context should be correctly initialized by EcNewByNid, and successfully
  generate key pair from EcGenerateKey().
  For P-256, the PublicSize is 64. First 32-byte is X, Second 32-byte is Y.
  For P-384, the PublicSize is 96. First 48-byte is X, Second 48-byte is Y.
  For P-521, the PublicSize is 132. First 66-byte is X, Second 66-byte is Y.
  @param[in, out]  EcContext      Pointer to EC context being set.
  @param[out]      PublicKey      Pointer to t buffer to receive generated public X,Y.
  @param[in, out]  PublicKeySize  On input, the size of Public buffer in bytes.
                                  On output, the size of data returned in Public buffer in bytes.
  @retval  TRUE   EC key component was retrieved successfully.
  @retval  FALSE  Invalid EC key component.
**/
BOOLEAN
EFIAPI
EcGetPubKey (
  IN OUT  VOID   *EcContext,
  OUT     UINT8  *PublicKey,
  IN OUT  UINTN  *PublicKeySize
  )
{
  // MU_CHANGE [BEGIN]
  EC_PKEY_CTX  *EcPkeyCtx;
  UINTN        HalfSize;
  UINT8        PubKeyBuf[133];
  UINTN        PubKeyBufLen;

  // MU_CHANGE [END]

  if ((EcContext == NULL) || (PublicKeySize == NULL)) {
    return FALSE;
  }

  if ((PublicKey == NULL) && (*PublicKeySize != 0)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx = (EC_PKEY_CTX *)EcContext;
  HalfSize  = GetHalfSizeFromNid (EcPkeyCtx->Nid);
  if (HalfSize == 0) {
    return FALSE;
  }

  // MU_CHANGE [END]

  // MU_CHANGE [BEGIN]
  // EcKey    = (EC_KEY *)EcContext;
  // Group    = EC_KEY_get0_group (EcKey);
  // HalfSize = (EC_GROUP_get_degree (Group) + 7) / 8;
  // MU_CHANGE [END]
  if (*PublicKeySize < HalfSize * 2) {
    *PublicKeySize = HalfSize * 2;
    return FALSE;
  }

  *PublicKeySize = HalfSize * 2;

  if (EcPkeyCtx->Pkey == NULL) {
    // MU_CHANGE
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  PubKeyBufLen = sizeof (PubKeyBuf);
  if (EVP_PKEY_get_octet_string_param (
        EcPkeyCtx->Pkey,
        OSSL_PKEY_PARAM_PUB_KEY,
        PubKeyBuf,
        sizeof (PubKeyBuf),
        &PubKeyBufLen
        ) != 1)
  {
    return FALSE;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  // PubKeyBuf is 0x04 || X || Y; skip the leading uncompressed-point prefix
  if (PubKeyBufLen != 1 + HalfSize * 2) {
    return FALSE;
    // MU_CHANGE [END]
  }

  // MU_CHANGE - ASSERT ((UINTN)XSize <= HalfSize && (UINTN)YSize <= HalfSize);
  if (PublicKey != NULL) {
    ZeroMem (PublicKey, *PublicKeySize);
    CopyMem (PublicKey, PubKeyBuf + 1, HalfSize * 2);  // MU_CHANGE
  }

  return TRUE;  // MU_CHANGE
}

/**
  Computes exchanged common key.
  Given peer's public key (X, Y), this function computes the exchanged common key,
  based on its own context including value of curve parameter and random secret.
  X is the first half of PeerPublic with size being PeerPublicSize / 2,
  Y is the second half of PeerPublic with size being PeerPublicSize / 2.
  If public key is compressed, the PeerPublic will only contain half key (X).
  If EcContext is NULL, then return FALSE.
  If PeerPublic is NULL, then return FALSE.
  If PeerPublicSize is 0, then return FALSE.
  If Key is NULL, then return FALSE.
  If KeySize is not large enough, then return FALSE.
  For P-256, the PeerPublicSize is 64. First 32-byte is X, Second 32-byte is Y.
  For P-384, the PeerPublicSize is 96. First 48-byte is X, Second 48-byte is Y.
  For P-521, the PeerPublicSize is 132. First 66-byte is X, Second 66-byte is Y.
  @param[in, out]  EcContext          Pointer to the EC context.
  @param[in]       PeerPublic         Pointer to the peer's public X,Y.
  @param[in]       PeerPublicSize     Size of peer's public X,Y in bytes.
  @param[in]       CompressFlag       Flag of PeerPublic is compressed or not.
  @param[out]      Key                Pointer to the buffer to receive generated key.
  @param[in, out]  KeySize            On input, the size of Key buffer in bytes.
                                      On output, the size of data returned in Key buffer in bytes.
  @retval TRUE   EC exchanged key generation succeeded.
  @retval FALSE  EC exchanged key generation failed.
  @retval FALSE  KeySize is not large enough.
**/
BOOLEAN
EFIAPI
EcDhComputeKey (
  IN OUT  VOID         *EcContext,
  IN      CONST UINT8  *PeerPublic,
  IN      UINTN        PeerPublicSize,
  IN      CONST INT32  *CompressFlag,
  OUT     UINT8        *Key,
  IN OUT  UINTN        *KeySize
  )
{
  // MU_CHANGE [BEGIN]
  EC_PKEY_CTX  *EcPkeyCtx;
  CONST CHAR8  *CurveName;
  // MU_CHANGE [END]
  UINTN  HalfSize;
  // MU_CHANGE [BEGIN]
  BOOLEAN         RetVal;
  UINT8           PubKeyBuf[133];
  UINTN           PubKeyLen;
  OSSL_PARAM_BLD  *Bld;
  OSSL_PARAM      *Params;
  EVP_PKEY_CTX    *FromdataCtx;
  EVP_PKEY        *PeerPkey;
  EVP_PKEY_CTX    *DeriveCtx;
  UINTN           DerivedLen;

  // MU_CHANGE [END]

  if ((EcContext == NULL) || (PeerPublic == NULL) || (KeySize == NULL)) {
    return FALSE;
  }

  if ((Key == NULL) && (*KeySize != 0)) {
    return FALSE;
  }

  if (PeerPublicSize > INT_MAX) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx = (EC_PKEY_CTX *)EcContext;
  HalfSize  = GetHalfSizeFromNid (EcPkeyCtx->Nid);
  if (HalfSize == 0) {
    return FALSE;
  }

  // MU_CHANGE [END]
  if ((CompressFlag == NULL) && (PeerPublicSize != HalfSize * 2)) {
    return FALSE;
  }

  if ((CompressFlag != NULL) && (PeerPublicSize != HalfSize)) {
    return FALSE;
  }

  if (*KeySize < HalfSize) {
    *KeySize = HalfSize;
    return FALSE;
  }

  *KeySize = HalfSize;

  // MU_CHANGE [BEGIN]
  if (EcPkeyCtx->Pkey == NULL) {
    return FALSE;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  CurveName = OpenSslNidToCurveName (EcPkeyCtx->Nid);
  if (CurveName == NULL) {
    return FALSE;
  }

  // Build uncompressed or compressed peer public key buffer
  // MU_CHANGE [END]
  if (CompressFlag == NULL) {
    // MU_CHANGE [BEGIN]
    PubKeyBuf[0] = 0x04;
    CopyMem (PubKeyBuf + 1, PeerPublic, HalfSize * 2);
    PubKeyLen = 1 + HalfSize * 2;
    // MU_CHANGE [END]
  } else {
    // MU_CHANGE [BEGIN]
    PubKeyBuf[0] = (UINT8)((*CompressFlag & 1) ? 0x03 : 0x02);
    CopyMem (PubKeyBuf + 1, PeerPublic, HalfSize);
    PubKeyLen = 1 + HalfSize;
  }

  RetVal      = FALSE;
  Bld         = NULL;
  Params      = NULL;
  FromdataCtx = NULL;
  PeerPkey    = NULL;
  DeriveCtx   = NULL;

  Bld = OSSL_PARAM_BLD_new ();
  if (Bld == NULL) {
    goto fail;
  }

  if (OSSL_PARAM_BLD_push_utf8_string (Bld, OSSL_PKEY_PARAM_GROUP_NAME, CurveName, 0) != 1) {
    goto fail;
  }

  if (OSSL_PARAM_BLD_push_octet_string (Bld, OSSL_PKEY_PARAM_PUB_KEY, PubKeyBuf, PubKeyLen) != 1) {
    goto fail;
  }

  Params = OSSL_PARAM_BLD_to_param (Bld);
  if (Params == NULL) {
    goto fail;
    // MU_CHANGE [END]
  }

  // MU_CHANGE [BEGIN]
  FromdataCtx = EVP_PKEY_CTX_new_from_name (NULL, "EC", NULL);
  if (FromdataCtx == NULL) {
    // MU_CHANGE [END]
    goto fail;
  }

  if (EVP_PKEY_fromdata_init (FromdataCtx) != 1) {
    // MU_CHANGE
    goto fail;
  }

  if (EVP_PKEY_fromdata (FromdataCtx, &PeerPkey, EVP_PKEY_PUBLIC_KEY, Params) != 1) {
    // MU_CHANGE
    goto fail;
  }

  // MU_CHANGE [BEGIN]
  DeriveCtx = EVP_PKEY_CTX_new (EcPkeyCtx->Pkey, NULL);
  if (DeriveCtx == NULL) {
    goto fail;
  }

  if (EVP_PKEY_derive_init (DeriveCtx) != 1) {
    goto fail;
  }

  // EVP_PKEY_derive_set_peer also validates the peer key
  if (EVP_PKEY_derive_set_peer (DeriveCtx, PeerPkey) != 1) {
    goto fail;
  }

  DerivedLen = *KeySize;
  // Use <= 0 (not != 1) to match OpenSSL convention: 1 = success, 0 or negative = error
  if (EVP_PKEY_derive (DeriveCtx, Key, &DerivedLen) <= 0) {
    // MU_CHANGE [END]
    goto fail;
  }

  RetVal = TRUE;

fail:
  // MU_CHANGE [BEGIN]
  OSSL_PARAM_BLD_free (Bld);
  OSSL_PARAM_free (Params);
  EVP_PKEY_CTX_free (FromdataCtx);
  EVP_PKEY_free (PeerPkey);
  EVP_PKEY_CTX_free (DeriveCtx);
  // MU_CHANGE [END]
  return RetVal;
}

/**
  Carries out the EC-DSA signature.

  This function carries out the EC-DSA signature.
  If the Signature buffer is too small to hold the contents of signature, FALSE
  is returned and SigSize is set to the required buffer size to obtain the signature.

  If EcContext is NULL, then return FALSE.
  If MessageHash is NULL, then return FALSE.
  If HashSize need match the HashNid. HashNid could be SHA256, SHA384, SHA512, SHA3_256, SHA3_384, SHA3_512.
  If SigSize is large enough but Signature is NULL, then return FALSE.

  For P-256, the SigSize is 64. First 32-byte is R, Second 32-byte is S.
  For P-384, the SigSize is 96. First 48-byte is R, Second 48-byte is S.
  For P-521, the SigSize is 132. First 66-byte is R, Second 66-byte is S.

  @param[in]       EcContext    Pointer to EC context for signature generation.
  @param[in]       HashNid      hash NID
  @param[in]       MessageHash  Pointer to octet message hash to be signed.
  @param[in]       HashSize     Size of the message hash in bytes.
  @param[out]      Signature    Pointer to buffer to receive EC-DSA signature.
  @param[in, out]  SigSize      On input, the size of Signature buffer in bytes.
                                On output, the size of data returned in Signature buffer in bytes.

  @retval  TRUE   Signature successfully generated in EC-DSA.
  @retval  FALSE  Signature generation failed.
  @retval  FALSE  SigSize is too small.

**/
BOOLEAN
EFIAPI
EcDsaSign (
  IN      VOID         *EcContext,
  IN      UINTN        HashNid,
  IN      CONST UINT8  *MessageHash,
  IN      UINTN        HashSize,
  OUT     UINT8        *Signature,
  IN OUT  UINTN        *SigSize
  )
{
  // MU_CHANGE [BEGIN]
  EC_PKEY_CTX   *EcPkeyCtx;
  UINTN         HalfSize;
  EVP_PKEY_CTX  *PkeyCtx;
  ECDSA_SIG     *EcDsaSig;
  UINT8         DerSig[150];
  UINTN         DerSigLen;
  CONST UINT8   *DerSigPtr;
  BIGNUM        *R;
  BIGNUM        *S;
  INTN          RSize;
  INTN          SSize;

  // MU_CHANGE [END]

  if ((EcContext == NULL) || (MessageHash == NULL)) {
    return FALSE;
  }

  if (Signature == NULL) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx = (EC_PKEY_CTX *)EcContext;
  HalfSize  = GetHalfSizeFromNid (EcPkeyCtx->Nid);
  if (HalfSize == 0) {
    return FALSE;
    // MU_CHANGE [END]
  }

  if (*SigSize < (UINTN)(HalfSize * 2)) {
    *SigSize = HalfSize * 2;
    return FALSE;
  }

  *SigSize = HalfSize * 2;
  ZeroMem (Signature, *SigSize);

  switch (HashNid) {
    case CRYPTO_NID_SHA256:
      if (HashSize != SHA256_DIGEST_SIZE) {
        return FALSE;
      }

      break;

    case CRYPTO_NID_SHA384:
      if (HashSize != SHA384_DIGEST_SIZE) {
        return FALSE;
      }

      break;

    case CRYPTO_NID_SHA512:
      if (HashSize != SHA512_DIGEST_SIZE) {
        return FALSE;
      }

      break;

    default:
      return FALSE;
  }

  // MU_CHANGE [BEGIN]
  if (EcPkeyCtx->Pkey == NULL) {
    return FALSE;
  }

  PkeyCtx = EVP_PKEY_CTX_new (EcPkeyCtx->Pkey, NULL);
  if (PkeyCtx == NULL) {
    return FALSE;
  }

  if (EVP_PKEY_sign_init (PkeyCtx) != 1) {
    EVP_PKEY_CTX_free (PkeyCtx);
    return FALSE;
  }

  DerSigLen = sizeof (DerSig);
  if (EVP_PKEY_sign (PkeyCtx, DerSig, &DerSigLen, MessageHash, HashSize) != 1) {
    EVP_PKEY_CTX_free (PkeyCtx);
    return FALSE;
  }

  EVP_PKEY_CTX_free (PkeyCtx);

  // Parse DER-encoded ECDSA signature into R and S bignums
  DerSigPtr = DerSig;
  EcDsaSig  = d2i_ECDSA_SIG (NULL, &DerSigPtr, (long)DerSigLen);
  // MU_CHANGE [END]
  if (EcDsaSig == NULL) {
    return FALSE;
  }

  ECDSA_SIG_get0 (EcDsaSig, (CONST BIGNUM **)&R, (CONST BIGNUM **)&S);

  RSize = BN_num_bytes (R);
  SSize = BN_num_bytes (S);
  if ((RSize <= 0) || (SSize <= 0)) {
    ECDSA_SIG_free (EcDsaSig);
    return FALSE;
  }

  ASSERT ((UINTN)RSize <= HalfSize && (UINTN)SSize <= HalfSize);

  BN_bn2bin (R, &Signature[0 + HalfSize - RSize]);
  BN_bn2bin (S, &Signature[HalfSize + HalfSize - SSize]);

  ECDSA_SIG_free (EcDsaSig);

  return TRUE;
}

/**
  Verifies the EC-DSA signature.

  If EcContext is NULL, then return FALSE.
  If MessageHash is NULL, then return FALSE.
  If Signature is NULL, then return FALSE.
  If HashSize need match the HashNid. HashNid could be SHA256, SHA384, SHA512, SHA3_256, SHA3_384, SHA3_512.

  For P-256, the SigSize is 64. First 32-byte is R, Second 32-byte is S.
  For P-384, the SigSize is 96. First 48-byte is R, Second 48-byte is S.
  For P-521, the SigSize is 132. First 66-byte is R, Second 66-byte is S.

  @param[in]  EcContext    Pointer to EC context for signature verification.
  @param[in]  HashNid      hash NID
  @param[in]  MessageHash  Pointer to octet message hash to be checked.
  @param[in]  HashSize     Size of the message hash in bytes.
  @param[in]  Signature    Pointer to EC-DSA signature to be verified.
  @param[in]  SigSize      Size of signature in bytes.

  @retval  TRUE   Valid signature encoded in EC-DSA.
  @retval  FALSE  Invalid signature or invalid EC context.

**/
BOOLEAN
EFIAPI
EcDsaVerify (
  IN  VOID         *EcContext,
  IN  UINTN        HashNid,
  IN  CONST UINT8  *MessageHash,
  IN  UINTN        HashSize,
  IN  CONST UINT8  *Signature,
  IN  UINTN        SigSize
  )
{
  // MU_CHANGE [BEGIN]
  INT32         Result;
  EC_PKEY_CTX   *EcPkeyCtx;
  ECDSA_SIG     *EcDsaSig;
  UINTN         HalfSize;
  BIGNUM        *R;
  BIGNUM        *S;
  UINT8         DerSig[150];
  UINT8         *DerSigPtr;
  INT32         DerSigLen;
  EVP_PKEY_CTX  *PkeyCtx;

  // MU_CHANGE [END]

  if ((EcContext == NULL) || (MessageHash == NULL) || (Signature == NULL)) {
    return FALSE;
  }

  if ((SigSize > INT_MAX) || (SigSize == 0)) {
    return FALSE;
  }

  // MU_CHANGE [BEGIN]
  EcPkeyCtx = (EC_PKEY_CTX *)EcContext;
  HalfSize  = GetHalfSizeFromNid (EcPkeyCtx->Nid);
  if (HalfSize == 0) {
    return FALSE;
    // MU_CHANGE [END]
  }

  if (SigSize != (UINTN)(HalfSize * 2)) {
    return FALSE;
  }

  switch (HashNid) {
    case CRYPTO_NID_SHA256:
      if (HashSize != SHA256_DIGEST_SIZE) {
        return FALSE;
      }

      break;

    case CRYPTO_NID_SHA384:
      if (HashSize != SHA384_DIGEST_SIZE) {
        return FALSE;
      }

      break;

    case CRYPTO_NID_SHA512:
      if (HashSize != SHA512_DIGEST_SIZE) {
        return FALSE;
      }

      break;

    default:
      return FALSE;
  }

  // Build ECDSA_SIG from raw R || S, then DER-encode for EVP_PKEY_verify  // MU_CHANGE
  EcDsaSig = ECDSA_SIG_new ();
  if (EcDsaSig == NULL) {
    // MU_CHANGE - ECDSA_SIG_free (EcDsaSig);
    return FALSE;
  }

  R = BN_bin2bn (Signature, (UINT32)HalfSize, NULL);
  S = BN_bin2bn (Signature + HalfSize, (UINT32)HalfSize, NULL);
  if ((R == NULL) || (S == NULL)) {
    // MU_CHANGE [BEGIN]
    BN_free (R);
    BN_free (S);
    // MU_CHANGE [END]
    ECDSA_SIG_free (EcDsaSig);
    return FALSE;
  }

  // ECDSA_SIG_set0 takes ownership of R and S  // MU_CHANGE
  ECDSA_SIG_set0 (EcDsaSig, R, S);

  // MU_CHANGE [BEGIN]
  DerSigPtr = DerSig;
  DerSigLen = i2d_ECDSA_SIG (EcDsaSig, &DerSigPtr);
  ECDSA_SIG_free (EcDsaSig);
  if (DerSigLen <= 0) {
    return FALSE;
  }

  if (EcPkeyCtx->Pkey == NULL) {
    return FALSE;
  }

  PkeyCtx = EVP_PKEY_CTX_new (EcPkeyCtx->Pkey, NULL);
  if (PkeyCtx == NULL) {
    return FALSE;
  }

  if (EVP_PKEY_verify_init (PkeyCtx) != 1) {
    EVP_PKEY_CTX_free (PkeyCtx);
    return FALSE;
  }

  Result = EVP_PKEY_verify (PkeyCtx, DerSig, (UINTN)DerSigLen, MessageHash, HashSize);

  EVP_PKEY_CTX_free (PkeyCtx);
  // MU_CHANGE [END]

  return (Result == 1);
}
