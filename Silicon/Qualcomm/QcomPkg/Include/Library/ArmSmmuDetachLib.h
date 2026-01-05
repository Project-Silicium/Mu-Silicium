/** @file
    Copyright (c) 2024-2025. Project Aloha Authors. All rights reserved.
    Copyright (c) 2022-2025. DuoWoA Authors.
    Copyright (c) 2023-2026. Project Silicium Authors.
    SPDX-License-Identifier: MIT

    Portion of this code is written by referencing
    linux/drivers/iommu/arm/arm-smmu/arm-smmu.c
*/

#include <Uefi.h>

VOID ArmSmmuDetach(
  IN CONST UINT16 *SkipStreams,
  IN UINTN SkipStreamCount
);