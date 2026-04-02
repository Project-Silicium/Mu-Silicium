/**
  Copyright (c) 2011-2013, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_SEC_LIB_H_
#define _PLATFORM_SEC_LIB_H_

#include <Ppi/ArmMpCoreInfo.h>

/**
  This Function Executes Platform Specific Assembly Code.
**/
VOID
PlatformAssemblyInitialize ();

/**
  This Function Executes Platform Specific C Code.
**/
VOID
PlatformInitialize ();

/**
  This Function returns the Core Table Data.

  @param[out] ArmCoreTable                 - The Core Table.
  @param[out] CoreCount                    - The Number of Entries in the Core Table.
**/
VOID
GetPlatformCoreTable (
  OUT ARM_CORE_INFO **ArmCoreTable,
  OUT UINTN          *CoreCount
  );

#endif /* _PLATFORM_SEC_LIB_H_ */
