/** @file Header file Ms Boot Options library

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SD_BOOT_OPTIONS_LIB_H_
#define _SD_BOOT_OPTIONS_LIB_H_

#include <Library/UefiBootManagerLib.h>

/**
  Return the boot option corresponding to the UFP Menu.

  @param BootOption    Return a created UFP Menu with the parameter passed
  @param Parameter     The parameter to add to the BootOption

  @retval EFI_SUCCESS   The UFP Menu is successfully returned.
  @retval Status        Return status of gRT->SetVariable (). BootOption still points
                        to the UFP Menu even the Status is not EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SdBootOptionsLibGetUFPMenu (
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption,
  IN     CHAR8                         *Parameter
  );

#endif // _SD_BOOT_OPTIONS_LIB_H_
