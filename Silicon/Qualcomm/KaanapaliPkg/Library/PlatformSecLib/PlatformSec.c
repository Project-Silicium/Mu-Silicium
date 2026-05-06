/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PlatformSecLib.h>
#include <Library/ArmSmmuDetachLib.h>

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  // Mpidr, MailboxSetAddress, MailboxGetAddress, MailboxClearAddress, MailboxClearValue

  // Cluster 0
  { 0x000, 0, 0, 0, 0xFFFFFFFF },
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x200, 0, 0, 0, 0xFFFFFFFF },
  { 0x300, 0, 0, 0, 0xFFFFFFFF },
  { 0x400, 0, 0, 0, 0xFFFFFFFF },
  { 0x500, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 1
  { 0x10000, 0, 0, 0, 0xFFFFFFFF },
  { 0x10100, 0, 0, 0, 0xFFFFFFFF }
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
PlatformInitialize ()
{
  // Set MDP SIDs
  CONST UINT16 MdpStreams[] = { 0x800, 0x801, 0x804, 0x81C, 0x19A1, 0x19A2, 0x19A3, 0x19A4, 0x19A5, 0x19AB, 0x2801, 0x2C01 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
