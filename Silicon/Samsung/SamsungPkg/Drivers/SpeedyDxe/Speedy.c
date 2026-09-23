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
#include <Library/IoLib.h>

#include <Protocol/EFISpeedy.h>

#include "Speedy.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_BUS *Bus[SPEEDY_BUS_COUNT] = { NULL };

VOID
SetSpeedyCommand (
  IN UINT8   BusNumber,
  IN UINT16  Address,
  IN UINT8   BurstLength,
  IN BOOLEAN IsBurst,
  IN BOOLEAN IsRead)
{
  UINT32 FifoController = 0;
  UINT32 Interrupt      = 0;
  UINT32 Command        = 0;

  // Reset SPEEDY FIFO Controller
  MmioOr32 ((UINTN)&Bus[BusNumber]->fifo_ctrl, SPEEDY_FIFO_RESET);

  // Get current SPEEDY FIFO Controller Configuration
  FifoController = MmioRead32 ((UINTN)&Bus[BusNumber]->fifo_ctrl);

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
  MmioWrite32 ((UINTN)&Bus[BusNumber]->fifo_ctrl, FifoController);

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
  MmioWrite32 ((UINTN)&Bus[BusNumber]->int_status, MAX_UINT32);

  // Write new SPEEDY Interrupt
  MmioWrite32 ((UINTN)&Bus[BusNumber]->int_enable, Interrupt);

  // Write new SPEEDY Command
  MmioWrite32 ((UINTN)&Bus[BusNumber]->cmd, Command);
}

EFI_STATUS
GetSpeedyStatus (IN UINT8 BusNumber)
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
    UINT32 InterruptStatus = MmioRead32 ((UINTN)&Bus[BusNumber]->int_status);

    // Check SPEEDY Completion
    if (InterruptStatus & CompletionMask) {
      // Clear SPEEDY Interrupt Status
      MmioWrite32 ((UINTN)&Bus[BusNumber]->int_status, MAX_UINT32);

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

EFI_STATUS
SpeedyRead (
  IN  UINT8  BusNumber,
  IN  UINT8  Slave,
  IN  UINT8  SlaveAddress,
  OUT UINT8 *Data)
{
  EFI_STATUS Status;

  // Verify Data Parameter
  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify Bus Number
  if (BusNumber >= SPEEDY_BUS_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Bus Init State
  if (Bus[BusNumber] == NULL) {
    return EFI_NOT_READY;
  }

  // Set Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (BusNumber, Address, 1, FALSE, TRUE);

  // Get SPEEDY Status
  Status = GetSpeedyStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  *Data = (UINT8)MmioRead32 ((UINTN)&Bus[BusNumber]->rx_data);

  return EFI_SUCCESS;
}

EFI_STATUS
SpeedyWrite (
  IN UINT8 BusNumber,
  IN UINT8 Slave,
  IN UINT8 SlaveAddress,
  IN UINT8 Data)
{
  EFI_STATUS Status;

  // Verify Bus Number
  if (BusNumber >= SPEEDY_BUS_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Bus Init State
  if (Bus[BusNumber] == NULL) {
    return EFI_NOT_READY;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (BusNumber, Address, 1, FALSE, FALSE);

  // Write new Data
  MmioWrite32 ((UINTN)&Bus[BusNumber]->tx_data, Data);

  // Get SPEEDY Status
  Status = GetSpeedyStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

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
  EFI_STATUS Status;

  // Verify Parameters
  if (Data == NULL || !DataCount) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify Bus Number
  if (BusNumber >= SPEEDY_BUS_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Bus Init State
  if (Bus[BusNumber] == NULL) {
    return EFI_NOT_READY;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (BusNumber, Address, DataCount, TRUE, TRUE);

  // Get SPEEDY Status
  Status = GetSpeedyStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  for (UINT8 i = 0; i < DataCount; i++) {
    Data[i] = (UINT8)MmioRead32 ((UINTN)&Bus[BusNumber]->rx_data);
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
  EFI_STATUS Status;

  // Verify Parameters
  if (Data == NULL || !DataCount) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify Bus Number
  if (BusNumber >= SPEEDY_BUS_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Bus Init State
  if (Bus[BusNumber] == NULL) {
    return EFI_NOT_READY;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set SPEEDY Command
  SetSpeedyCommand (BusNumber, Address, DataCount, TRUE, FALSE);

  // Write new Data
  for (UINT8 i = 0; i < DataCount; i++) {
    MmioWrite32 ((UINTN)&Bus[BusNumber]->tx_data, Data[i]);
  }

  // Get SPEEDY Status
  Status = GetSpeedyStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC EFI_SPEEDY_PROTOCOL mSpeedy = {
  SpeedyRead,
  SpeedyWrite,
  SpeedyBurstRead,
  SpeedyBurstWrite
};

VOID
InitBus (IN UINT8 BusNumber)
{
  // Clear Interrupt Status
  MmioWrite32 ((UINTN)&Bus[BusNumber]->int_status, MAX_UINT32);

  // Reset Controller
  MmioOr32 ((UINTN)&Bus[BusNumber]->ctrl, SPEEDY_SW_RST);

  // Wait 10us
  gBS->Stall (10);

  // Enable Bus
  MmioOr32 ((UINTN)&Bus[BusNumber]->ctrl, SPEEDY_ENABLE);
}

EFI_STATUS
MapBusMemory (
  IN UINT8                BusNumber,
  IN EFI_PHYSICAL_ADDRESS Address)
{
  EFI_STATUS Status;

  // Map Bus Memory
  Status = MapMemoryRegion (Address, SPEEDY_MMIO_LENGTH, EfiMemoryMappedIO);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Populate Bus Structure
  Bus[BusNumber] = (EFI_SPEEDY_BUS *)Address;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitSpeedy (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Bus Addresses
  CONST UINT32 *BusAddress = (UINT32 *)FixedPcdGetPtr (PcdSpeedyBusAddr);
  if (BusAddress[0] == MAX_UINT32) {
    return EFI_NOT_FOUND;
  }

  // Go thru each Bus
  for (UINT8 i = 0; i < SPEEDY_BUS_COUNT; i++) {
    // Map Bus Memory
    Status = MapBusMemory (i, BusAddress[i]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Map Bus %u Memory (0x%llx)! Status = %r\n", i, BusAddress[i], Status));
      continue;
    }

    // Init Bus
    InitBus (i);

    // Show Progress
    DEBUG ((EFI_D_WARN, "Bus %u Initialized\n", i));
  }

  // Register SPEEDY Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiSpeedyProtocolGuid, &mSpeedy, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register SPEEDY Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
