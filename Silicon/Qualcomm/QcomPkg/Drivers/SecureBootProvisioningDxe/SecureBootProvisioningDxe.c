/** @file SecureBootProvisioningDxe.c

    This file contains functions for provisioning Secure Boot.

    Copyright (C) DuoWoA authors. All rights reserved.
    Copyright (C) Microsoft Corporation. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>

#include <Library/MuSecureBootKeySelectorLib.h>

EFI_STATUS
EFIAPI
SecureBootProvisioningDxeInitialize(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  return SetSecureBootConfig(0);
}
