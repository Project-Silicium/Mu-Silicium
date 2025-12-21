/**
  Copyright (c) 2023 - 2024, Arm Limited. All rights reserved.
  Copyright (c) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _RNG_H_
#define _RNG_H_

//
// Secure RNG Algorithm Array
//
typedef struct {
  EFI_GUID    *Guid;
  CONST CHAR8 *Name;
  BOOLEAN      Available;
} SECURE_RNG_ALGO_ARRAY;

//
// Secure Hash Algorithms
//
STATIC SECURE_RNG_ALGO_ARRAY mSecureHashAlgorithms[] = {
  {
    &gEfiRngAlgorithmArmRndr,
    "ARM-RNDR",
    FALSE
  },
  {
    &gEfiRngAlgorithmSp80090Ctr256Guid,
    "DRBG-CTR",
    FALSE
  },
  {
    &gEfiRngAlgorithmSp80090Hmac256Guid,
    "DRBG-HMAC",
    FALSE
  },
  {
    &gEfiRngAlgorithmSp80090Hash256Guid,
    "DRBG-Hash",
    FALSE
  },
  {
    &gEfiRngAlgorithmRaw,
    "TRNG",
    FALSE
  }
};

#endif /* _RNG_H_ */
