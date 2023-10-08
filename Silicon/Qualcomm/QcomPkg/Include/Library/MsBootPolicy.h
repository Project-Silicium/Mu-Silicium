/** @file
 *Header file for Ms Boot Policy Library

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MS_BOOT_POLICY_H_
#define _MS_BOOT_POLICY_H_

/**
 *Ask if the platform is requesting UEFI Shell

 *@retval TRUE     System is requesting UEFI Shell
 *@retval FALSE    System is not requesting UEFI Shell.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibUEFIShell (
  VOID
  );

#endif /* _MS_BOOT_POLICY_H_ */
