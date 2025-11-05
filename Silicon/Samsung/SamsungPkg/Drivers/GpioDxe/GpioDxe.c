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
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiGpio.h>

#include "Gpio.h"

//
// Global Variables
//
EFI_MEMORY_REGION_DESCRIPTOR_EX GpioRegion;

EFI_STATUS
ConfigurePin (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Config)
{
  EFI_STATUS Status;
  UINT32     Value;

  // Check GPIO Address
  Status = LocateMemoryMapAreaByAddress ((EFI_PHYSICAL_ADDRESS)Bank, &GpioRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: This GPIO Base Address is not Mapped!\n", __FUNCTION__));
    return Status;
  }

  // Set Configuration Value
  Value  = MmioRead32 ((UINTN)&Bank->Con + Offset);
  Value &= ~CON_MASK(Pin);
  Value |= CON_SFR(Pin, Config);

  // Write new Configuration
  MmioWrite32 ((UINTN)&Bank->Con + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetDirectionOutput (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN BOOLEAN   Enable)
{
  EFI_STATUS Status;
  UINT32     Value;

  // Check GPIO Address
  Status = LocateMemoryMapAreaByAddress ((EFI_PHYSICAL_ADDRESS)Bank, &GpioRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: This GPIO Base Address is not Mapped!\n", __FUNCTION__));
    return Status;
  }

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

  return EFI_SUCCESS;
}

EFI_STATUS
SetDirectionInput (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin)
{
  // Configure Pin
  return ConfigurePin (Bank, Offset, Pin, GPA_INPUT);
}

EFI_STATUS
GetPin (
  IN  GpioBank *Bank,
  IN  UINT32    Offset,
  IN  INT32     Pin,
  OUT UINT32   *State)
{
  EFI_STATUS Status;
  UINT32     Value;

  // Check GPIO Address
  Status = LocateMemoryMapAreaByAddress ((EFI_PHYSICAL_ADDRESS)Bank, &GpioRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: This GPIO Base Address is not Mapped!\n", __FUNCTION__));
    return Status;
  }

  // Get current Pin State
  Value = MmioRead32 ((UINTN)&Bank->Dat + Offset);

  // Return Pin State
  *State =  (Value & DAT_MASK(Pin));

  return EFI_SUCCESS;
}

EFI_STATUS
SetPull (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Mode)
{
  EFI_STATUS Status;
  UINT32     Value;

  // Check GPIO Address
  Status = LocateMemoryMapAreaByAddress ((EFI_PHYSICAL_ADDRESS)Bank, &GpioRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: This GPIO Base Address is not Mapped!\n", __FUNCTION__));
    return Status;
  }

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
      DEBUG ((EFI_D_ERROR, "%a: Invalid Pull Mode! Got %d\n", __FUNCTION__, Mode));
      return EFI_INVALID_PARAMETER;
  }

  // Write new Pull Configuration
  MmioWrite32 ((UINTN)&Bank->Pull + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetDrv (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Mode)
{
  EFI_STATUS Status;
  UINT32     Value;

  // Check GPIO Address
  Status = LocateMemoryMapAreaByAddress ((EFI_PHYSICAL_ADDRESS)Bank, &GpioRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: This GPIO Base Address is not Mapped!\n", __FUNCTION__));
    return Status;
  }

  // Set DRV Configuration
  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~DRV_MASK(Pin);
  Value |= DRV_SET(Pin, Mode);

  // Write new DRV Configuration
  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetRate (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Mode)
{
  EFI_STATUS Status;
  UINT32     Value;

  // Check GPIO Address
  Status = LocateMemoryMapAreaByAddress ((EFI_PHYSICAL_ADDRESS)Bank, &GpioRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: This GPIO Base Address is not Mapped!\n", __FUNCTION__));
    return Status;
  }

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
    DEBUG ((EFI_D_ERROR, "%a: Invalid Speed Mode! Got %d\n", __FUNCTION__, Mode));
    return EFI_INVALID_PARAMETER;
  }

  // Write new Rate Configuration
  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);

  return EFI_SUCCESS;
}

STATIC EFI_GPIO_PROTOCOL mGpio = {
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

  // Register GPIO Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiGpioProtocolGuid, &mGpio, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GPIO Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
