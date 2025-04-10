/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiSpeedy.h>

#include "Speedy.h"

VOID
ResetFifo (IN UINT32 BaseAddress)
{
  UINT32 SpeedyFifoCtrl;

  // Set SpeedyCtrl Config
  SpeedyFifoCtrl  = MmioRead32 (BaseAddress + SPEEDY_FIFO_CTRL);
  SpeedyFifoCtrl |= SPEEDY_FIFO_RESET;

  // Write SpeedyCtrl Config
  MmioWrite32 (BaseAddress + SPEEDY_FIFO_CTRL, SpeedyFifoCtrl);

  // Wait until Speedy Fifo Resets
  MicroSecondDelay (10);
}

EFI_STATUS
SetCmd (
  UINT32 BaseAddress,
  INT32  Direction,
  UINT16 Address,
  INT32  Random,
  INT32  BurstLength)
{
  UINT32 SpeedyFifoCtrl = 0;
  UINT32 SpeedyIntEn    = 0;
  UINT32 SpeedyCommand  = 0;

  // Reset Speedy Fifo
  ResetFifo (BaseAddress);

  // Read Speedy Fifo Ctrl
  SpeedyFifoCtrl = MmioRead32 (BaseAddress + SPEEDY_FIFO_CTRL);

  // Set Speedy Command
  SpeedyCommand |= SPEEDY_ADDRESS (Address);

  switch (Random) {
    case ACCESS_RANDOM:
      // Set Access Random Config
      SpeedyCommand  |= SPEEDY_ACCESS_RANDOM;
      SpeedyFifoCtrl |= (SPEEDY_RX_TRIGGER_LEVEL (1) | SPEEDY_TX_TRIGGER_LEVEL (1));
      break;
    
    case ACCESS_BURST:
      // Set Access Burst Config
      SpeedyCommand  |= (SPEEDY_ACCESS_BURST | SPEEDY_BURST_INCR | SPEEDY_BURST_LENGTH (BurstLength - 1));
      SpeedyFifoCtrl |= (SPEEDY_RX_TRIGGER_LEVEL (BurstLength) | SPEEDY_TX_TRIGGER_LEVEL (1));
      break;
    
    default:
      DEBUG ((EFI_D_ERROR, "%a: Invalid Access Mode!\n", __FUNCTION__));
      return EFI_INVALID_PARAMETER;
  }

  // Write Speedy Fifo Ctrl
  MmioWrite32 (BaseAddress + SPEEDY_FIFO_CTRL, SpeedyFifoCtrl);

  // Set SpeedyIntEn Config
  SpeedyIntEn |= (SPEEDY_TIMEOUT_CMD_EN | SPEEDY_TIMEOUT_STANDBY_EN | SPEEDY_TIMEOUT_DATA_EN);

  switch (Direction) {
    case DIRECTION_READ:
      // Set Direction Read Config
      SpeedyCommand |= SPEEDY_DIRECTION_READ;
      SpeedyIntEn   |= (SPEEDY_FIFO_RX_ALMOST_FULL_EN | SPEEDY_RX_FIFO_INT_TRAILER_EN | SPEEDY_RX_MODEBIT_ERR_EN | SPEEDY_RX_GLITCH_ERR_EN | SPEEDY_RX_ENDBIT_ERR_EN | SPEEDY_REMOTE_RESET_REQ_EN);
      break;
    
    case DIRECTION_WRITE:
      // Set Direction Write Config
      SpeedyCommand |= SPEEDY_DIRECTION_WRITE;
      SpeedyIntEn   |= (SPEEDY_TRANSFER_DONE_EN | SPEEDY_FIFO_TX_ALMOST_EMPTY_EN | SPEEDY_TX_LINE_BUSY_ERR_EN | SPEEDY_TX_STOPBIT_ERR_EN | SPEEDY_REMOTE_RESET_REQ_EN);
      break;
    
    default:
      DEBUG ((EFI_D_ERROR, "%a: Invalid Write Mode!\n", __FUNCTION__));
      return EFI_INVALID_PARAMETER;
  }

  // Configure Speedy
  MmioWrite32 (BaseAddress + SPEEDY_INT_STATUS, 0xFFFFFFFF);
  MmioWrite32 (BaseAddress + SPEEDY_INT_ENABLE, SpeedyIntEn);
  MmioWrite32 (BaseAddress + SPEEDY_CMD, SpeedyCommand);

  return EFI_SUCCESS;
}

EFI_STATUS
WaitTransfer (UINT32 BaseAddress)
{
  EFI_STATUS Status          = EFI_SUCCESS;
  UINT32     SpeedyIntStatus = 0;
  UINT64     Timeout         = 1000;

  // Loop until Timeout is Reached
  while (Timeout-- > 0) {
    // Read Speedy Status
    SpeedyIntStatus = MmioRead32 (BaseAddress + SPEEDY_INT_STATUS);

    // Exit Loop
    if (SpeedyIntStatus & SPEEDY_TRANSFER_DONE) {
      break;
    }
  }

  // Set Error
  if (Timeout == 0) {
    Status = EFI_TIMEOUT;
  }

  // Reset Interrupt State
  MmioWrite32 (BaseAddress + SPEEDY_INT_STATUS, 0xFFFFFFFF);

  return Status;
}

EFI_STATUS
Read (
  IN  UINT32 BaseAddress,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  OUT UINT8 *Data)
{
  EFI_STATUS Status;
  UINT16     Address;

  // Set Address
  Address = Addr + ((Slave & 0xF) << 8);

  // Set Speedy Cmd
  Status = SetCmd (BaseAddress, DIRECTION_READ, Address, ACCESS_RANDOM, 1);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reconfigure Speedy! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Check if Speedy Read is Done
  Status = WaitTransfer (BaseAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Timeout at Speedy Read!\n", __FUNCTION__));
    return Status;
  }

  // Pass Data
  *Data = (UINT8)MmioRead32 (BaseAddress + SPEEDY_RX_DATA);

  return EFI_SUCCESS;
}

EFI_STATUS
BurstRead (
  IN  UINT32 BaseAddress,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  OUT UINT8 *Data,
  IN  UINT8  Count)
{
  EFI_STATUS Status;
  UINT16     Address;

  // Set Address
  Address = Addr + ((Slave & 0xF) << 8);

  // Set Speedy Cmd
  Status = SetCmd (BaseAddress, DIRECTION_READ, Address, ACCESS_BURST, Count);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reconfigure Speedy! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Check if Speedy Read is Done
  Status = WaitTransfer (BaseAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Timeout in Speedy Burst Read! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Data
  for (INT32 i = 0; i < Count; i++) {
    Data[i] = (UINT8)MmioRead32 (BaseAddress + SPEEDY_RX_DATA);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
Write (
  IN UINT32 BaseAddress,
  IN UINT8  Slave,
  IN UINT8  Addr,
  IN UINT8  Data)
{
  EFI_STATUS Status;
  UINT16     Address;

  // Set Address
  Address = Addr + ((Slave & 0xF) << 8);

  // Set Speedy Cmd
  Status = SetCmd (BaseAddress, DIRECTION_WRITE, Address, ACCESS_RANDOM, 1);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reconfigure Speedy! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Write Speedy Data
  MmioWrite32 (BaseAddress + SPEEDY_TX_DATA, Data);

  // Check if Speedy Write is Done
  Status = WaitTransfer (BaseAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Timeout in Speedy Write!\n", __FUNCTION__));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
BurstWrite (
  IN UINT32 BaseAddress,
  IN UINT8  Slave,
  IN UINT8  Addr,
  IN UINT8 *Data,
  IN UINT8  Count)
{
  EFI_STATUS Status;
  UINT16     Address;

  // Set Address
  Address = Addr + ((Slave & 0xF) << 8);

  // Set Speedy Cmd
  Status = SetCmd (BaseAddress, DIRECTION_WRITE, Address, ACCESS_BURST, Count);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reconfigure Speedy! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Write Speedy Data
  for (INT32 i = 0; i < Count; i++) {
    MmioWrite32 (BaseAddress + SPEEDY_TX_DATA, Data[i]);
  }

  // Check if Speedy Write is Done
  Status = WaitTransfer (BaseAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Timeout in Speedy Burst Write! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC EFI_SPEEDY_PROTOCOL mSpeedy = {
  ResetFifo,
  Read,
  BurstRead,
  Write,
  BurstWrite
};

VOID
SWResetSpeedy (IN UINT32 BaseAddress)
{
  UINT32 SpeedyCtrl;

  // Set SpeedyCtrl Config
  SpeedyCtrl  = MmioRead32 (BaseAddress + SPEEDY_CTRL);
  SpeedyCtrl |= SPEEDY_SW_RST;

  // Write Speedy Config
  MmioWrite32 (BaseAddress + SPEEDY_CTRL, SpeedyCtrl);

  // Wait a Bit
  MicroSecondDelay (10);
}

VOID
EnableSpeedy (IN UINT32 BaseAddress)
{
  UINT32 SpeedyCtrl;

  // Set SpeedyCtrl Config
  SpeedyCtrl  = MmioRead32 (BaseAddress + SPEEDY_CTRL);
  SpeedyCtrl |= SPEEDY_ENABLE;

  // Write SpeedyCtrl Config
  MmioWrite32 (BaseAddress + SPEEDY_CTRL, SpeedyCtrl);
}

EFI_STATUS
EFIAPI
InitSpeedy (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX SpeedyRegion;

  // Locate Speedy Memory Region
  Status = LocateMemoryMapAreaByName ("Speedy", &SpeedyRegion);
  if (!EFI_ERROR (Status)) {
    // Clear Speedy Interrupt States
    MmioWrite32 (SpeedyRegion.Address + SPEEDY_INT_STATUS, 0xFFFFFFFF);

    // Configure Speedy
    SWResetSpeedy (SpeedyRegion.Address);
    EnableSpeedy  (SpeedyRegion.Address);

    DEBUG ((EFI_D_WARN, "Sucessfully Configured Speedy\n"));
  } else {
    CHAR8 SpeedyName[9] = "";

    // Clear Message
    ZeroMem (SpeedyName, 9);

    // Go thru each Register
    for (UINT8 i = 1; i < 9; i++) {
      // Append Number
      AsciiSPrint (SpeedyName, sizeof (SpeedyName), "Speedy-%u", i);

      // Locate Speedy Memory Region
      Status = LocateMemoryMapAreaByName ((CHAR8 *)SpeedyName, &SpeedyRegion);
      if (EFI_ERROR (Status)) {
        if (i == 1) {
          DEBUG ((EFI_D_WARN, "This Platform does not have Speedy\n"));
          return EFI_SUCCESS;
        }

        break;
      }

      // Clear Speedy Interrupt States
      MmioWrite32 (SpeedyRegion.Address + SPEEDY_INT_STATUS, 0xFFFFFFFF);

      // Configure Speedy
      SWResetSpeedy (SpeedyRegion.Address);
      EnableSpeedy  (SpeedyRegion.Address);

      DEBUG ((EFI_D_WARN, "Sucessfully Configured Speedy %u\n", i));
    }
  }

  // Install Speedy Protocol in a new Handle
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiSpeedyProtocolGuid, &mSpeedy, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Speedy Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
