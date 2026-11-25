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
EFIAPI
UsiGetControllerAddr (
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
      case BUS_UART:
        PrivateBusNumber = UsiData[i].UartBus;
        break;

      case BUS_SPI:
        PrivateBusNumber = UsiData[i].SpiBus;
        break;

      case BUS_I2C:
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

STATIC
EFI_USI_DATA *
GetUsiDataByController (IN EFI_PHYSICAL_ADDRESS Address)
{
  // Go thru each USI
  for (UINT8 i = 0; i < UsiCount; i++) {
    // Compare Controller Addresses
    if (UsiData[i].ControllerAddress == Address) {
      return &UsiData[i];
    }
  }

  return NULL;
}

STATIC
BOOLEAN
IsModeSupported (
  IN EFI_USI_DATA *Data,
  IN EFI_USI_MODE  Mode)
{
  switch (Mode) {
    case MODE_UART:
      return Data->UartBus != MAX_UINT8;

    case MODE_SPI:
      return Data->SpiBus != MAX_UINT8;

    case MODE_I2C:
      return Data->I2cBus != MAX_UINT8;

    default:
      return FALSE;
  }
}

STATIC
VOID
WriteUsiMode (
  IN EFI_USI_DATA *Data,
  IN UINT32        Mode)
{
  UINT32 Mask;

  Mask = Data->Version == UsiVersion1 ? USI_V1_SW_CONF_MASK : USI_V2_SW_CONF_MASK;
  MmioAndThenOr32 (Data->UsiAddress, ~Mask, Mode);
}

STATIC
UINT32
GetUsiActiveMode (IN EFI_USI_DATA *Data)
{
  for (UINT8 i = 0; i < UsiCount; i++) {
    if (UsiData[i].UsiAddress == Data->UsiAddress) {
      return UsiData[i].ActiveMode;
    }
  }

  return USI_V1_NONE_SW_CONF;
}

STATIC
VOID
SetUsiActiveMode (
  IN EFI_USI_DATA *Data,
  IN UINT32        Mode)
{
  for (UINT8 i = 0; i < UsiCount; i++) {
    if (UsiData[i].UsiAddress == Data->UsiAddress) {
      UsiData[i].ActiveMode = Mode;
    }
  }
}

STATIC
EFI_STATUS
GetUsiV1Mode (
  IN  EFI_USI_DATA *Data,
  IN  EFI_USI_MODE  Mode,
  OUT UINT32       *UsiMode)
{
  UINT32 ActiveMode;
  UINT32 RequestedMode;

  if (!IsModeSupported (Data, Mode)) {
    return EFI_UNSUPPORTED;
  }

  switch (Mode) {
    case MODE_UART:
      RequestedMode = USI_V1_UART_SW_CONF;
      break;

    case MODE_SPI:
      RequestedMode = USI_V1_SPI_SW_CONF;
      break;

    case MODE_I2C:
      RequestedMode = Data->Channel == UsiChannel1 ? USI_V1_I2C1_SW_CONF : USI_V1_I2C0_SW_CONF;
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  ActiveMode = GetUsiActiveMode (Data);
  if ((ActiveMode == USI_V1_NONE_SW_CONF) || (ActiveMode == RequestedMode)) {
    *UsiMode = RequestedMode;
    return EFI_SUCCESS;
  }

  if (((ActiveMode == USI_V1_I2C0_SW_CONF) && (RequestedMode == USI_V1_I2C1_SW_CONF)) ||
      ((ActiveMode == USI_V1_I2C1_SW_CONF) && (RequestedMode == USI_V1_I2C0_SW_CONF))) {
    *UsiMode = USI_V1_I2C0_I2C1_SW_CONF;
    return EFI_SUCCESS;
  }

  if (((ActiveMode == USI_V1_UART_SW_CONF) && (RequestedMode == USI_V1_I2C1_SW_CONF)) ||
      ((ActiveMode == USI_V1_I2C1_SW_CONF) && (RequestedMode == USI_V1_UART_SW_CONF))) {
    *UsiMode = USI_V1_UART_I2C1_SW_CONF;
    return EFI_SUCCESS;
  }

  if ((ActiveMode == USI_V1_I2C0_I2C1_SW_CONF) &&
      ((RequestedMode == USI_V1_I2C0_SW_CONF) || (RequestedMode == USI_V1_I2C1_SW_CONF))) {
    *UsiMode = ActiveMode;
    return EFI_SUCCESS;
  }

  if ((ActiveMode == USI_V1_UART_I2C1_SW_CONF) &&
      ((RequestedMode == USI_V1_UART_SW_CONF) || (RequestedMode == USI_V1_I2C1_SW_CONF))) {
    *UsiMode = ActiveMode;
    return EFI_SUCCESS;
  }

  return EFI_ACCESS_DENIED;
}

EFI_STATUS
EFIAPI
UsiSetMode (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN EFI_USI_MODE         Mode)
{
  EFI_USI_DATA *Data;
  EFI_STATUS    Status;
  UINT32        UsiMode;

  if ((Mode < MODE_UART) || (Mode > MODE_I2C)) {
    return EFI_INVALID_PARAMETER;
  }

  // Get USI Data
  Data = GetUsiDataByController (Address);
  if (Data == NULL) {
    return EFI_NOT_FOUND;
  }

  // Dedicated controllers do not require a USI mode selection.
  if (Data->Version == UsiVersionNone) {
    return IsModeSupported (Data, Mode) ? EFI_SUCCESS : EFI_UNSUPPORTED;
  }

  // USIv1 can enable both I2C channels, or UART and I2C1, at the same time.
  if (Data->Version == UsiVersion1) {
    Status = GetUsiV1Mode (Data, Mode, &UsiMode);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    WriteUsiMode (Data, UsiMode);
    SetUsiActiveMode (Data, UsiMode);
    return EFI_SUCCESS;
  }

  // Set Specified USI Mode
  switch (Mode) {
    case MODE_UART:
      UsiMode = USI_V2_UART_SW_CONF;
      break;

    case MODE_SPI:
      UsiMode = USI_V2_SPI_SW_CONF;
      break;

    case MODE_I2C:
      UsiMode = USI_V2_I2C_SW_CONF;
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  // Write new USI Mode
  WriteUsiMode (Data, UsiMode);

  // USIv2 has a USI register block at the end of the controller register map.
  // Clear USI_CON to release the selected controller from reset.
  MmioWrite32 (Address + USI_V2_CON_OFFSET, 0x0);

  return EFI_SUCCESS;
}

STATIC EFI_USI_PROTOCOL pUsi = {
  UsiGetControllerAddr,
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
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiUsiProtocolGuid, EFI_NATIVE_INTERFACE, &pUsi);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register USI Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
