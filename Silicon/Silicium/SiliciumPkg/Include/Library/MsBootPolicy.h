/**
  Header file for Ms Boot Policy Library

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BOOT_POLICY_H_
#define _MS_BOOT_POLICY_H_

/**
  Ask if the Platform is Requesting Volume Up Application.

  @retval TRUE            - The System is Requesting Volume Up Application.
  @retval FALSE           - The System is not Requesting Volume Up Application.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsVolumeUp ();

/**
  Ask if the Platform is Requesting Volume Down Application.

  @retval TRUE            - The System is Requesting Volume Down Application.
  @retval FALSE           - The System is not Requesting Volume Down Application.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibIsVolumeDown ();

#endif /* _MS_BOOT_POLICY_H_ */
