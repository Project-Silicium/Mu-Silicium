/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PlatformSecLib.h>

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  // Mpidr, MailboxSetAddress, MailboxGetAddress, MailboxClearAddress, MailboxClearValue

  // Cluster 0
  { 0x000, 0, 0, 0, 0xFFFFFFFF },
  { 0x001, 0, 0, 0, 0xFFFFFFFF },
  { 0x002, 0, 0, 0, 0xFFFFFFFF },
  { 0x003, 0, 0, 0, 0xFFFFFFFF },

  // Cluster 1
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x101, 0, 0, 0, 0xFFFFFFFF },
  { 0x102, 0, 0, 0, 0xFFFFFFFF },
  { 0x103, 0, 0, 0, 0xFFFFFFFF }
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
  // Nothing to do Here.
}
