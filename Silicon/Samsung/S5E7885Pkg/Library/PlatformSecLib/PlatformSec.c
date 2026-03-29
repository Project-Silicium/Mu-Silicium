/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PlatformRegisters.h"

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  // Mpidr, MailboxSetAddress, MailboxGetAddress, MailboxClearAddress, MailboxClearValue

  // Cluster 0
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x101, 0, 0, 0, 0xFFFFFFFF },
  { 0x102, 0, 0, 0, 0xFFFFFFFF },
  { 0x103, 0, 0, 0, 0xFFFFFFFF },
  { 0x200, 0, 0, 0, 0xFFFFFFFF },
  { 0x201, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 1
  { 0x000, 0, 0, 0, 0xFFFFFFFF },
  { 0x001, 0, 0, 0, 0xFFFFFFFF }
};

VOID
GetPlatformCoreTable (
  OUT ARM_CORE_INFO **ArmCoreTable,
  OUT UINTN          *CoreCount)
{
  // Pass Data
  *ArmCoreTable = mArmPlatformMpCoreInfoTable;
  *CoreCount    = ARRAY_SIZE (mArmPlatformMpCoreInfoTable);
}

VOID
EnableFrameBufferWrites ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR DeconRegion;

  // Locate "Decon" Memory Region
  Status = LocateMemoryRegionByName ("Decon", &DeconRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Configure Decon
  MmioWrite32 (DeconRegion.Address + HW_SW_TRIG_CONTROL, 0x1281);
}

VOID
PlatformInitialize ()
{
  // Enable Frame Buffer Writes
  EnableFrameBufferWrites ();
}
