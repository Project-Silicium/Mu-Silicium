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
#include <Library/GpioBankLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiGpio.h>

#include "Gpio.h"

//
// Global Variables
//
EFI_MEMORY_REGION_DESCRIPTOR_EX PinctrlRegion;

EFI_STATUS
GetBankData (
  IN  UINT8                 Number,
  IN  UINT8                 Id,
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT16               *Offset)
{
  EFI_STATUS Status;

  // Get GPIO Bank Data
  EFI_GPIO_BANK_DATA *BankData = GetPlatformBankData ();

  // Loop thru each Bank ID
  for (UINT8 i = 0; i < sizeof (BankData); i++) {
    // Compare Bank IDs
    if (BankData[i].Id != Id) {
      continue;
    }

    // Loop thru each Bank
    for (UINT8 j = 0; j < sizeof (BankData[i].Bank); j++) {
      // Compare Bank Numbers
      if (BankData[i].Bank[j].Number != Number) {
        continue;
      }

      // Verify Pinctrl Address
      Status = LocateMemoryMapAreaByAddress (BankData[i].Bank[j].Address, &PinctrlRegion);
      if (EFI_ERROR (Status)) {
        return EFI_NO_MAPPING;
      }

      // Pass Address & Offset
      *Address = BankData[i].Bank[j].Address;
      *Offset  = BankData[i].Bank[j].Offset;

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
ConfigurePin (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Config)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS Address;
  UINT32               Value;
  UINT16               Offset;

  // Get GPIO Bank Data
  Status = GetBankData (BankNumber, BankId, &Address, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate GPIO Bank Structure
  EFI_GPIO_BANK *Bank = (EFI_GPIO_BANK *)Address;

  // Set New Configuration
  Value  = MmioRead32 ((UINTN)&Bank->Con + Offset);
  Value &= ~CON_MASK(Pin);
  Value |= CON_SFR(Pin, Config);

  // Write new Configuration
  MmioWrite32 ((UINTN)&Bank->Con + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetDirectionOutput (
  IN UINT8   BankNumber,
  IN UINT8   BankId,
  IN UINT8   Pin,
  IN BOOLEAN Enable)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS Address;
  UINT32               Value;
  UINT16               Offset;

  // Configure GPIO Pin
  Status = ConfigurePin (BankNumber, BankId, Pin, GPIO_OUTPUT);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get GPIO Bank Data
  Status = GetBankData (BankNumber, BankId, &Address, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate GPIO Bank Structure
  EFI_GPIO_BANK *Bank = (EFI_GPIO_BANK *)Address;

  // Set new Direction
  Value  = MmioRead32 ((UINTN)&Bank->Dat + Offset);
  Value &= ~DAT_MASK(Pin);

  // Enable GPIO Pin
  if (Enable) {
    Value |= DAT_SET(Pin);
  }

  // Write new Direction
  MmioWrite32 ((UINTN)&Bank->Dat + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetDirectionInput (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin)
{
  // Configure Pin
  return ConfigurePin (BankNumber, BankId, Pin, GPIO_INPUT);
}

EFI_STATUS
GetPin (
  IN  UINT8  BankNumber,
  IN  UINT8  BankId,
  IN  UINT8  Pin,
  OUT UINT8 *State)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS Address;
  UINT32               Value;
  UINT16               Offset;

  // Get GPIO Bank Data
  Status = GetBankData (BankNumber, BankId, &Address, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate GPIO Bank Structure
  EFI_GPIO_BANK *Bank = (EFI_GPIO_BANK *)Address;

  // Get current GPIO Pin State
  Value = MmioRead32 ((UINTN)&Bank->Dat + Offset);

  // Pass GPIO Pin State
  *State = (Value & DAT_MASK(Pin));

  return EFI_SUCCESS;
}

EFI_STATUS
SetPull (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Mode)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS Address;
  UINT32               Value;
  UINT16               Offset;

  // Get GPIO Bank Data
  Status = GetBankData (BankNumber, BankId, &Address, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate GPIO Bank Structure
  EFI_GPIO_BANK *Bank = (EFI_GPIO_BANK *)Address;

  // Set Pull Mask Value
  Value  = MmioRead32 ((UINTN)&Bank->Pull + Offset);
  Value &= ~PULL_MASK(Pin);

  // Set new Pull
  switch (Mode) {
    case GPIO_PULL_NONE:
    case GPIO_PULL_DOWN:
    case GPIO_PULL_UP:
      Value |= PULL_MODE(Pin, Mode);
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  // Write new Pull
  MmioWrite32 ((UINTN)&Bank->Pull + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetDrv (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Mode)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS Address;
  UINT32               Value;
  UINT16               Offset;

  // Get GPIO Bank Data
  Status = GetBankData (BankNumber, BankId, &Address, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate GPIO Bank Structure
  EFI_GPIO_BANK *Bank = (EFI_GPIO_BANK *)Address;

  // Set new DRV Configuration
  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~DRV_MASK(Pin);
  Value |= DRV_SET(Pin, Mode);

  // Write new DRV Configuration
  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);

  return EFI_SUCCESS;
}

EFI_STATUS
SetRate (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Mode)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS Address;
  UINT32               Value;
  UINT16               Offset;

  // Get GPIO Bank Data
  Status = GetBankData (BankNumber, BankId, &Address, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate GPIO Bank Structure
  EFI_GPIO_BANK *Bank = (EFI_GPIO_BANK *)Address;

  // Set Rate Mask
  Value  = MmioRead32 ((UINTN)&Bank->Drv + Offset);
  Value &= ~RATE_MASK(Pin);

  // Set new Rate
  switch (Mode) {
    case GPIO_DRV_FAST:
    case GPIO_DRV_SLOW:
      Value |= RATE_SET(Pin);
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  // Write new Rate
  MmioWrite32 ((UINTN)&Bank->Drv + Offset, Value);

  return EFI_SUCCESS;
}

STATIC EFI_EXYNOS_GPIO_PROTOCOL mGpio = {
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
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiExynosGpioProtocolGuid, &mGpio, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GPIO Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
