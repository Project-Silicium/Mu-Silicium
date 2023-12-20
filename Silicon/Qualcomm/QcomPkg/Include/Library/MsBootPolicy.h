/** @file
  Header file for Ms Boot Policy Library

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BOOT_POLICY_H_
#define _MS_BOOT_POLICY_H_

/**
  Ask if the platform is requesting Slot Switch

  @retval TRUE     System is requesting Slot Switch
  @retval FALSE    System is not requesting Slot Switch.
**/
BOOLEAN
EFIAPI
MsBootPolicyLibSlotSwitch (VOID);

#endif /* _MS_BOOT_POLICY_H_ */