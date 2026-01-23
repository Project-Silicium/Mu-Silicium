/**
  Portion of this Code is Written by Referencing this:
  linux/drivers/iommu/arm/arm-smmu/arm-smmu.c

  Copyright (c) 2024-2025. Project Aloha Authors. All rights reserved.
  Copyright (c) 2022-2025. DuoWoA Authors.
  Copyright (c) 2023-2026. Project Silicium Authors.

  SPDX-License-Identifier: MIT
*/

#include <Library/IoLib.h>
#include <Library/BaseLib.h>

#include "SmmuV2.h"

UINT32
ArmSmmuGr0Read (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset)
{
  return MmioRead32 (Smmu->BaseAddress + (ARM_SMMU_GR0 << Smmu->PageShift) + Offset);
}

VOID
ArmSmmuGr0Write (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset,
  IN UINT32              Value)
{
  MmioWrite32 (Smmu->BaseAddress + (ARM_SMMU_GR0 << Smmu->PageShift) + Offset, Value);

  MemoryFence ();
}

UINT32
ArmSmmuGr1Read (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset)
{
  return MmioRead32 (Smmu->BaseAddress + (ARM_SMMU_GR1 << Smmu->PageShift) + Offset);
}

VOID
ArmSmmuGr1Write (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINTN               Offset,
  IN UINT32              Value)
{
  MmioWrite32 (Smmu->BaseAddress + (ARM_SMMU_GR1 << Smmu->PageShift) + Offset, Value);

  MemoryFence ();
}

UINT32
ArmSmmuCbRead (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32              Bank,
  IN UINTN               Offset)
{
  return MmioRead32 (Smmu->BaseAddress + (UINTN)(ARM_SMMU_CB (Smmu, Bank) << Smmu->PageShift) + Offset);
}

VOID
ArmSmmuCbWrite (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32              Bank,
  IN UINTN               Offset,
  IN UINT32              Value)
{
  MmioWrite32 (Smmu->BaseAddress + (UINTN)(ARM_SMMU_CB (Smmu, Bank) << Smmu->PageShift) + Offset, Value);

  MemoryFence ();
}

VOID
ArmSmmuCbWrite64 (
  IN ARM_SMMU_V2_DEVICE *Smmu,
  IN UINT32              Bank,
  IN UINTN               Offset,
  IN UINT64              Value)
{
  ArmSmmuCbWrite (Smmu, Bank, Offset, (UINT32)Value);
  ArmSmmuCbWrite (Smmu, Bank, Offset + 4, (UINT32)(Value >> 32));
}
