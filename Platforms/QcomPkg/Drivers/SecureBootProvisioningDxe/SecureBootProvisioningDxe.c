/** @file SecureBootProvisioningDxe.c
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Library/MuSecureBootKeySelectorLib.h>

EFI_STATUS
EFIAPI
SecureBootProvisioningDxeInitialize(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Microsoft Plus 3rd Party Plus Windows On Andromeda
  Status  = SetSecureBootConfig(0);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Failed to enroll SecureBoot config %r!\n", __FUNCTION__, Status));
    goto exit;
  }

exit:
  return Status;
}
