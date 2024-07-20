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

#include <Protocol/EfiExynosGpio.h>

#include "ExynosGpioDxe.h"

VOID
ConfigurePin (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin,
  IN INT32          Config)
{
  UINT32 Value;

  // Set Configuration Value
  Value  = MmioRead32 ((UINTN)&Bank->Con + Offset);
  Value &= ~CON_MASK(Pin);
  Value |= CON_SFR(Pin, Config);

  // Write new Configuration
  MmioWrite32 ((UINTN)&Bank->Con + Offset, Value);
}

VOID
SetDirectionOutput (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin,
  IN BOOLEAN        Enable)
{
  UINT32 Value;

  // Configure Pin
  ConfigurePin (Bank, Offset, Pin, GPA_OUTPUT);

  // Set Direction Value
  Value  = MmioRead32 ((UINTN)&Bank->Dat + Offset);
  Value &= ~DAT_MASK(Pin);

  // Enable Pin
  if (Enable) {
    Value |= DAT_SET(Pin);
  }

  // Write new Direction
  MmioWrite32 ((UINTN)&Bank->Dat + Offset, Value);
}

VOID
SetDirectionInput (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin)
{
  // Configure Pin
  ConfigurePin (Bank, Offset, Pin, GPA_INPUT);
}

UINT32
GetPin (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin)
{
  UINT32 Value;

  // Get current Pin State
  Value = MmioRead32 ((UINTN)&Bank->Dat + Offset);

  // Return Pin State
  return (Value & DAT_MASK(Pin));
}

VOID
SetPull (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin,
  IN INT32          Mode)
{
  UINT32 Value;

  // Set Pull Mask Value
  Value  = MmioRead32 ((UINTN)&Bank->Pull + Offset);
  Value &= ~PULL_MASK(Pin);

  // Set Pull Mode
  switch (Mode) {
    case GPA_PULL_NONE:
    case GPA_PULL_DOWN:
    case GPA_PULL_UP:
      Value |= PULL_MODE(Pin, Mode);
      break;
    
    default:
      break;
  }

  // Write new Pull Configuration
  MmioWrite32 ((UINTN)&Bank->Pull + Offset, Value);
}

VOID
SetDrv (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin,
  IN INT32          Mode)
{
  UINT32 Value;

  // Set DRV Configuration
  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~DRV_MASK(Pin);
  Value |= DRV_SET(Pin, Mode);

  // Write new DRV Configuration
  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);
}

VOID
SetRate (
  IN ExynosGpioBank *Bank,
  IN UINT32         Offset,
  IN INT32          Pin,
  IN INT32          Mode)
{
  UINT32 Value;

  // Set Rate Mask
  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~RATE_MASK(Pin);

  // Set Rate Speed
  switch (Mode) {
    case GPA_DRV_FAST:
    case GPA_DRV_SLOW:
      Value |= RATE_SET(Pin);
      break;
    
    default:
      return;
  }

  // Write new Rate Configuration
  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);
}

STATIC EFI_EXYNOS_GPIO_PROTOCOL mExynosGpio = {
  ConfigurePin,
  SetDirectionOutput,
  SetDirectionInput,
  GetPin,
  SetPull,
  SetDrv,
  SetRate
};

EFI_STATUS
EFIAPI
InitGpioDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Register Exynos GPIO Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiExynosGpioProtocolGuid, &mExynosGpio, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Exynos GPIO Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
