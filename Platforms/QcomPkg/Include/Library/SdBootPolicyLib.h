/** @file
 *Header file for Ms Boot Policy Library

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SD_BOOT_POLICY_LIB_H_
#define _SD_BOOT_POLICY_LIB_H_

/**
 *Ask if the platform is requesting UFP Change

 *@retval TRUE     System is requesting UFP Change
 *@retval FALSE    System is not requesting Changes.
**/
BOOLEAN
EFIAPI
SdBootPolicyLibIsUFPBoot (
  VOID
  );

#endif
