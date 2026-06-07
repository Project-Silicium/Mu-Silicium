/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SpeedyLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFISpeedy.h>

#include "Speedy.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_BUS_DATA *BusData;
STATIC UINT8                BusCount;

VOID
ClearSpeedyInterrupt (IN EFI_SPEEDY_BUS *Bus)
{
  // Clear SPEEDY Interrupt Status
  MmioWrite32 ((UINTN)&Bus->int_status, MAX_UINT32);
}

VOID
SetSpeedyCommand (
  IN EFI_SPEEDY_BUS *Bus,
  IN UINT16          Address,
  IN UINT8           BurstLength,
  IN BOOLEAN         IsBurst,
  IN BOOLEAN         IsRead)
{
  UINT32 FifoController = 0;
  UINT32 Interrupt      = 0;
  UINT32 Command        = 0;

  // Reset SPEEDY FIFO Controller
  MmioOr32 ((UINTN)&Bus->fifo_ctrl, SPEEDY_FIFO_RESET);

  // Get current SPEEDY FIFO Controller Configuration
  FifoController = MmioRead32 ((UINTN)&Bus->fifo_ctrl);

  // Target SPEEDY Address
  Command |= SPEEDY_ADDRESS (Address);

  // Set Access Mode
  if (IsBurst) {
    Command        |= (SPEEDY_ACCESS_BURST | SPEEDY_BURST_INCR | SPEEDY_BURST_LENGTH (BurstLength - 1));
    FifoController |= (SPEEDY_RX_TRIGGER_LEVEL (BurstLength) | SPEEDY_TX_TRIGGER_LEVEL (1));
  } else {
    Command        |= SPEEDY_ACCESS_RANDOM;
    FifoController |= (SPEEDY_RX_TRIGGER_LEVEL (1) | SPEEDY_TX_TRIGGER_LEVEL (1));
  }

  // Write new SPEEDY FIFO Trigger Level
  MmioWrite32 ((UINTN)&Bus->fifo_ctrl, FifoController);

  // Set Inital SPEEDY Interrupt
  Interrupt |= (SPEEDY_TIMEOUT_CMD_EN | SPEEDY_TIMEOUT_STANDBY_EN | SPEEDY_TIMEOUT_DATA_EN);

  // Set Command Direction
  if (IsRead) {
    Command   |= SPEEDY_DIRECTION_READ;
    Interrupt |= (
      SPEEDY_FIFO_RX_ALMOST_FULL_EN |
      SPEEDY_RX_FIFO_INT_TRAILER_EN |
      SPEEDY_RX_MODEBIT_ERR_EN      |
      SPEEDY_RX_GLITCH_ERR_EN       |
      SPEEDY_RX_ENDBIT_ERR_EN       |
      SPEEDY_REMOTE_RESET_REQ_EN
    );
  } else {
    Command   |= SPEEDY_DIRECTION_WRITE;
    Interrupt |= (
      SPEEDY_TRANSFER_DONE_EN        |
      SPEEDY_FIFO_TX_ALMOST_EMPTY_EN |
      SPEEDY_TX_LINE_BUSY_ERR_EN     |
      SPEEDY_TX_STOPBIT_ERR_EN       |
      SPEEDY_REMOTE_RESET_REQ_EN
    );
  }

  // Clear SPEEDY Interrupt Status
  ClearSpeedyInterrupt (Bus);

  // Write new SPEEDY Interrupt
  MmioWrite32 ((UINTN)&Bus->int_enable, Interrupt);

  // Write new SPEEDY Command
  MmioWrite32 ((UINTN)&Bus->cmd, Command);
}

EFI_STATUS
GetSpeedyStatus (IN EFI_SPEEDY_BUS *Bus)
{
  // Set Completion Masks
  STATIC CONST UINT32 CompletionMask = (
    SPEEDY_TRANSFER_DONE    | SPEEDY_TIMEOUT_CMD      |
    SPEEDY_TIMEOUT_STANDBY  | SPEEDY_TIMEOUT_DATA     |
    SPEEDY_RX_MODEBIT_ERR   | SPEEDY_RX_GLITCH_ERR    |
    SPEEDY_RX_ENDBIT_ERR    | SPEEDY_TX_LINE_BUSY_ERR |
    SPEEDY_TX_STOPBIT_ERR   | SPEEDY_REMOTE_RESET_REQ_STAT
    );

  // Set 1000 Tries
  for (UINT16 Timeout = 1000; Timeout > 0; Timeout--) {
    // Get current SPEEDY Interrupt Status
    UINT32 InterruptStatus = MmioRead32 ((UINTN)&Bus->int_status);

    // Check SPEEDY Completion
    if (InterruptStatus & CompletionMask) {
      // Clear SPEEDY Interrupt Status
      ClearSpeedyInterrupt (Bus);

      // Translate SPEEDY Completion
      switch (InterruptStatus & CompletionMask) {
        case SPEEDY_TRANSFER_DONE:
          return EFI_SUCCESS;

        case SPEEDY_TIMEOUT_CMD:
        case SPEEDY_TIMEOUT_STANDBY:
        case SPEEDY_TIMEOUT_DATA:
          return EFI_TIMEOUT;

        case SPEEDY_RX_MODEBIT_ERR:
        case SPEEDY_RX_ENDBIT_ERR:
        case SPEEDY_TX_STOPBIT_ERR:
          return EFI_PROTOCOL_ERROR;

        case SPEEDY_RX_GLITCH_ERR:
          return EFI_CRC_ERROR;

        default:
          return EFI_DEVICE_ERROR;
      }
    }
  }

  return EFI_TIMEOUT;
}

EFI_SPEEDY_BUS*
GetSpeedyBus (IN UINT8 BusNumber)
{
  // Verify Bus Number
  if (BusNumber >= BusCount) {
    return NULL;
  }

  // Return SPEEDY Bus
  return (EFI_SPEEDY_BUS *)BusData[BusNumber].Address;
}

EFI_STATUS
SpeedyRead (
  IN  UINT8  BusNumber,
  IN  UINT8  Slave,
  IN  UINT8  SlaveAddress,
  OUT UINT8 *Data)
{
  EFI_STATUS      Status;
  EFI_SPEEDY_BUS *Bus;

  // Verify Data Parameter
  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get SPEEDY Bus
  Bus = GetSpeedyBus (BusNumber);
  if (Bus == NULL) {
    return EFI_NOT_FOUND;
  }

  // Set Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (Bus, Address, 1, FALSE, TRUE);

  // Get SPEEDY Status
  Status = GetSpeedyStatus (Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  *Data = (UINT8)MmioRead32 ((UINTN)&Bus->rx_data);

  return EFI_SUCCESS;
}

EFI_STATUS
SpeedyBurstRead (
  IN  UINT8  BusNumber,
  IN  UINT8  Slave,
  IN  UINT8  SlaveAddress,
  IN  UINT8  DataCount,
  OUT UINT8 *Data)
{
  EFI_STATUS      Status;
  EFI_SPEEDY_BUS *Bus;

  // Verify Parameters
  if (Data == NULL || !DataCount) {
    return EFI_INVALID_PARAMETER;
  }

  // Get SPEEDY Bus
  Bus = GetSpeedyBus (BusNumber);
  if (Bus == NULL) {
    return EFI_NOT_FOUND;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (Bus, Address, DataCount, TRUE, TRUE);

  // Get SPEEDY Status
  Status = GetSpeedyStatus (Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  for (UINT8 i = 0; i < DataCount; i++) {
    Data[i] = (UINT8)MmioRead32 ((UINTN)&Bus->rx_data);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SpeedyWrite (
  IN UINT8 BusNumber,
  IN UINT8 Slave,
  IN UINT8 SlaveAddress,
  IN UINT8 Data)
{
  EFI_STATUS      Status;
  EFI_SPEEDY_BUS *Bus;

  // Get SPEEDY Bus
  Bus = GetSpeedyBus (BusNumber);
  if (Bus == NULL) {
    return EFI_NOT_FOUND;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (Bus, Address, 1, FALSE, FALSE);

  // Write new Data
  MmioWrite32 ((UINTN)&Bus->tx_data, Data);

  // Get SPEEDY Status
  Status = GetSpeedyStatus (Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SpeedyBurstWrite (
  IN UINT8  BusNumber,
  IN UINT8  Slave,
  IN UINT8  SlaveAddress,
  IN UINT8  DataCount,
  IN UINT8 *Data)
{
  EFI_STATUS      Status;
  EFI_SPEEDY_BUS *Bus;

  // Verify Parameters
  if (Data == NULL || !DataCount) {
    return EFI_INVALID_PARAMETER;
  }

  // Get SPEEDY Bus
  Bus = GetSpeedyBus (BusNumber);
  if (Bus == NULL) {
    return EFI_NOT_FOUND;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (Bus, Address, DataCount, TRUE, FALSE);

  // Write new Data
  for (UINT8 i = 0; i < DataCount; i++) {
    MmioWrite32 ((UINTN)&Bus->tx_data, Data[i]);
  }

  // Get SPEEDY Status
  Status = GetSpeedyStatus (Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC EFI_SPEEDY_PROTOCOL mSpeedy = {
  SpeedyRead,
  SpeedyBurstRead,
  SpeedyWrite,
  SpeedyBurstWrite
};

VOID
InitSpeedyBus (IN EFI_SPEEDY_BUS *Bus)
{
  // Clear SPEEDY Interrupt Status
  ClearSpeedyInterrupt (Bus);

  // Reset SPEEDY Bus Controller
  MmioOr32 ((UINTN)&Bus->ctrl, SPEEDY_SW_RST);

  // Wait 10us
  gBS->Stall (10);

  // Enable SPEEDY Bus Controller
  MmioOr32 ((UINTN)&Bus->ctrl, SPEEDY_ENABLE);
}

EFI_STATUS
EFIAPI
InitSpeedy (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Speedy Bus Data
  GetSpeedyBusData (&BusData, &BusCount);

  // Verify Bus Count
  if (!BusCount) {
    return EFI_UNSUPPORTED;
  }

  // Go thru all SPEEDY Buses
  for (UINT8 i = 0; i < BusCount; i++) {
    // Get SPEEDY Bus Address
    EFI_PHYSICAL_ADDRESS BusAddress = BusData[i].Address;

    // Map SPEEDY Bus
    Status = MapMemoryRegion (BusAddress, SPEEDY_MMIO_LENGTH, EfiMemoryMappedIO);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Map SPEEDY Bus: 0x%llx!\n", BusAddress));
      return Status;
    }

    // Get SPEEDY Bus
    EFI_SPEEDY_BUS *Bus = (EFI_SPEEDY_BUS *)BusAddress;

    // Init SPEEDY Bus
    InitSpeedyBus (Bus);
  }

  // Register SPEEDY Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiSpeedyProtocolGuid, &mSpeedy, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register SPEEDY Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
