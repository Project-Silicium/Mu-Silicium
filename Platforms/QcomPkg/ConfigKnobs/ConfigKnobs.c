/** @file
  Consumer module to locate conf data from variable storage, initialize
  the GFX policy data and override the policy based on configuration values.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <PolicyDataStructGFX.h>

#include <Ppi/Policy.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <ConfigStdStructDefs.h>

// XML autogen definitions
#include <Generated/ConfigClientGenerated.h>
#include <Generated/ConfigServiceGenerated.h>

#include "ConfigKnobs.h"

/**
  Module entry point that will check configuration data and publish them to policy database.

  @param FileHandle                     The image handle.
  @param PeiServices                    The PEI services table.

  @retval Status                        From internal routine or boot object, should not fail
**/
EFI_STATUS
EFIAPI
ConfigKnobsEntry (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS  Status;
  POLICY_PPI  *PolPpi = NULL;

  BOOLEAN  GfxEnablePort0;

  DEBUG ((DEBUG_INFO, "%a - Entry.\n", __FUNCTION__));

  // First locate policy ppi, should not fail as we populate the
  Status = PeiServicesLocatePpi (&gPeiPolicyPpiGuid, 0, NULL, (VOID *)&PolPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to locate Policy PPI - %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Done;
  }

  Status = ConfigGetPowerOnPort0 (&GfxEnablePort0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to get PowerOnPort0 config knob! - %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Done;
  }

  Status = ApplyGfxConfigToPolicy (PolPpi, &GfxEnablePort0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to apply configuration data to the GFX silicon policy - %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Done;
  }

Done:

  return Status;
}
