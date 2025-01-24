/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BOOT_OPTIONS_H_
#define _MS_BOOT_OPTIONS_H_

#include <Library/UefiBootManagerLib.h>

EFI_STATUS
BuildFwLoadOption (
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  EFI_GUID                     *FwFile,
  CHAR8                        *Parameter
  );

#endif /* _MS_BOOT_OPTIONS_H_ */
