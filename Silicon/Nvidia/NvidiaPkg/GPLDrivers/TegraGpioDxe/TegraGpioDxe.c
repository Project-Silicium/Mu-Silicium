/**
  Tegra GPIO Driver

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
GetConfig (UINTN Gpio)
{
  UINT32 Value;
  INT32  Type;

  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  Value = MmioRead32 ((UINTN)&Bank->GpioConfig[GPIO_PORT(Gpio)]);
  Type  = (Value >> GPIO_BIT(Gpio)) & 1;

  return Type ? CONFIG_GPIO : CONFIG_SFIO;
}

STATIC
EFI_STATUS
SetConfig (
  UINTN Gpio,
  INT32 Type)
{
  UINT32 Value;

  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  Value = MmioRead32 ((UINTN)&Bank->GpioConfig[GPIO_PORT(Gpio)]);

  if (Type == CONFIG_GPIO) {
    Value |= 1 << GPIO_BIT(Gpio);
  } else if (Type == CONFIG_SFIO) {
    Value &= ~(1 << GPIO_BIT(Gpio));
  } else {
    DEBUG ((EFI_D_ERROR, "Invalid GPIO Configuration!\n"));
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 ((UINTN)&Bank->GpioConfig[GPIO_PORT(Gpio)], Value);

  return EFI_SUCCESS;
}

STATIC
INT32
GetDirection (UINTN Gpio)
{
  UINT32 Value;
  INT32  Direction;

  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  Value     = MmioRead32 ((UINTN)&Bank->GpioDirection[GPIO_PORT(Gpio)]);
  Direction = (Value >> GPIO_BIT(Gpio)) & 1;

  return Direction ? DIRECTION_OUTPUT : DIRECTION_INPUT;
}

STATIC
EFI_STATUS
SetDirection (
  UINTN Gpio,
  INT32 Direction)
{
  UINT32 Value;

  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  Value = MmioRead32 ((UINTN)&Bank->GpioDirection[GPIO_PORT(Gpio)]);

  if (Direction == DIRECTION_OUTPUT) {
    Value |= 1 << GPIO_BIT(Gpio);
  } else if (Direction == DIRECTION_INPUT) {
    Value &= ~(1 << GPIO_BIT(Gpio));
  } else {
    DEBUG ((EFI_D_ERROR, "Invalid GPIO Direction!\n"));
    return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 ((UINTN)&Bank->GpioDirection[GPIO_PORT(Gpio)], Value);

  return EFI_SUCCESS;
}

STATIC
VOID
SetLevel (
  UINTN   Gpio,
  BOOLEAN High)
{
  UINT32 Value;

  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  Value = MmioRead32 ((UINTN)&Bank->GpioOut[GPIO_PORT(Gpio)]);

  if (High) {
    Value |= 1 << GPIO_BIT(Gpio);
  } else {
    Value &= ~(1 << GPIO_BIT(Gpio));
  }

  MmioWrite32 ((UINTN)&Bank->GpioOut[GPIO_PORT(Gpio)], Value);
}

STATIC
INT32
GetState (UINTN Gpio)
{
  UINT32 Value;

  GpioCtrl     *Ctrl = (GpioCtrl *)GpioMemoryRegion.Address;
  GpioCtrlBank *Bank = &Ctrl->GpioBank[GPIO_BANK(Gpio)];

  if (GetDirection(Gpio) == DIRECTION_INPUT) {
    Value = MmioRead32 ((UINTN)&Bank->GpioIn[GPIO_PORT(Gpio)]);
  } else {
    Value = MmioRead32 ((UINTN)&Bank->GpioOut[GPIO_PORT(Gpio)]);
  }

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
    goto exit;
  }

  // Install Tegra Gpio Protocol in a new Handle
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiTegraGpioProtocolGuid, &mTegraGpio, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Tegra GPIO Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

exit:
  return Status;
}