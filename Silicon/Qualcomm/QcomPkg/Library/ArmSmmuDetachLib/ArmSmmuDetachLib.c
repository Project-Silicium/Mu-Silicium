/**
  Portion of this Code is Written by Referencing this:
  linux/drivers/iommu/arm/arm-smmu/arm-smmu.c

  Copyright (c) 2024-2025. Project Aloha Authors. All rights reserved.
  Copyright (c) 2022-2025. DuoWoA Authors.
  Copyright (c) 2023-2026. Project Silicium Authors.

  SPDX-License-Identifier: MIT
*/

#include <Library/ArmSmmuDetachLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>

#include "SmmuV2.h"

//
// Global Variables
//
STATIC ARM_SMMU_V2_DEVICE SmmuDevice;

VOID
ArmSmmuDisableContextBank (IN UINT32 ContextBankIndex)
{
  UINT32 FaultStatus;

  // Clear Page Table Base Addresses
  ArmSmmuCbWrite64 (&SmmuDevice, ContextBankIndex, ARM_SMMU_CB_TTBR0, 0);
  ArmSmmuCbWrite64 (&SmmuDevice, ContextBankIndex, ARM_SMMU_CB_TTBR1, 0);

  // Clear CB_TCR
  ArmSmmuCbWrite (&SmmuDevice, ContextBankIndex, ARM_SMMU_CB_TCR, 0);

  // Get Fault Status
  FaultStatus = ArmSmmuCbRead (&SmmuDevice, ContextBankIndex, ARM_SMMU_CB_FSR);

  // Clear Fault Status
  if (FaultStatus) {
    ArmSmmuCbWrite (&SmmuDevice, ContextBankIndex, ARM_SMMU_CB_FSR, FaultStatus);
  }
}

VOID
ArmSmmuFreeContextBank (IN UINT32 ContextBankIndex)
{
  UINT32 Cbar;
  UINT32 CbarType;
  UINT32 CbarVmid;

  // Disable Context Bank
  ArmSmmuDisableContextBank (ContextBankIndex);

  // Get GR1_CBAR
  Cbar = ArmSmmuGr1Read (&SmmuDevice, ARM_SMMU_GR1_CBAR (ContextBankIndex));

  // Set CBAR_TYPE
  CbarType  = (Cbar >> CBAR_TYPE_SHIFT);
  CbarType &= CBAR_TYPE_MASK;

  // Set CBAR_VMID
  CbarVmid  = (Cbar >> CBAR_VMID_SHIFT);
  CbarVmid &= CBAR_VMID_MASK;

  // Verify CBAR_TYPE & CBAR_VMID
  if (CbarType != CBAR_TYPE_S2_TRANS && CbarType != CBAR_TYPE_S1_TRANS_S2_FAULT && (CbarType != CBAR_TYPE_S1_TRANS_S2_BYPASS || CbarVmid != 0xFF)) {
    // Set CBAR
    Cbar  = ((CBAR_TYPE_S1_TRANS_S2_BYPASS & CBAR_TYPE_MASK) << CBAR_TYPE_SHIFT);
    Cbar |= ((0xFF & CBAR_VMID_MASK) << CBAR_VMID_SHIFT);

    // Write GR1_CBAR
    ArmSmmuGr1Write (&SmmuDevice, ARM_SMMU_GR1_CBAR (ContextBankIndex), Cbar);
  }
}

BOOLEAN
ArmSmmuSkipStream (
  IN UINT32        StreamMask,
  IN UINT32        MaskedStreamId,
  IN CONST UINT16 *SkipStreams,
  IN UINT8         SkipStreamCount)
{
  // Verify Skip Strams
  if (SkipStreams == NULL) {
    return FALSE;
  }

  // Loop thru all Skip Strams
  for (UINTN i = 0; i < SkipStreamCount; i++) {
    // Verify Stream ID
    if ((SkipStreams[i] & ~StreamMask) == MaskedStreamId) {
      return TRUE;
    }
  }

  return FALSE;
}

VOID
ArmSmmuDetachAll (
  IN CONST UINT16 *SkipStreams,
  IN UINT8         SkipStreamCount)
{
  UINT32 ContextBank[16];
  UINT32 NumContextBanks = 0;

  // Get GR0_ID0
  UINT32 Id0 = ArmSmmuGr0Read (&SmmuDevice, ARM_SMMU_GR0_ID0);

  // Get the Number of SMRGs
  UINT32 NumSmrg = (Id0 >> 0) & 0xFF;

  // Go thru each SMRG
  for (UINT32 i = 0; i < NumSmrg; i++) {
    UINT32 ContextBankIndex;
    UINT32 MaskedSmrId;
    UINT32 SmrMask;
    UINT32 SmrId;
    UINT32 S2cr;
    UINT32 Smr;

    // Get GR0_SMR
    Smr = ArmSmmuGr0Read (&SmmuDevice, ARM_SMMU_GR0_SMR (i));

    // Verify SMR
    if (!(Smr & SMR_VALID)) {
      continue;
    }

    // Get GR0_S2CR
    S2cr = ArmSmmuGr0Read (&SmmuDevice, ARM_SMMU_GR0_S2CR (i));

    // Extract Context Bank index and type
    ContextBankIndex = (S2cr >> S2CR_CBNDX_SHIFT) & S2CR_CBNDX_MASK;

    // Extract Stream ID and Mask
    SmrId   = (Smr >> SMR_ID_SHIFT) & SMR_ID_MASK;
    SmrMask = (Smr >> SMR_MASK_SHIFT) & SMR_ID_MASK;

    // Set Masked SMR ID
    MaskedSmrId = SmrId & ~SmrMask;

    // Verify Stream
    if (ArmSmmuSkipStream (SmrMask, MaskedSmrId, SkipStreams, SkipStreamCount)) {
      BOOLEAN AlreadyTracked = FALSE;

      // Go thru each Contect Bank
      for (UINT32 i = 0; i < NumContextBanks; i++) {
        // Compare Context Bank Index
        if (ContextBank[i] == ContextBankIndex) {
          AlreadyTracked = TRUE;
          break;
        }
      }

      // ?
      if (!AlreadyTracked && NumContextBanks < 16) {
        ContextBank[NumContextBanks++] = ContextBankIndex;
      }

      // Skip this Context Bank
      continue;
    }

    // Set FAULT Mode
    S2cr  = ((S2CR_TYPE_FAULT & S2CR_TYPE_MASK) << S2CR_TYPE_SHIFT);
    S2cr |= ((ContextBankIndex & S2CR_CBNDX_MASK) << S2CR_CBNDX_SHIFT);

    // Write GR0_S2CR
    ArmSmmuGr0Write (&SmmuDevice, ARM_SMMU_GR0_S2CR (i), S2cr);

    // Invalidate SMR Entry
    Smr &= ~SMR_VALID;
  
    // Write GR0_SMR
    ArmSmmuGr0Write (&SmmuDevice, ARM_SMMU_GR0_SMR (i), Smr);
  }

  // Invalidate TLB
  ArmInvalidateTlb ();

  // Go thru all Context Banks
  for (UINT32 i = 0; i < SmmuDevice.NumContextBanks; i++) {
    BOOLEAN AlreadyTracked = FALSE;

    // Go thru each Context Bank
    for (UINT32 j = 0; j < NumContextBanks; j++) {
      // Compare Context Bank ID
      if (ContextBank[j] == i) {
        AlreadyTracked = TRUE;
        break;
      }
    }

    // Skip Context Bank
    if (AlreadyTracked) {
      continue;
    }

    // Free Context Bank
    ArmSmmuFreeContextBank (i);
  }
}

VOID
ArmSmmuDetach (
  IN CONST UINT16 *SkipStreams,
  IN UINT8         SkipStreamCount)
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX SmmuRegion;

  // Locate "SMMU" Memory Region
  Status = LocateMemoryMapAreaByName ("SMMU", &SmmuRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Clear SMMU Device Variable
  ZeroMem ((VOID *)&SmmuDevice, sizeof (SmmuDevice));

  // Set SMMU Base Address
  SmmuDevice.BaseAddress = SmmuRegion.Address;

  // Get GR0_ID1
  UINT32 Id1 = ArmSmmuGr0Read (&SmmuDevice, ARM_SMMU_GR0_ID1);

  // Set Remaining SMMU Data
  SmmuDevice.PageShift       = (Id1 & ARM_SMMU_ID1_PAGESIZE) ? 16 : 12;
  SmmuDevice.NumPage         = 1 << ((READ_FIELD (ARM_SMMU_ID1_NUMPAGENDXB, Id1)) + 1);
  SmmuDevice.NumContextBanks = Id1 & ARM_SMMU_ID1_NUMCB_MASK;

  // Detach IOMMU Domains
  ArmSmmuDetachAll (SkipStreams, SkipStreamCount);
}
