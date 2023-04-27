/** @file
  Consumer module to locate conf data from variable storage, initialize
  the GFX policy data and override the policy based on configuration values.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef CONFIG_KNOBS_H_
#define CONFIG_KNOBS_H_

/**
  Helper function to apply GFX configuration data to GFX silicon policy.

  @param[in]      PolicyInterface   Pointer to current policy protocol/PPI interface.
  @param[in]      GfxConfigBuffer   Pointer to GFX configuration data.

  @retval EFI_SUCCESS           The configuration is translated to policy successfully.
  @retval EFI_INVALID_PARAMETER One or more of the required input pointers are NULL.
  @retval EFI_BUFFER_TOO_SMALL  Supplied GfxSiliconPolicy is too small to fit in translated data.
  @retval Others                Other errors occurred when getting GFX policy.
**/
EFI_STATUS
EFIAPI
ApplyGfxConfigToPolicy (
  IN  POLICY_PPI  *PolicyInterface,
  IN  VOID        *ConfigBuffer
  );

#endif // CONFIG_KNOBS_H_
