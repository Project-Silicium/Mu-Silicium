/** @file
    Copyright (c) 2024-2025. Project Aloha Authors. All rights reserved.
    Copyright (c) 2022-2025. DuoWoA Authors.
    Copyright (c) 2023-2026. Project Silicium Authors.
    SPDX-License-Identifier: MIT

    Portion of this code is written by referencing
    linux/drivers/iommu/arm/arm-smmu/arm-smmu.c
*/

#include <Uefi.h>

#include <Library/ArmSmmuDetachLib.h>
#include "ArmSmmuDetachLibInternal.h"

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/TimerLib.h>

// ============================================================================
// Register Access Functions
// ============================================================================

static
UINT32
ArmSmmuGr0Read (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN Offset)
{
  return MmioRead32 (Smmu->BaseAddress + (ARM_SMMU_GR0 << Smmu->PageShift) + Offset);
}

static
VOID
ArmSmmuGr0Write (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN Offset,
  IN UINT32 Value)
{
  MmioWrite32 (Smmu->BaseAddress + (ARM_SMMU_GR0 << Smmu->PageShift) + Offset, Value);
  MemoryFence ();
}

static
UINT32
ArmSmmuGr1Read (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN Offset)
{
  return MmioRead32 (Smmu->BaseAddress + (ARM_SMMU_GR1 << Smmu->PageShift) + Offset);
}

static
VOID
ArmSmmuGr1Write (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN Offset,
  IN UINT32 Value)
{
  MmioWrite32 (Smmu->BaseAddress + (ARM_SMMU_GR1 << Smmu->PageShift) + Offset, Value);
  MemoryFence ();
}

static
UINT32
ArmSmmuCbRead(
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32 Bank,
  IN UINTN Offset)
{
  return MmioRead32 (Smmu->BaseAddress + (UINTN)(ARM_SMMU_CB(Smmu, Bank) << Smmu->PageShift) + Offset);
}

static
VOID
ArmSmmuCbWrite(
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32 Bank,
  IN UINTN Offset,
  IN UINT32 Value)
{
  MmioWrite32 (Smmu->BaseAddress + (UINTN)(ARM_SMMU_CB(Smmu, Bank) << Smmu->PageShift) + Offset, Value);
  MemoryFence ();
}

static
VOID
ArmSmmuCbWrite64(
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32 Bank,
  IN UINTN Offset,
  IN UINT64 Value)
{
  ArmSmmuCbWrite (Smmu, Bank, Offset, (UINT32)Value);
  ArmSmmuCbWrite (Smmu, Bank, Offset + 4, (UINT32)(Value >> 32));
}

// ============================================================================
// TLB Invalidation
// ============================================================================

static
VOID
ArmSmmuInvalidateAllTlb (
  IN ARM_SMMU_V2_DEVICE *Smmu)
{
  UINT32 Status;
  UINTN  Timeout;

  // Invalidate all non-secure, non-Hyp TLB entries
  ArmSmmuGr0Write (Smmu, ARM_SMMU_GR0_TLBIALLNSNH, 0);

  // Wait for TLB sync to complete
  Timeout = 1000000;
  do {
    Status = ArmSmmuGr0Read (Smmu, ARM_SMMU_GR0_sTLBGSTATUS);
    if (!(Status & sTLBGSTATUS_GSACTIVE)) {
      break;
    }
    MicroSecondDelay (1);
  } while (--Timeout);
}

// ============================================================================
// Context Bank Management
// ============================================================================

static
VOID
ArmSmmuDisableContextBank (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32 Cbndx)
{
  UINT32 Sctlr;
  UINT32 Fsr;
  UINTN  Timeout;

  // Read current SCTLR
  Sctlr = ArmSmmuCbRead (Smmu, Cbndx, ARM_SMMU_CB_SCTLR);

  // Disable MMU
  Sctlr &= ~SCTLR_M;
  ArmSmmuCbWrite (Smmu, Cbndx, ARM_SMMU_CB_SCTLR, Sctlr);

  // Wait for MMU disable to complete
  Timeout = 1000;
  while (Timeout--) {
    Sctlr = ArmSmmuCbRead (Smmu, Cbndx, ARM_SMMU_CB_SCTLR);
    if (!(Sctlr & SCTLR_M)) {
      break;
    }
    MicroSecondDelay (10);
  }

  // Clear page table base addresses
  ArmSmmuCbWrite64 (Smmu, Cbndx, ARM_SMMU_CB_TTBR0, 0);
  ArmSmmuCbWrite64 (Smmu, Cbndx, ARM_SMMU_CB_TTBR1, 0);

  // Clear TCR
  ArmSmmuCbWrite (Smmu, Cbndx, ARM_SMMU_CB_TCR, 0);

  // Clear fault status
  Fsr = ArmSmmuCbRead (Smmu, Cbndx, ARM_SMMU_CB_FSR);
  if (Fsr) {
    ArmSmmuCbWrite (Smmu, Cbndx, ARM_SMMU_CB_FSR, Fsr);
  }
}

static
VOID
ArmSmmuFreeContextBank(
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32 Cbndx)
{
  UINT32 Cbar;
  UINT32 CbarType;
  UINT32 CbarVmid;

  // Disable the Context Bank
  ArmSmmuDisableContextBank (Smmu, Cbndx);

  // Mark the Context Bank as free to use
  Cbar = ArmSmmuGr1Read (Smmu, ARM_SMMU_GR1_CBAR(Cbndx));

  CbarType = (Cbar >> CBAR_TYPE_SHIFT) & CBAR_TYPE_MASK;
  CbarVmid = (Cbar >> CBAR_VMID_SHIFT) & CBAR_VMID_MASK;

  if (CbarType != CBAR_TYPE_S2_TRANS &&
      CbarType != CBAR_TYPE_S1_TRANS_S2_FAULT &&
      (CbarType != CBAR_TYPE_S1_TRANS_S2_BYPASS || CbarVmid != 0xFF)) {
    Cbar =
        ((CBAR_TYPE_S1_TRANS_S2_BYPASS & CBAR_TYPE_MASK) << CBAR_TYPE_SHIFT) |
        ((0xFF & CBAR_VMID_MASK) << CBAR_VMID_SHIFT);
    ArmSmmuGr1Write (Smmu, ARM_SMMU_GR1_CBAR(Cbndx), Cbar);
  }
}

// ============================================================================
// Stream Management
// ============================================================================

static
BOOLEAN
ArmSmmuSkipStream (
  IN UINT32 StreamMask,
  IN UINT32 MaskedStreamId,
  IN CONST UINT16 *SkipStreams,
  IN UINTN SkipStreamCount)
{
  for (UINTN i = 0; i < SkipStreamCount; i++)
  {
    if ((SkipStreams[i] & ~StreamMask) == MaskedStreamId) {
      return TRUE;
    }
  }

  return FALSE;
}

static
VOID
ArmSmmuDetachAll(
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN CONST UINT16 *SkipStreams,
  IN UINTN SkipStreamCount)
{
  UINT32 Smr;
  UINT32 S2cr;
  UINT32 Cbndx;
  UINT32 S2crType;

  UINT32 SmrId;
  UINT32 SmrMask;
  UINT32 MaskedSmrId;
  UINT32 ContextBanks[16]; // Track context banks used
  UINT32 NumContextBanks = 0;

  UINT32 Id0     = ArmSmmuGr0Read (Smmu, ARM_SMMU_GR0_ID0);
  UINT32 NumSmrg = (Id0 >> 0) & 0xFF;

  for (UINT32 i = 0; i < NumSmrg; i++) {
    Smr = ArmSmmuGr0Read (Smmu, ARM_SMMU_GR0_SMR(i));

    if (Smr & SMR_VALID) {
      // Read S2CR register
      S2cr = ArmSmmuGr0Read (Smmu, ARM_SMMU_GR0_S2CR(i));

      // Extract Context Bank index and type
      Cbndx    = (S2cr >> S2CR_CBNDX_SHIFT) & S2CR_CBNDX_MASK;
      S2crType = (S2cr >> S2CR_TYPE_SHIFT) & S2CR_TYPE_MASK;

      // Extract StreamID and mask (16-bit fields)
      SmrId   = (Smr >> SMR_ID_SHIFT) & SMR_ID_MASK;
      SmrMask = (Smr >> SMR_MASK_SHIFT) & SMR_ID_MASK;

      MaskedSmrId = SmrId & ~SmrMask;

      if (ArmSmmuSkipStream (SmrMask, MaskedSmrId, SkipStreams, SkipStreamCount))
      {
        BOOLEAN AlreadyTracked = FALSE;

        for (UINT32 i = 0; i < NumContextBanks; i++) {
          if (ContextBanks[i] == Cbndx) {
            AlreadyTracked = TRUE;
            break;
          }
        }

        if (!AlreadyTracked && NumContextBanks < 16) {
          ContextBanks[NumContextBanks++] = Cbndx;
        }

        // Skip removing the SMR entry
        continue;
      }

      // Set S2CR to FAULT mode to block all accesses
      S2cr = ((S2CR_TYPE_FAULT & S2CR_TYPE_MASK) << S2CR_TYPE_SHIFT) |
             ((Cbndx & S2CR_CBNDX_MASK) << S2CR_CBNDX_SHIFT);
      ArmSmmuGr0Write (Smmu, ARM_SMMU_GR0_S2CR(i), S2cr);

      // Invalidate the SMR entry
      Smr &= ~SMR_VALID;
      ArmSmmuGr0Write (Smmu, ARM_SMMU_GR0_SMR(i), Smr);
    }
  }

  // Invalidate TLB
  ArmSmmuInvalidateAllTlb (Smmu);

  for (Cbndx = 0; Cbndx < Smmu->NumContextBanks; Cbndx++) {
    BOOLEAN AlreadyTracked = FALSE;

    for (UINT32 i = 0; i < NumContextBanks; i++) {
      if (ContextBanks[i] == Cbndx) {
        AlreadyTracked = TRUE;
        break;
      }
    }

    // Check if the context bank is reserved by streams we deliberately keep
    if (AlreadyTracked) {
      continue;
    }

    ArmSmmuFreeContextBank (Smmu, Cbndx);
  }
}

VOID
ArmSmmuDetach(
  IN CONST UINT16 *SkipStreams,
  IN UINTN SkipStreamCount
)
{
  ARM_SMMU_V2_DEVICE Smmu = {0};
  EFI_MEMORY_REGION_DESCRIPTOR_EX MemoryRegion;

  LocateMemoryMapAreaByName ("SMMU", &MemoryRegion);
  Smmu.BaseAddress = MemoryRegion.Address;

  UINT32 id1         = ArmSmmuGr0Read (&Smmu, ARM_SMMU_GR0_ID1);
  Smmu.PageShift = (id1 & ARM_SMMU_ID1_PAGESIZE) ? 16 : 12;
  Smmu.NumPage = 1 << ((READ_FIELD(ARM_SMMU_ID1_NUMPAGENDXB, id1)) + 1);
  Smmu.NumContextBanks = id1 & ARM_SMMU_ID1_NUMCB_MASK;

  ArmSmmuDetachAll (&Smmu, SkipStreams, SkipStreamCount);
}