/**
  Header file for Ms Boot Policy Library

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BOOT_POLICY_H_
#define _MS_BOOT_POLICY_H_

/**
  Ask if the Device is Requesting Slot Switch.

  @retval TRUE     - System is Requesting Slot Switch.
  @retval FALSE    - System is not Requesting Slot Switch.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibSlotSwitch ();

#endif /* _MS_BOOT_POLICY_H_ */
