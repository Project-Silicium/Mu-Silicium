/**
  Copyright (c) 2011-2013, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_SEC_LIB_H_
#define _PLATFORM_SEC_LIB_H_

#include <Ppi/ArmMpCoreInfo.h>

VOID
PlatformAssemblyInitialize ();

VOID
PlatformInitialize ();

VOID
GetPlatformCoreTable (
  OUT ARM_CORE_INFO **ArmCoreTable,
  OUT UINTN          *CoreCount);

#endif /* _PLATFORM_SEC_LIB_H_ */
