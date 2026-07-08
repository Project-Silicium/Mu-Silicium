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
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/GpioLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIGpio.h>

#include "Gpio.h"

//
// Global Variables
//
STATIC EFI_GPIO_CONTROLLER_DATA *ControllerData;
STATIC UINT8                     ControllerCount;

EFI_GPIO_BANK*
GetBank (
  IN EFI_GPIO_BANK_ID Id,
  IN UINT8            Number)
{
  // Verify Bank ID
  if (!Id || Id >= BANK_ID_COUNT) {
    return NULL;
  }

  // Go thru all GPIO Controller
  for (UINT8 i = 0; i < ControllerCount; i++) {
    // Get GPIO Controller Address
    EFI_PHYSICAL_ADDRESS ControllerAddress = ControllerData[i].Address;

    // Go thru all GPIO Banks
    for (UINT8 j = 0; j < MAX_GPIO_BANK_COUNT; j++) {
      // Get GPIO Bank Data
      EFI_GPIO_BANK_ID BankId     = ControllerData[i].BankData[j].Id;
      UINT16           BankOffset = ControllerData[i].BankData[j].Offset;
      UINT8            BankNumber = ControllerData[i].BankData[j].Number;

      // Compare Bank ID & Number
      if (BankId == Id && BankNumber == Number) {
        // Return GPIO Bank
        return (EFI_GPIO_BANK *)(ControllerAddress + BankOffset);
      }
    }
  }

  return NULL;
}

EFI_STATUS
GpioGetState (
  IN  EFI_GPIO_BANK_ID  BankId,
  IN  UINT8             BankNumber,
  IN  UINT8             Pin,
  OUT BOOLEAN          *State)
{
  UINT32 Value;

  // Verify GPIO Pin
  if (Pin >= MAX_GPIO_PIN_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  // Get GPIO Bank
  EFI_GPIO_BANK *Bank = GetBank (BankId, BankNumber);
  if (Bank == NULL) {
    return EFI_NOT_FOUND;
  }

  // Get current GPIO Bank DAT
  Value = MmioRead32 ((UINTN)&Bank->dat);

  // Pass GPIO Pin State
  *State = !!(Value & DAT_MASK (Pin));

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetState (
  IN EFI_GPIO_BANK_ID  BankId,
  IN UINT8             BankNumber,
  IN UINT8             Pin,
  IN BOOLEAN          *Enable)
{
  // Verify GPIO Pin
  if (Pin >= MAX_GPIO_PIN_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  // Get GPIO Bank
  EFI_GPIO_BANK *Bank = GetBank (BankId, BankNumber);
  if (Bank == NULL) {
    return EFI_NOT_FOUND;
  }

  // Clear current GPIO Pin State
  MmioAnd32 ((UINTN)&Bank->dat, ~DAT_MASK (Pin));

  // Enable GPIO Pin
  if (Enable) {
    MmioOr32 ((UINTN)&Bank->dat, DAT_SET (Pin));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetFunction (
  IN EFI_GPIO_BANK_ID  BankId,
  IN UINT8             BankNumber,
  IN UINT8             Pin,
  IN EFI_GPIO_FUNCTION Function)
{
  // Verify GPIO Pin & GPIO Pin Function
  if (Pin >= MAX_GPIO_PIN_COUNT || Function >= FUNCTION_NUM) {
    return EFI_INVALID_PARAMETER;
  }

  // Get GPIO Bank
  EFI_GPIO_BANK *Bank = GetBank (BankId, BankNumber);
  if (Bank == NULL) {
    return EFI_NOT_FOUND;
  }

  // Re-set GPIO Pin Function
  MmioAndThenOr32 ((UINTN)&Bank->con, ~CON_MASK (Pin), CON_SFR (Pin, Function));

  return EFI_SUCCESS;
}

EFI_STATUS
GpioSetPull (
  IN EFI_GPIO_BANK_ID   BankId,
  IN UINT8              BankNumber,
  IN UINT8              Pin,
  IN EFI_GPIO_PULL_MODE Pull)
{
  // Verify GPIO Pin & GPIO Pin Pull
  if (Pin >= MAX_GPIO_PIN_COUNT || Pull >= PULL_NUM) {
    return EFI_INVALID_PARAMETER;
  }

  // Get GPIO Bank
  EFI_GPIO_BANK *Bank = GetBank (BankId, BankNumber);
  if (Bank == NULL) {
    return EFI_NOT_FOUND;
  }

  // Re-set GPIO Pin Pull
  MmioAndThenOr32 ((UINTN)&Bank->pull, ~PULL_MASK (Pin), PULL_MODE (Pin, Pull));

  return EFI_SUCCESS;
}

STATIC EFI_GPIO_PROTOCOL mGpio = {
  GpioGetState,
  GpioSetState,
  GpioSetFunction,
  GpioSetPull
};

EFI_STATUS
EFIAPI
InitGpio (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get GPIO Controllers
  GetGpioControllerData (&ControllerData, &ControllerCount);

  // Verify GPIO Controller Count
  if (!ControllerCount) {
    return EFI_UNSUPPORTED;
  }

  // Go thru all GPIO Controllers
  for (UINT8 i = 0; i < ControllerCount; i++) {
    // Get GPIO Controller Address
    EFI_PHYSICAL_ADDRESS ControllerAddress = ControllerData[i].Address;

    // Map GPIO Controller
    Status = MapMemoryRegion (ControllerAddress, GPIO_MMIO_LENGTH, EfiMemoryMappedIO);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Map GPIO Controller: 0x%llx!\n", ControllerAddress));
      return Status;
    }
  }

  // Register GPIO Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiGpioProtocolGuid, EFI_NATIVE_INTERFACE, &mGpio);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GPIO Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
