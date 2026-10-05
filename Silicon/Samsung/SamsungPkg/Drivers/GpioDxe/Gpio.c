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
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/GpioBankLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIGpio.h>

#include "Gpio.h"

//
// Global Variables
//
STATIC EFI_GPIO_CTRL *mCtrl[GPIO_CTRL_COUNT] = { NULL };

EFI_STATUS
GetBankDetails (
  IN  EFI_GPIO_BANK_ID  Id,
  IN  UINT8             Number,
  OUT UINT8            *CtrlNum,
  OUT UINT16           *Offset)
{
  EFI_GPIO_BANK *Bank;
  UINT8          BankCount;

  // Get Platform Banks
  GetGpioBanks (&Bank, &BankCount);

  // Go thru each Bank
  for (UINT8 i = 0; i < BankCount; i++) {
    // Compare Bank IDs & Numbers
    if (Bank[i].Id == Id && Bank[i].Number == Number) {
      // Save Bank Controller Number & Offset
      *CtrlNum = Bank[i].CtrlNum;
      *Offset  = Bank[i].Offset;

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
GpioGetState (
  IN  EFI_GPIO_BANK_ID  BankId,
  IN  UINT8             BankNumber,
  IN  UINT8             Pin,
  OUT BOOLEAN          *State)
{
  EFI_STATUS Status;
  UINT32     Value;
  UINT16     Offset;
  UINT8      CtrlNum;

  // Verify Bank & Pin
  if (BankId >= BANK_ID_MAX || Pin >= MAX_GPIO_PIN_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Bank Controller Number & Offset
  Status = GetBankDetails (BankId, BankNumber, &CtrlNum, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Verify Controller State
  if (mCtrl[CtrlNum] == NULL) {
    return EFI_NOT_READY;
  }

  // Get current Pin State
  Value = MmioRead32 ((UINTN)&mCtrl[CtrlNum]->dat + Offset);

  // Pass Pin State
  *State = !!(Value & DAT_MASK (Pin));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GpioSetState (
  IN EFI_GPIO_BANK_ID  BankId,
  IN UINT8             BankNumber,
  IN UINT8             Pin,
  IN BOOLEAN          *Enable)
{
  EFI_STATUS Status;
  UINT16     Offset;
  UINT8      CtrlNum;

  // Verify Bank & Pin
  if (BankId >= BANK_ID_MAX || Pin >= MAX_GPIO_PIN_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Bank Controller Number & Offset
  Status = GetBankDetails (BankId, BankNumber, &CtrlNum, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Verify Controller State
  if (mCtrl[CtrlNum] == NULL) {
    return EFI_NOT_READY;
  }

  // Clear current Pin State
  MmioAnd32 ((UINTN)&mCtrl[CtrlNum]->dat + Offset, ~DAT_MASK (Pin));

  // Enable Pin
  if (Enable) {
    MmioOr32 ((UINTN)&mCtrl[CtrlNum]->dat + Offset, DAT_SET (Pin));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GpioSetFunction (
  IN EFI_GPIO_BANK_ID  BankId,
  IN UINT8             BankNumber,
  IN UINT8             Pin,
  IN EFI_GPIO_FUNCTION Function)
{
  EFI_STATUS Status;
  UINT16     Offset;
  UINT8      CtrlNum;

  // Verify Bank & Pin & Function
  if (BankId >= BANK_ID_MAX || Pin >= MAX_GPIO_PIN_COUNT || Function >= FUNCTION_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Bank Controller Number & Offset
  Status = GetBankDetails (BankId, BankNumber, &CtrlNum, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Verify Controller State
  if (mCtrl[CtrlNum] == NULL) {
    return EFI_NOT_READY;
  }

  // Re-set Pin Function
  MmioAndThenOr32 ((UINTN)&mCtrl[CtrlNum]->con + Offset, ~CON_MASK (Pin), CON_SFR (Pin, Function));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GpioSetPull (
  IN EFI_GPIO_BANK_ID   BankId,
  IN UINT8              BankNumber,
  IN UINT8              Pin,
  IN EFI_GPIO_PULL_MODE Pull)
{
  EFI_STATUS Status;
  UINT16     Offset;
  UINT8      CtrlNum;

  // Verify Bank & Pin & Pull
  if (BankId >= BANK_ID_MAX || Pin >= MAX_GPIO_PIN_COUNT || (Pull >= PULL_MAX || Pull == 2)) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Bank Controller Number & Offset
  Status = GetBankDetails (BankId, BankNumber, &CtrlNum, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Verify Controller State
  if (mCtrl[CtrlNum] == NULL) {
    return EFI_NOT_READY;
  }

  // Re-set Pin Pull
  MmioAndThenOr32 ((UINTN)&mCtrl[CtrlNum]->pull + Offset, ~PULL_MASK (Pin), PULL_MODE (Pin, Pull));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GpioSetDrive (
  IN EFI_GPIO_BANK_ID        BankId,
  IN UINT8                   BankNumber,
  IN UINT8                   Pin,
  IN EFI_GPIO_DRIVE_STRENGTH Drive)
{
  EFI_STATUS Status;
  UINT16     Offset;
  UINT8      CtrlNum;

  // Verify Bank & Pin & Drive
  if (BankId >= BANK_ID_MAX || Pin >= MAX_GPIO_PIN_COUNT || Drive >= DRIVE_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Bank Controller Number & Offset
  Status = GetBankDetails (BankId, BankNumber, &CtrlNum, &Offset);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Verify Controller State
  if (mCtrl[CtrlNum] == NULL) {
    return EFI_NOT_READY;
  }

  // Re-set Pin Drive Strength
  MmioAndThenOr32 ((UINTN)&mCtrl[CtrlNum]->drv + Offset, ~DRV_MASK (Pin), DRV_SET (Pin, Drive));

  return EFI_SUCCESS;
}

STATIC EFI_GPIO_PROTOCOL pGpio = {
  GpioGetState,
  GpioSetState,
  GpioSetFunction,
  GpioSetPull,
  GpioSetDrive
};

EFI_STATUS
EFIAPI
InitGpio (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Controller Addresses
  CONST UINT32 *CtrlAddress = (UINT32 *)FixedPcdGetPtr (PcdGpioCtrlAddr);
  if (CtrlAddress[0] == MAX_UINT32) {
    return EFI_UNSUPPORTED;
  }

  // Go thru each Controller
  for (UINT8 i = 0; i < GPIO_CTRL_COUNT; i++) {
    // Map Controller Memory
    Status = MapMemoryRegion (CtrlAddress[i], GPIO_MMIO_LENGTH, EfiMemoryMappedIO);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Map Controller %u Memory (0x%p)! Status = %r\n", i, CtrlAddress[i], Status));
      continue;
    }

    // Populate Controller Structure
    mCtrl[i] = (EFI_GPIO_CTRL *)(EFI_PHYSICAL_ADDRESS)CtrlAddress[i];
  }

  // Register GPIO Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiGpioProtocolGuid, EFI_NATIVE_INTERFACE, &pGpio);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register GPIO Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
