/**
  Portion of this Code is Written by Referencing this:
  linux/drivers/iommu/arm/arm-smmu/arm-smmu.c

  Copyright (c) 2024-2025. Project Aloha Authors. All rights reserved.
  Copyright (c) 2022-2025. DuoWoA Authors.
  Copyright (c) 2023-2026. Project Silicium Authors.

  SPDX-License-Identifier: MIT
*/

#ifndef _SMMU_V2_H_
#define _SMMU_V2_H_

//
// ?
//
#define GEN_MSK(h, l)        (((1U << ((h) - (l) + 1)) - 1) << (l))
#define READ_FIELD(msk, val) (((val) & (msk)) >> (__builtin_ffsll(msk) - 1))

//
// SMMU v2 Register offsets
//
#define ARM_SMMU_GR0_ID0 0x20
#define ARM_SMMU_GR0_ID1 0x24

//
// Page Selector Constants
//
#define ARM_SMMU_CB(s, n)        ((s)->NumPage + (n))
#define ARM_SMMU_ID1_NUMPAGENDXB  GEN_MSK (30, 28)
#define ARM_SMMU_GR0              0
#define ARM_SMMU_GR1              1

//
// Stream Mapping Registers
//
#define ARM_SMMU_GR0_SMR(n)  (0x800 + ((n) << 2))
#define ARM_SMMU_GR0_S2CR(n) (0xC00 + ((n) << 2))

//
// Context Bank Registers
//
#define ARM_SMMU_CB_TTBR0 0x20
#define ARM_SMMU_CB_TTBR1 0x28
#define ARM_SMMU_CB_TCR   0x30
#define ARM_SMMU_CB_FSR   0x58

//
// SMR Bits
//
#define SMR_VALID      (1U << 31)
#define SMR_MASK_SHIFT  16
#define SMR_ID_SHIFT    0
#define SMR_ID_MASK     0xFFFF

//
// S2CR Type Bits
//
#define S2CR_CBNDX_SHIFT 0
#define S2CR_CBNDX_MASK  0xFF
#define S2CR_TYPE_SHIFT  16
#define S2CR_TYPE_MASK   0x3
#define S2CR_TYPE_FAULT  2

//
// ID Register Fields
//
#define ARM_SMMU_ID1_PAGESIZE   (1 << 31)
#define ARM_SMMU_ID1_NUMCB_MASK  0xFF

//
// CBAR Register Fields
//
#define ARM_SMMU_GR1_CBAR(n)         ((n) << 2)
#define CBAR_VMID_SHIFT               0
#define CBAR_VMID_MASK                0xFF
#define CBAR_TYPE_SHIFT               16
#define CBAR_TYPE_MASK                0x3
#define CBAR_TYPE_S2_TRANS            0
#define CBAR_TYPE_S1_TRANS_S2_BYPASS  1
#define CBAR_TYPE_S1_TRANS_S2_FAULT   2

//
// SMMU v2 Device
//
typedef struct {
  UINTN  BaseAddress;
  UINTN  PageShift;
  UINT32 NumContextBanks;
  UINT32 NumPage;
} ARM_SMMU_V2_DEVICE;

//
// IO Functions
//
UINT32
ArmSmmuGr0Read (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset
  );

VOID
ArmSmmuGr0Write (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset,
  IN UINT32              Value
  );

UINT32
ArmSmmuGr1Read (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset
  );

VOID
ArmSmmuGr1Write (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset,
  IN UINT32              Value
  );

UINT32
ArmSmmuCbRead (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32              Bank,
  IN UINTN               Offset
  );

VOID
ArmSmmuCbWrite (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32              Bank,
  IN UINTN               Offset,
  IN UINT32              Value
  );

VOID
ArmSmmuCbWrite64 (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32              Bank,
  IN UINTN               Offset,
  IN UINT64              Value
  );

#endif /* _SMMU_V2_H_ */
