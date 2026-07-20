/**
  Copyright (c) 2021, NUVIA Inc. All rights reserved.
  Copyright (c) 2021 - 2022, Ampere Computing LLC. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ArmLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <IndustryStandard/ArmCache.h>

#include "SmBiosTable.h"

//
// External Functions
//
extern UINTN ArmReadIdAA64Pfr1 ();

//
// SMBIOS Cache Associativity Types
//
STATIC
CONST
SMBIOS_CACHE_ASSOCIATIVITY_TYPE
SmbiosAssociativityTypes[] = {
  { 1,  CacheAssociativityDirectMapped },
  { 2,  CacheAssociativity2Way         },
  { 4,  CacheAssociativity4Way         },
  { 8,  CacheAssociativity8Way         },
  { 12, CacheAssociativity12Way        },
  { 16, CacheAssociativity16Way        },
  { 20, CacheAssociativity20Way        },
  { 24, CacheAssociativity24Way        },
  { 32, CacheAssociativity32Way        },
  { 48, CacheAssociativity48Way        },
  { 64, CacheAssociativity64Way        }
};

UINTN
GetMidr ()
{
  return ArmReadMidr ();
}

BOOLEAN
IsArmv9 ()
{
  // Extract SME Field
  UINT32 Sme = (ArmReadIdAA64Pfr1 () >> 24) & 0xF;

  return Sme >= 1 ? TRUE : FALSE;
}

UINT32
GetCacheSize (
  IN CCSIDR_DATA Ccsidr,
  IN BOOLEAN     CcidxSupported)
{
  // Get Cache Size
  if (CcidxSupported) {
    return ((1 << (Ccsidr.BitsCcidxAA64.LineSize      + 4)) *
            (      Ccsidr.BitsCcidxAA64.Associativity + 1)  *
            (      Ccsidr.BitsCcidxAA64.NumSets       + 1)) /
            1024;
  } else {
    return ((1 << (Ccsidr.BitsNonCcidx.LineSize      + 4)) * 
            (      Ccsidr.BitsNonCcidx.Associativity + 1)  *
            (      Ccsidr.BitsNonCcidx.NumSets       + 1)) /
            1024;
  }
}

CACHE_ASSOCIATIVITY_DATA
GetCacheAssociativity (
  IN CCSIDR_DATA Ccsidr,
  IN BOOLEAN     CcidxSupported)
{
  UINT32 Associativity;

  // Get Cache Associativity
  if (CcidxSupported) {
    Associativity = Ccsidr.BitsCcidxAA64.Associativity + 1;
  } else {
    Associativity = Ccsidr.BitsNonCcidx.Associativity + 1;
  }

  // Convert Cache Associativity
  for (UINT8 i = 0; i < ARRAY_SIZE (SmbiosAssociativityTypes); i++) {
    // Compare Associativity Ways
    if (SmbiosAssociativityTypes[i].Ways == Associativity) {
      return SmbiosAssociativityTypes[i].Type;
    }
  }

  return CacheAssociativityUnknown;
}

BOOLEAN
CacheLevelSupported (
  IN UINT8   Level,
  IN BOOLEAN InstructionCache)
{
  CLIDR_DATA Clidr;
  UINT8      Type;

  // Read CLIDR
  Clidr.Data = ReadCLIDR ();

  // Get Cache Type
  Type = CLIDR_GET_CACHE_TYPE (Clidr.Data, Level - 1);

  // Verify Cache Type
  if (Type == ClidrCacheTypeNone) {
    return FALSE;
  }

  // Check Cache Type
  if (InstructionCache) {
    return (Type == ClidrCacheTypeInstructionOnly || Type == ClidrCacheTypeSeparate || Type == ClidrCacheTypeUnified);
  } else {
    return (Type == ClidrCacheTypeDataOnly || Type == ClidrCacheTypeSeparate || Type == ClidrCacheTypeUnified);
  }
}

VOID
GetCpuCacheDetails (
  IN  UINT8                     Level,
  IN  BOOLEAN                   InstructionCache,
  OUT UINT32                   *Size,
  OUT CACHE_ASSOCIATIVITY_DATA *Associativity)
{
  CCSIDR_DATA Ccsidr;
  CSSELR_DATA Csselr;
  BOOLEAN     CcidxSupported;

  // Verify Cache Existense
  if (!CacheLevelSupported (Level, InstructionCache)) {
    // Pass Dummy Values
    *Size          = 0;
    *Associativity = CacheAssociativityUnknown;

    return;
  }

  // Set CSSELR Data
  Csselr.Data       = 0;
  Csselr.Bits.Level = Level - 1;
  Csselr.Bits.InD   = InstructionCache;

  // Read CCSIDR
  Ccsidr.Data = ReadCCSIDR (Csselr.Data);

  // Get CCIDX Support State
  CcidxSupported = ArmHasCcidx ();

  // Pass Data
  *Size          = GetCacheSize          (Ccsidr, CcidxSupported);
  *Associativity = GetCacheAssociativity (Ccsidr, CcidxSupported);
}
