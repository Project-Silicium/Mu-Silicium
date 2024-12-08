/**
  Copyright (C) 2010-2012, 2015, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>

#include <Protocol/EfiTegraGpio.h>

#include "TegraGpioDxe.h"

STATIC CONST INT32 CONFIG_SFIO      = 0;
STATIC CONST INT32 CONFIG_GPIO      = 1;
STATIC CONST INT32 DIRECTION_INPUT  = 0;
STATIC CONST INT32 DIRECTION_OUTPUT = 1;

STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX GpioMemoryRegion;

STATIC
INT32
GetConfig (IN UINTN Gpio)
{
  UINT32 Value;
  INT32  Type;

  // Get GPIO Bank
  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  // Get Current GPIO Config
  Value = MmioRead32 ((UINTN)&Bank->GpioConfig[GPIO_PORT(Gpio)]);
  Type  = (Value >> GPIO_BIT(Gpio)) & 1;

  return Type ? CONFIG_GPIO : CONFIG_SFIO;
}

STATIC
EFI_STATUS
SetConfig (
  IN UINTN Gpio,
  IN INT32 Type)
{
  UINT32 Value;

  // Get GPIO Bank
  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  // Get Current GPIO Config
  Value = MmioRead32 ((UINTN)&Bank->GpioConfig[GPIO_PORT(Gpio)]);

  // Set new GPIO Config
  if (Type == CONFIG_GPIO) {
    Value |= 1 << GPIO_BIT(Gpio);
  } else if (Type == CONFIG_SFIO) {
    Value &= ~(1 << GPIO_BIT(Gpio));
  } else {
    DEBUG ((EFI_D_ERROR, "Invalid GPIO Configuration!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Write New GPIO Config
  MmioWrite32 ((UINTN)&Bank->GpioConfig[GPIO_PORT(Gpio)], Value);

  return EFI_SUCCESS;
}

STATIC
INT32
GetDirection (IN UINTN Gpio)
{
  UINT32 Value;
  INT32  Direction;

  // Get GPIO Bank
  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  // Get Current Direction
  Value     = MmioRead32 ((UINTN)&Bank->GpioDirection[GPIO_PORT(Gpio)]);
  Direction = (Value >> GPIO_BIT(Gpio)) & 1;

  return Direction ? DIRECTION_OUTPUT : DIRECTION_INPUT;
}

STATIC
EFI_STATUS
SetDirection (
  IN UINTN Gpio,
  IN INT32 Direction)
{
  UINT32 Value;

  // Get GPIO Bank
  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  // Get Current GPIO Direction
  Value = MmioRead32 ((UINTN)&Bank->GpioDirection[GPIO_PORT(Gpio)]);

  // Set New GPIO Direction
  if (Direction == DIRECTION_OUTPUT) {
    Value |= 1 << GPIO_BIT(Gpio);
  } else if (Direction == DIRECTION_INPUT) {
    Value &= ~(1 << GPIO_BIT(Gpio));
  } else {
    DEBUG ((EFI_D_ERROR, "Invalid GPIO Direction!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Write New GPIO Direction
  MmioWrite32 ((UINTN)&Bank->GpioDirection[GPIO_PORT(Gpio)], Value);

  return EFI_SUCCESS;
}

STATIC
VOID
SetLevel (
  IN UINTN   Gpio,
  IN BOOLEAN High)
{
  UINT32 Value;

  // Get GPIO Bank
  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  // Get Current GPIO Level
  Value = MmioRead32 ((UINTN)&Bank->GpioOut[GPIO_PORT(Gpio)]);

  // Set New GPIO Level
  if (High) {
    Value |= 1 << GPIO_BIT(Gpio);
  } else {
    Value &= ~(1 << GPIO_BIT(Gpio));
  }

  // Write New GPIO Level
  MmioWrite32 ((UINTN)&Bank->GpioOut[GPIO_PORT(Gpio)], Value);
}

STATIC
INT32
GetState (IN UINTN Gpio)
{
  UINT32 Value;

  // Get GPIO Bank
  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  // Get Current State of Defined GPIO
  if (GetDirection(Gpio) == DIRECTION_INPUT) {
    Value = MmioRead32 ((UINTN)&Bank->GpioIn[GPIO_PORT(Gpio)]);
  } else {
    Value = MmioRead32 ((UINTN)&Bank->GpioOut[GPIO_PORT(Gpio)]);
  }

  // Return GPIO State
  return (Value >> GPIO_BIT(Gpio)) & 1;
}

STATIC EFI_TEGRA_GPIO_PROTOCOL mTegraGpio = {
  GetConfig,
  SetConfig,
  GetDirection,
  SetDirection,
  SetLevel,
  GetState
};

EFI_STATUS
EFIAPI
InitGpioDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get GPIO Controller Memory Region
  Status = LocateMemoryMapAreaByName ("GPIO Controller", &GpioMemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get GPIO Controller Memory Region!\n"));
    return Status;
  }

  // Register Tegra GPIO Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiTegraGpioProtocolGuid, &mTegraGpio, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Tegra GPIO Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
