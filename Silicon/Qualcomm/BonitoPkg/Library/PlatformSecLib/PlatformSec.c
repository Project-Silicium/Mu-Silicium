/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PlatformSecLib.h>
#include <Library/ArmSmmuDetachLib.h>

VOID
PlatformInitialize ()
{
  // Set MDP SIDs
  CONST UINT16 MdpStreams[] = { 0x800, 0x801, 0x804 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
