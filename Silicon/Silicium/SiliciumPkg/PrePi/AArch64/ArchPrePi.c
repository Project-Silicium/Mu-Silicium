/**
  Copyright (c) 2011-2017, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ArmLib.h>

VOID
ArchInitialize ()
{
  // Enable Floating Point
  if (FixedPcdGet32 (PcdVFPEnabled)) {
    ArmEnableVFP ();
  }

  // Get current EL Level
  if (ArmReadCurrentEL () == AARCH64_EL2) {
    // Trap General Exceptions
    ArmWriteHcr (ARM_HCR_TGE);

    // Enable Timer Access on EL1 and EL0
    ArmWriteCntHctl (CNTHCTL_EL2_EL1PCTEN | CNTHCTL_EL2_EL1PCEN);
  }
}
