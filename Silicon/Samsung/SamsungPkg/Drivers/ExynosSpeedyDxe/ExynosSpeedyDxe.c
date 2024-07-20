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
#include <Library/ExynosSpeedyLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiExynosSpeedy.h>

STATIC
VOID
ResetSpeedyFifo (IN UINT32 SpeedyBase)
{
  UINT32 SpeedyFifoCtrl;

  // Set SpeedyCtrl Config
  SpeedyFifoCtrl  = MmioRead32 (SpeedyBase + SPEEDY_FIFO_CTRL);
  SpeedyFifoCtrl |= SPEEDY_FIFO_RESET;

  // Write SpeedyCtrl Config
  MmioWrite32 (SpeedyBase + SPEEDY_FIFO_CTRL, SpeedyFifoCtrl);

  // Wait until Speedy Fifo Resets
  MicroSecondDelay (10);
}

STATIC
VOID
SetSpeedyCmd (
  UINT32 SpeedyBase,
  INT32  Direction,
  UINT16 Address,
  INT32  Random,
  INT32  BurstLength)
{
  UINT32 SpeedyFifoCtrl = 0;
  UINT32 SpeedyIntEn    = 0;
  UINT32 SpeedyCommand  = 0;

  // Reset Speedy Fifo
  ResetSpeedyFifo (SpeedyBase);

  // Read Speedy Fifo Ctrl
  SpeedyFifoCtrl = MmioRead32 (SpeedyBase + SPEEDY_FIFO_CTRL);

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
      DEBUG ((EFI_D_ERROR, "Invalid Access Mode!\n"));
      break;
  }

  // Write Speedy Fifo Ctrl
  MmioWrite32 (SpeedyBase + SPEEDY_FIFO_CTRL, SpeedyFifoCtrl);

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
      DEBUG ((EFI_D_ERROR, "Invalid Write Mode!\n"));
      break;
  }

  // Configure Speedy
  MmioWrite32 (SpeedyBase + SPEEDY_INT_STATUS, 0xFFFFFFFF);
  MmioWrite32 (SpeedyBase + SPEEDY_INT_ENABLE, SpeedyIntEn);
  MmioWrite32 (SpeedyBase + SPEEDY_CMD, SpeedyCommand);
}

STATIC
EFI_STATUS
WaitSpeedyTransfer (UINT32 SpeedyBase)
{
  EFI_STATUS Status          = EFI_SUCCESS;
  UINT32     SpeedyIntStatus = 0;
  UINT64     Timeout         = 1000;

  // Loop until Timeout is Reached
  while (Timeout-- > 0) {
    // Read Speedy Status
    SpeedyIntStatus = MmioRead32 (SpeedyBase + SPEEDY_INT_STATUS);

    if (SpeedyIntStatus & SPEEDY_TRANSFER_DONE) {
      break;
    }
  }

  if (Timeout == 0) {
    Status = EFI_TIMEOUT;
  }

  // Reset Interrupt State
  MmioWrite32 (SpeedyBase + SPEEDY_INT_STATUS, 0xFFFFFFFF);

  return Status;
}

VOID
ReadSpeedy (
  IN  UINT32 SpeedyBase,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  OUT UINT8 *Data)
{
  EFI_STATUS Status;
  UINT16     Address;

  // Set Address
  Address = Addr + ((Slave & 0xF) << 8);

  // Set Speedy Cmd
  SetSpeedyCmd (SpeedyBase, DIRECTION_READ, Address, ACCESS_RANDOM, 1);

  // Check if Speedy Read is Done
  Status = WaitSpeedyTransfer (SpeedyBase);
  if (!EFI_ERROR (Status)) {
    *Data = (UINT8)MmioRead32 (SpeedyBase + SPEEDY_RX_DATA);
  } else {
    DEBUG ((EFI_D_ERROR, "Timeout at Speedy Read!\n"));
  }
}

VOID
BurstReadSpeedy (
  IN  UINT32 SpeedyBase,
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
  SetSpeedyCmd (SpeedyBase, DIRECTION_READ, Address, ACCESS_BURST, Count);

  // Check if Speedy Read is Done
  Status = WaitSpeedyTransfer (SpeedyBase);
  if (!EFI_ERROR (Status)) {
    for (INT32 i = 0; i < Count; i++) {
      Data[i] = (UINT8)MmioRead32 (SpeedyBase + SPEEDY_RX_DATA);
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Timeout in Speedy Burst Read!\n"));
  }
}

VOID
WriteSpeedy (
  IN UINT32 SpeedyBase,
  IN UINT8  Slave,
  IN UINT8  Addr,
  IN UINT8  Data)
{
  EFI_STATUS Status;
  UINT16     Address;

  // Set Address
  Address = Addr + ((Slave & 0xF) << 8);

  // Set Speedy Cmd
  SetSpeedyCmd (SpeedyBase, DIRECTION_WRITE, Address, ACCESS_RANDOM, 1);

  // Write Speedy Data
  MmioWrite32 (SpeedyBase + SPEEDY_TX_DATA, Data);

  // Check if Speedy Write is Done
  Status = WaitSpeedyTransfer (SpeedyBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Timeout in Speedy Write!\n"));
  }
}

VOID
BurstWriteSpeedy (
  IN UINT32 SpeedyBase,
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
  SetSpeedyCmd (SpeedyBase, DIRECTION_WRITE, Address, ACCESS_BURST, Count);

  // Write Speedy Data
  for (INT32 i = 0; i < Count; i++) {
    MmioWrite32 (SpeedyBase + SPEEDY_TX_DATA, Data[i]);
  }

  // Check if Speedy Write is Done
  Status = WaitSpeedyTransfer (SpeedyBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Timeout in Speedy Burst Write!\n"));
  }
}

STATIC EFI_EXYNOS_SPEEDY_PROTOCOL mExynosSpeedy = {
  ResetSpeedyFifo,
  ReadSpeedy,
  BurstReadSpeedy,
  WriteSpeedy,
  BurstWriteSpeedy
};

EFI_STATUS
EFIAPI
InitSpeedyDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Execute SoC Specific Speedy Init
  InitSpeedy ();

  // Install Exynos Speedy Protocol in a new Handle
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiExynosSpeedyProtocolGuid, &mExynosSpeedy, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Exynos Speedy Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
