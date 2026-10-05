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
STATIC EFI_SPEEDY_BUS *mBus[SPEEDY_BUS_COUNT] = { NULL };

STATIC
VOID
ConfigureBusFifo (
  IN UINT8 BusNumber,
  IN UINT8 BurstLength)
{
  UINT32 FifoCtrlCfg;

  // Reset Bus FIFO Controller
  MmioOr32 ((UINTN)&mBus[BusNumber]->fifo_ctrl, SPEEDY_FIFO_RESET);

  // Wait 10us
  gBS->Stall (10);

  // Get Current FIFO Configuration
  FifoCtrlCfg = MmioRead32 ((UINTN)&mBus[BusNumber]->fifo_ctrl);

  // Set new FIFO Configuration
  if (BurstLength > 1) {
    FifoCtrlCfg |= (SPEEDY_RX_TRIGGER_LEVEL (BurstLength) | SPEEDY_TX_TRIGGER_LEVEL (1));
  } else {
    FifoCtrlCfg |= (SPEEDY_RX_TRIGGER_LEVEL (1) | SPEEDY_TX_TRIGGER_LEVEL (1));
  }

  // Write new FIFO Configuration
  MmioWrite32 ((UINTN)&mBus[BusNumber]->fifo_ctrl, FifoCtrlCfg);
}

STATIC
UINT32
GetBusInterruptMask (IN BOOLEAN IsRead)
{
  // Set Default Interrupts
  UINT32 Interrupt = (SPEEDY_TIMEOUT_CMD_EN | SPEEDY_TIMEOUT_STANDBY_EN | SPEEDY_TIMEOUT_DATA_EN);

  // Append Read/Write Interrupts
  if (IsRead) {
    Interrupt |= (
      SPEEDY_FIFO_RX_ALMOST_FULL_EN | SPEEDY_RX_FIFO_INT_TRAILER_EN |
      SPEEDY_RX_MODEBIT_ERR_EN      | SPEEDY_RX_GLITCH_ERR_EN       |
      SPEEDY_RX_ENDBIT_ERR_EN       | SPEEDY_REMOTE_RESET_REQ_EN
    );
  } else {
    Interrupt |= (
      SPEEDY_TRANSFER_DONE_EN    | SPEEDY_FIFO_TX_ALMOST_EMPTY_EN |
      SPEEDY_TX_LINE_BUSY_ERR_EN | SPEEDY_TX_STOPBIT_ERR_EN       |
      SPEEDY_REMOTE_RESET_REQ_EN
    );
  }

  return Interrupt;
}

STATIC
UINT32
GetBusCommand (
  IN UINT16  Address,
  IN UINT8   BurstLength,
  IN BOOLEAN IsWrite)
{
  // Set Inital Command
  UINT32 Command = SPEEDY_ADDRESS (Address);

  // Append Burst Arguments
  if (BurstLength > 1) {
    Command |= (SPEEDY_BURST_INCR | SPEEDY_BURST_LENGTH (BurstLength - 1));
  } else {
    Command |= SPEEDY_ACCESS_RANDOM;
  }

  // Append Write Argument
  if (IsWrite) {
    Command |= SPEEDY_DIRECTION_WRITE;
  }

  return Command;
}

VOID
SetBusCommand (
  IN UINT8   BusNumber,
  IN UINT16  Address,
  IN UINT8   BurstLength,
  IN BOOLEAN IsRead)
{
  // Configure Bus FIFO
  ConfigureBusFifo (BusNumber, BurstLength);

  // Get Bus Interrupt Mask
  UINT32 InterruptMask = GetBusInterruptMask (IsRead);

  // Get Bus Command
  UINT32 Command = GetBusCommand (Address, BurstLength, !IsRead);

  // Clear Bus Interrupt Status
  MmioWrite32 ((UINTN)&mBus[BusNumber]->int_status, MAX_UINT32);
  MmioWrite32 ((UINTN)&mBus[BusNumber]->int_enable, InterruptMask);

  // Send Bus Command
  MmioWrite32 ((UINTN)&mBus[BusNumber]->cmd, Command);
}

EFI_STATUS
VerifyBus (IN UINT8 BusNumber)
{
  // Verify Bus Number
  if (BusNumber >= SPEEDY_BUS_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Bus Init State
  if (mBus[BusNumber] == NULL) {
    return EFI_NOT_READY;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetBusStatus (IN UINT8 BusNumber)
{
  // Set Completion Masks
  STATIC CONST UINT32 CompletionMask = (
    SPEEDY_TRANSFER_DONE    | SPEEDY_RX_MODEBIT_ERR |
    SPEEDY_RX_GLITCH_ERR    | SPEEDY_RX_ENDBIT_ERR  |
    SPEEDY_TX_LINE_BUSY_ERR | SPEEDY_TX_STOPBIT_ERR
    );

  // Set 1000 Tries
  for (UINT16 Timeout = 1000; Timeout > 0; Timeout--) {
    // Get current Bus Interrupt Status
    UINT32 InterruptStatus = MmioRead32 ((UINTN)&mBus[BusNumber]->int_status);

    // Check Bus Interrupt Status
    if ((InterruptStatus & CompletionMask) != 0) {
      // Clear Bus Interrupt Status
      MmioWrite32 ((UINTN)&mBus[BusNumber]->int_status, MAX_UINT32);

      // Check for Protocol Error
      if (InterruptStatus & (SPEEDY_RX_MODEBIT_ERR | SPEEDY_RX_ENDBIT_ERR | SPEEDY_TX_STOPBIT_ERR)) {
        return EFI_PROTOCOL_ERROR;
      }

      // Check for CRC Error
      if (InterruptStatus & SPEEDY_RX_GLITCH_ERR) {
        return EFI_CRC_ERROR;
      }

      // Check for Busy Error
      if (InterruptStatus & SPEEDY_TX_LINE_BUSY_ERR) {
        return EFI_ABORTED;
      }

      // Check for Success
      if (InterruptStatus & SPEEDY_TRANSFER_DONE) {
        return EFI_SUCCESS;
      }

      return EFI_DEVICE_ERROR;
    }

    // Wait 10us
    gBS->Stall (10);
  }

  return EFI_TIMEOUT;
}

EFI_STATUS
EFIAPI
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

  // Verify Bus State
  Status = VerifyBus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set Bus Command
  SetBusCommand (BusNumber, Address, 1, TRUE);

  // Get Bus Status
  Status = GetBusStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  *Data = (UINT8)MmioRead32 ((UINTN)&mBus[BusNumber]->rx_data);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SpeedyWrite (
  IN UINT8 BusNumber,
  IN UINT8 Slave,
  IN UINT8 SlaveAddress,
  IN UINT8 Data)
{
  EFI_STATUS Status;

  // Verify Bus State
  Status = VerifyBus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set Bus Command
  SetBusCommand (BusNumber, Address, 1, FALSE);

  // Write new Data
  MmioWrite32 ((UINTN)&mBus[BusNumber]->tx_data, Data);

  // Get Bus Status
  Status = GetBusStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SpeedyBurstRead (
  IN  UINT8  BusNumber,
  IN  UINT8  Slave,
  IN  UINT8  SlaveAddress,
  IN  UINT8  DataCount,
  OUT UINT8 *Data)
{
  EFI_STATUS Status;

  // Verify Parameters
  if (Data == NULL || DataCount <= 1) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify Bus State
  Status = VerifyBus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set Bus Command
  SetBusCommand (BusNumber, Address, DataCount, TRUE);

  // Get Bus Status
  Status = GetBusStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  for (UINT8 i = 0; i < DataCount; i++) {
    Data[i] = (UINT8)MmioRead32 ((UINTN)&mBus[BusNumber]->rx_data);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SpeedyBurstWrite (
  IN UINT8  BusNumber,
  IN UINT8  Slave,
  IN UINT8  SlaveAddress,
  IN UINT8  DataCount,
  IN UINT8 *Data)
{
  EFI_STATUS Status;

  // Verify Parameters
  if (Data == NULL || DataCount <= 1) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify Bus State
  Status = VerifyBus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set new Target Address
  UINT16 Address = SPEEDY_SLAVE_ADDRESS (Slave, SlaveAddress);

  // Set Bus Command
  SetBusCommand (BusNumber, Address, DataCount, FALSE);

  // Write new Data
  for (UINT8 i = 0; i < DataCount; i++) {
    MmioWrite32 ((UINTN)&mBus[BusNumber]->tx_data, Data[i]);
  }

  // Get Bus Status
  Status = GetBusStatus (BusNumber);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC EFI_SPEEDY_PROTOCOL pSpeedy = {
  SpeedyRead,
  SpeedyWrite,
  SpeedyBurstRead,
  SpeedyBurstWrite
};

VOID
InitBus (IN UINT8 BusNumber)
{
  // Reset Bus
  MmioWrite32 ((UINTN)&mBus[BusNumber]->int_status, MAX_UINT32);
  MmioOr32    ((UINTN)&mBus[BusNumber]->ctrl,       SPEEDY_SW_RST);

  // Wait 10us
  gBS->Stall (10);

  // Enable Bus
  MmioOr32 ((UINTN)&mBus[BusNumber]->ctrl, SPEEDY_ENABLE);
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
  mBus[BusNumber] = (EFI_SPEEDY_BUS *)Address;

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
    return EFI_UNSUPPORTED;
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
  }

  // Register SPEEDY Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiSpeedyProtocolGuid, &pSpeedy, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register SPEEDY Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
