/**
  Copyright (c) 2011-2013, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_SEC_LIB_H_
#define _PLATFORM_SEC_LIB_H_

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

#endif /* _PLATFORM_SEC_LIB_H_ */
