/**
  Header file Ms Boot Options library

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BOOT_OPTIONS_H_
#define _MS_BOOT_OPTIONS_H_

#include <Library/UefiBootManagerLib.h>

/**
  This Function Builds a Firmware Load Option using the File and the Parameter.

  @param BootOption              - The Boot Option that gets Build.
  @param FwFile                  - The EFI Application that gets Loaded by the Boot Option.
  @param Parameter               - Parameter for the Boot Option Build.

  @return Status                 - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
BuildFwLoadOption (
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  EFI_GUID                     *FwFile,
  CHAR8                        *Parameter
  );

#endif /* _MS_BOOT_OPTIONS_H_ */
