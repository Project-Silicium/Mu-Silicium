/**
  Copyright (c), 2017, Andrey Warkentin <andrey.warkentin@gmail.com>
  Copyright (c), 2018, Bingxing Wang <uefi-oss-projects@imbushuo.net>
  Copyright (c), Microsoft Corporation. All rights reserved.
  Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2013 Linaro.org

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _TYPE_7_TABLE_H_
#define _TYPE_7_TABLE_H_

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1IC = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // CacheConfiguration
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                       // CacheSpeed
  CacheErrorParity,        // ErrorCorrectionType
  CacheTypeInstruction,    // SystemCacheType
  CacheAssociativity16Way, // Associativity
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  }
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L1DC = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0280, // CacheConfiguration
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                       // CacheSpeed
  CacheErrorParity,        // ErrorCorrectionType
  CacheTypeData,           // SystemCacheType
  CacheAssociativity16Way, // Associativity
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  }
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L2C = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0281, // CacheConfiguration
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // CacheSpeed
  CacheErrorParity,       // ErrorCorrectionType
  CacheTypeUnified,       // SystemCacheType
  CacheAssociativity8Way, // Associativity
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  }
};

SMBIOS_TABLE_TYPE7 mCacheInfoType7_L3C = {
  {EFI_SMBIOS_TYPE_CACHE_INFORMATION, sizeof(SMBIOS_TABLE_TYPE7), 0},
  1,      // SocketDesignation String
  0x0282, // CacheConfiguration
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  {
    0, // Other
    1, // Unknown
    0, // NonBurst
    0, // Burst
    0, // PipelineBurst
    0, // Synchronous
    0, // Asynchronous
    0  // Reserved
  },
  0,                      // CacheSpeed
  CacheErrorParity,       // ErrorCorrectionType
  CacheTypeUnified,       // SystemCacheType
  CacheAssociativity8Way, // Associativity
  {
    0, // Size
    0  // Granularity64K
  },
  {
    0, // Size
    0  // Granularity64K
  }
};

CHAR8 *mCacheInfoType7_L1ICStrings[] = {
  "Not Specified",
  NULL
};

CHAR8 *mCacheInfoType7_L1DCStrings[] = {
  "Not Specified",
  NULL
};

CHAR8 *mCacheInfoType7_L2CStrings[] = {
  "Not Specified",
  NULL
};

CHAR8 *mCacheInfoType7_L3CStrings[] = {
  "Not Specified",
  NULL
};

#endif /* _TYPE_7_TABLE_H_ */
