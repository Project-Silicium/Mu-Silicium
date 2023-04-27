/** @file
  Consumer module to locate conf data from variable storage, initialize
  the GFX policy data and override the policy based on configuration values.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <PolicyDataStructGFX.h>
#include <Protocol/Policy.h>

#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/Policy.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>

// Statically define policy initialization for 2 GFX ports
GFX_POLICY_DATA  DefaultQemuGfxPolicy[GFX_PORT_MAX_CNT] = {
  {
    .Power_State_Port = TRUE
  },
  {
    .Power_State_Port = TRUE
  }
};

/**
  Publish default silicon policy for GFX to policy database.

  @param FileHandle                     The image handle.
  @param PeiServices                    The PEI services table.

  @retval Status                        From internal routine or boot object, should not fail
**/
EFI_STATUS
EFIAPI
PublishDefaultPolicyGfxEntry (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS  Status;
  POLICY_PPI  *PolPpi = NULL;

  DEBUG ((DEBUG_INFO, "%a - Entry.\n", __FUNCTION__));

  // First locate policy ppi, should not fail as we populate the
  Status = PeiServicesLocatePpi (&gPeiPolicyPpiGuid, 0, NULL, (VOID *)&PolPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to locate Policy PPI - %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    goto Done;
  }

  // Publish GFX policy
  Status = PolPpi->SetPolicy (&gSbsaPolicyDataGFXGuid, 0, DefaultQemuGfxPolicy, sizeof (DefaultQemuGfxPolicy));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Failed to set GFX policy - %r\n", __FUNCTION__, Status));
  }

Done:
  return Status;
}
