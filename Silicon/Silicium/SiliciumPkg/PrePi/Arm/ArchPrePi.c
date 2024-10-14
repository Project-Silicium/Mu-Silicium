/**
  Copyright (c) 2011-2017, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PrePi.h"

VOID
ArchInitialize ()
{
  // Enable Floating Point
  if (FixedPcdGet32 (PcdVFPEnabled)) {
    ArmEnableVFP ();
  }
}