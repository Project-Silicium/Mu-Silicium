/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.

  Alternatively, this program is free software in case of open source project
  you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiExynosGpa.h>

#include "ExynosGpaDxe.h"

VOID
ConfigurePin (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Config)
{
  UINT32 Value;

  Value  = MmioRead32 ((UINTN)&Bank->Con + Offset);
  Value &= ~CON_MASK(Pin);
  Value |= CON_SFR(Pin, Config);

  MmioWrite32 ((UINTN)&Bank->Con + Offset, Value);
}

VOID
SetDirectionOutput (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  BOOLEAN        Enable)
{
  UINT32 Value;

  ConfigurePin (Bank, Offset, Pin, GPA_OUTPUT);

  Value  = MmioRead32 ((UINTN)&Bank->Dat + Offset);
  Value &= ~DAT_MASK(Pin);

  if (Enable) {
    Value |= DAT_SET(Pin);
  }

  MmioWrite32 ((UINTN)&Bank->Dat + Offset, Value);
}

VOID
SetDirectionInput (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin)
{
  ConfigurePin (Bank, Offset, Pin, GPA_INPUT);
}

UINT32
GetGpa (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin)
{
  UINT32 Value;

  Value = MmioRead32 ((UINTN)&Bank->Dat + Offset);

  return (Value & DAT_MASK(Pin));
}

VOID
SetPull (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Mode)
{
  UINT32 Value;

  Value  = MmioRead32 ((UINTN)&Bank->Pull + Offset);
  Value &= ~PULL_MASK(Pin);

  switch (Mode) {
    case GPA_PULL_NONE:
    case GPA_PULL_DOWN:
    case GPA_PULL_UP:
      Value |= PULL_MODE(Pin, Mode);
      break;
    
    default:
      break;
  }

  MmioWrite32 ((UINTN)&Bank->Pull + Offset, Value);
}

VOID
SetDrv (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Mode)
{
  UINT32 Value;

  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~DRV_MASK(Pin);
  Value |= DRV_SET(Pin, Mode);

  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);
}

VOID
SetRate (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Mode)
{
  UINT32 Value;

  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~RATE_MASK(Pin);

  switch (Mode) {
    case GPA_DRV_FAST:
    case GPA_DRV_SLOW:
      Value |= RATE_SET(Pin);
      break;
    
    default:
      return;
  }

  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);
}

STATIC EFI_EXYNOS_GPA_PROTOCOL mExynosGpa = {
  ConfigurePin,
  SetDirectionOutput,
  SetDirectionInput,
  GetGpa,
  SetPull,
  SetDrv,
  SetRate
};

EFI_STATUS
EFIAPI
InitGpaDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Install Exynos Gpa Protocol in a new Handle
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiExynosGpaProtocolGuid, &mExynosGpa, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Exynos GPA Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}