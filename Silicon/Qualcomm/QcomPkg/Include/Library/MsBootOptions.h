/**
  Header file Ms Boot Options library

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BOOT_OPTIONS_H_
#define _MS_BOOT_OPTIONS_H_

#include <Library/UefiBootManagerLib.h>

/**
  Return the Boot Option Corresponding to the Slot Switch App.

  @param BootOption     - Return a Created Slot Switch App with the Parameter Passed.

  @retval Status        - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsBootOptionsLibSlotSwitchApp (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  );

#endif /* _MS_BOOT_OPTIONS_H_ */
