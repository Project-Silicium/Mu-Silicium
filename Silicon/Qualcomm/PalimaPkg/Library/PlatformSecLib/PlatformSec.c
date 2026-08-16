/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/ConfigurationMapHelperLib.h>
#include <Library/ArmSmmuDetachLib.h>

#include "PlatformRegisters.h"

VOID
WakeUpCores ()
{
  EFI_STATUS Status;
  UINT32     EarlyInitCoreCnt;

  // Get Early Cores Count
  Status = LocateConfigurationEntry32 ("EarlyInitCoreCnt", &EarlyInitCoreCnt);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Update GIC Wakers
  for (UINTN i = 0; i < EarlyInitCoreCnt; i++) {
    MmioAnd32 (GICR_WAKER_CPU (i), ~GIC_WAKER_PROCESSORSLEEP);
  }
}

VOID
PlatformInitialize ()
{
  // Wake Up Cores
  WakeUpCores ();

  // Set MDP SIDs
  CONST UINT16 MdpStreams[] = { 0x2800, 0x2801, 0x2803, 0x2804, 0x2C01 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
