/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/MshcHostBridge.h>

#include "MshcExynos.h"

UINTN
MshcBoardGetHostCount (VOID)
{
  return 1;
}

EFI_STATUS
MshcBoardInit (
  IN     UINTN      Index,
  IN OUT MSHC_HOST *Host)
{
  EFI_MEMORY_REGION_DESCRIPTOR MemRegion;
  EFI_STATUS                   Status;

  if (Index != 0) {
    return EFI_NOT_FOUND;
  }

  Status = LocateMemoryRegionByName ("MSHC", &MemRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate the MSHC Memory Region! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Host->ControllerId  = Index;
  Host->BaseAddress   = MemRegion.Address;
  Host->CiuClockHz    = 400000000;
  Host->ClkSelDivider = 3;
  Host->ClkSelTiming  = MSHC_TIMING_SDR;
  Host->BusWidth      = 8;
  Host->Coherent = FALSE;
  Host->ClkSelTimingSdr50  = MSHC_TIMING_SDR;
  Host->ClkSelTimingSdr104 = MSHC_TIMING_SDR;

  return EFI_SUCCESS;
}

EFI_STATUS
MshcBoardSetCiuClock (
  IN OUT MSHC_HOST *Host,
  IN     UINTN      Frequency)
{
  (VOID)Frequency;

  Host->CiuClockHz = 400000000;

  return EFI_SUCCESS;
}

EFI_STATUS
MshcBoardSetVoltage (
  IN OUT MSHC_HOST *Host,
  IN     UINT32     Millivolts)
{
  (VOID)Host;
  (VOID)Millivolts;

  return EFI_UNSUPPORTED;
}

EFI_STATUS
MshcBoardPowerCycle (IN OUT MSHC_HOST *Host)
{
  (VOID)Host;

  return EFI_UNSUPPORTED;
}

VOID
MshcBoardPostReset (IN OUT MSHC_HOST *Host)
{
  (VOID)Host;
}

BOOLEAN
MshcBoardIsCardPresent (IN MSHC_HOST *Host)
{
  (VOID)Host;
  return TRUE;
}

BOOLEAN
MshcBoardIsReadOnly (IN MSHC_HOST *Host)
{
  (VOID)Host;
  return FALSE;
}
