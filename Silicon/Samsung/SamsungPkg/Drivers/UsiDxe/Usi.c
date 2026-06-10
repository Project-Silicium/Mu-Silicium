/**
  Copyright (c) 2017 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UsiLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIUsi.h>

#include "Usi.h"

//
// Global Variables
//
STATIC EFI_USI_DATA *UsiData;
STATIC UINT8         UsiCount;

EFI_STATUS
UsiGetControllerAddress (
  IN  UINT8                 BusNumber,
  IN  EFI_USI_BUS_TYPE      BusType,
  OUT EFI_PHYSICAL_ADDRESS *Address)
{
  UINT8 PrivateBusNumber;

  // Verify Address Parameter
  if (Address == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Go thru each USI
  for (UINT8 i = 0; i < UsiCount; i++) {
    // Get Bus Number
    switch (BusType) {
      case BUS_TYPE_UART:
        PrivateBusNumber = UsiData[i].UartBus;
        break;

      case BUS_TYPE_SPI:
        PrivateBusNumber = UsiData[i].SpiBus;
        break;

      case BUS_TYPE_I2C:
        PrivateBusNumber = UsiData[i].I2cBus;
        break;

      default:
        return EFI_INVALID_PARAMETER;
    }

    // Compare Bus Numbers
    if (PrivateBusNumber == BusNumber) {
      // Pass Controller Address
      *Address = UsiData[i].ControllerAddress;

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_PHYSICAL_ADDRESS
GetUsiAddress (IN EFI_PHYSICAL_ADDRESS Address)
{
  // Go thru each USI
  for (UINT8 i = 0; i < UsiCount; i++) {
    // Compare Controller Addresses
    if (UsiData[i].ControllerAddress == Address) {
      return UsiData[i].UsiAddress;
    }
  }

  return 0;
}

EFI_STATUS
UsiSetMode (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN EFI_USI_MODE         Mode)
{
  UINT32 UsiMode;

  // Set Specified USI Mode
  switch (Mode) {
    case MODE_UART:
      UsiMode = UART_SW_CONF;
      break;

    case MODE_SPI:
      UsiMode = SPI_SW_CONF;
      break;

    case MODE_I2C:
      UsiMode = I2C_SW_CONF;
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  // Get USI Address
  EFI_PHYSICAL_ADDRESS UsiAddress = GetUsiAddress (Address);
  if (!UsiAddress) {
    return EFI_NOT_FOUND;
  }

  // Write new USI Mode
  MmioWrite32 (UsiAddress, UsiMode);

  return EFI_SUCCESS;
}

STATIC EFI_USI_PROTOCOL mUsi = {
  UsiGetControllerAddress,
  UsiSetMode
};

EFI_STATUS
EFIAPI
RegisterUsi (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get USI Data
  GetUsiData (&UsiData, &UsiCount);

  // Verify USI Count
  if (!UsiCount) {
    return EFI_UNSUPPORTED;
  }

  // Register USI Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiUsiProtocolGuid, EFI_NATIVE_INTERFACE, &mUsi);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register USI Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
