/** @file Header file Ms Boot Options library

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MS_BOOT_OPTIONS_H_
#define _MS_BOOT_OPTIONS_H_

#include <Library/UefiBootManagerLib.h>

/**
  Return the boot option corresponding to the Slot Switch App.

  @param BootOption     Return a created Slot Switch App with the parameter passed

  @retval EFI_SUCCESS   The Slot Switch App is successfully returned.
  @retval Status        Return status of gRT->SetVariable (). BootOption still points
                        to the Slot Switch App even the Status is not EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MsBootOptionsLibSlotSwitchApp (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  );

/**
  Return the boot option corresponding to the UEFI Shell.

  @param BootOption     Return a created Slot UEFI Shell with the parameter passed

  @retval EFI_SUCCESS   The Slot UEFI Shell is successfully returned.
  @retval Status        Return status of gRT->SetVariable (). BootOption still points
                        to the Slot UEFI Shell even the Status is not EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
MsBootOptionsLibUEFIShell (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  );

#endif /* _MS_BOOT_OPTIONS_H_ */
