/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Uefi.h>

#include <Library/CacheMaintenanceLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DmaLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "MshcDxe.h"

extern UINTN           gMshcBaseAddr;
       UINTN           gMshcDmaBaseAddr  = 0;
       UINTN          *pgMshcDmaBaseAddr = NULL;
       VOID           *gDmaMapping       = NULL;

VOID
MshcResetFifo ()
{
  UINT32 Value;
  
  // Reset FIFO
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_CTRL);
  Value |= FIFO_RESET;
  MmioWrite32 (gMshcBaseAddr + MSHCI_CTRL, Value);
}

EFI_STATUS
MshcResetAll ()
{
  UINT32 Value;
  UINT32 Count = 10000;

  // Get Current MSHC Status
  while (MmioRead32 (gMshcBaseAddr + MSHCI_STATUS) & DATA_BUSY) {    
    if (Count == 0) {
      DEBUG ((EFI_D_ERROR, "%a: The Controller didn't Release in Time!\n", __FUNCTION__));
      return EFI_TIMEOUT;
    }
    
    Count--;
    MicroSecondDelay (1);
  }

  // Reset CIU
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_CTRL);
  Value |= CTRL_RESET;
  MmioWrite32 (gMshcBaseAddr + MSHCI_CTRL, Value);

  // Reset FIFO
  MshcResetFifo ();
 
  // Reset DMA
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_CTRL);
  Value |= DMA_RESET;
  MmioWrite32 (gMshcBaseAddr + MSHCI_CTRL, Value);

  // Set CMD Auto-stop
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_CTRL);
  Value |= SEND_AS_CCSD;
  MmioWrite32 (gMshcBaseAddr + MSHCI_CTRL, Value);
  
  return EFI_SUCCESS;
}

VOID
MshcSetDdr (IN UINT32 BusMode)
{
  UINT32 ClockPhase;

  switch (BusMode) {
    case BUSMODE_DDR:
      // Set the DDR Bus Mode
      MmioWrite32 (gMshcBaseAddr + MSHCI_UHS_REG, UHS_DDR);
      ClockPhase = 0x03040002;
      break;

    default:
      // Set the non-DDR Bus Mode
      MmioWrite32 (gMshcBaseAddr + MSHCI_UHS_REG, UHS_NON_DDR);
      ClockPhase = 0x03040000;
      break;
  }

  // Select the Defined Clock
  MmioWrite32 (gMshcBaseAddr + MSHCI_CLKSEL, ClockPhase);
}

EFI_STATUS
MshcInitialize ()
{
  EFI_STATUS Status;

  // Enable the MSHC Power
  MmioWrite32 (gMshcBaseAddr + MSHCI_PWREN, POWER_ENABLE);

  // Reset MSHC
  Status = MshcResetAll ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reset MSHC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Initialize FIFO threshold
  MmioWrite32 (gMshcBaseAddr + MSHCI_FIFOTH, MSIZE_16 | TX_WMARK_DEFAULT | RX_WMARK_DEFAULT);

  // Clear all Pending Interrupts
  MmioWrite32 (gMshcBaseAddr + MSHCI_RINTSTS, INTMSK_ALL);
  
  // Disable all Interrupts
  MmioWrite32 (gMshcBaseAddr + MSHCI_INTMSK, 0);

  // Set the Debounce Filter Value
  MmioWrite32 (gMshcBaseAddr + MSHCI_DEBNCE, 0xFFFFF);

  // Clear the Card Type
  MmioWrite32 (gMshcBaseAddr + MSHCI_CTYPE, 0x0);

  // Reset IDMA in BMOD
  MmioWrite32 (gMshcBaseAddr + MSHCI_BMOD, BMOD_IDMAC_RESET);
  
  // Disable DMA Interrupts
  MmioWrite32 (gMshcBaseAddr + MSHCI_IDINTEN, 0x0);

  // Set max Timeout
  MmioWrite32 (gMshcBaseAddr + MSHCI_TMOUT, 0xFFFFFFFF);

  // Set the Block Size
  MmioWrite32 (gMshcBaseAddr + MSHCI_BLKSIZ, BLEN_512BYTES);
  
  // Set the SDR Card Mode
  MshcSetDdr (BUSMODE_NON_DDR);

  // Clear CLKSRC
  MmioWrite32 (gMshcBaseAddr + MSHCI_CLKSRC, 0x0);

  return EFI_SUCCESS;
}

EFI_STATUS
MshcSetClockState (IN UINT32 State)
{
  UINT32 Timeout = MAX_RETRY_COUNT;

  // Set the New Clock State
  MmioWrite32 (gMshcBaseAddr + MSHCI_CLKENA, State);
  MmioWrite32 (gMshcBaseAddr + MSHCI_CMD, 0);
  MmioWrite32 (gMshcBaseAddr + MSHCI_CMD, CMD_ONLY_CLK);

  // Get the Current MSHCI Status
  while ((MmioRead32 (gMshcBaseAddr + MSHCI_CMD) & CMD_STRT_BIT) != 0) {
    if (Timeout == 0) {
      DEBUG ((EFI_D_ERROR, "%a: Timeout when Setting the Clock State to %u!\n", __FUNCTION__, State));
      return EFI_TIMEOUT;
    }

    Timeout--;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MshcSetClockFrequency (IN UINTN Frequency)
{
  EFI_STATUS Status;

  // Before Setting the Clock Frequency, Disable the Card Clock
  Status = MshcSetClockState (CLK_DISABLE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Disable the Card Clock! Status = %r\n", Status));
    return Status;
  }

  // Set the new Clock Frequency
  switch (Frequency) {
    case MSHC_CLK_400:
      MmioWrite32 (gMshcBaseAddr + MSHCI_CLKDIV, 125);
      break;
      
    case MSHC_CLK_25M:
      MmioWrite32 (gMshcBaseAddr + MSHCI_CLKDIV, 2);
      break;
      
    case MSHC_CLK_50M:
      MmioWrite32 (gMshcBaseAddr + MSHCI_CLKDIV, 1);
      MshcSetDdr (BUSMODE_DDR);
      break;
      
    default:
      DEBUG ((EFI_D_WARN, "%a: Invalid Clock Frequency specified, not Changing.\n", __FUNCTION__));
  }

  // Enable the Card Clock
  Status = MshcSetClockState (CLK_ENABLE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable the Card Clock! Status = %r\n", Status));
    return Status;
  }

  return Status;
}

EFI_STATUS
MshcSendCmd (
  UINTN Cmd,
  UINTN CmdInterruptEnableVal,
  UINTN CmdArgument)
{
  UINTN  MmcStatus  = 0;
  UINTN  RetryCount = 0;
  UINT32 CmdFlags   = 0;
  UINT32 Timeout    = 0;

  Timeout = MAX_RETRY_COUNT;

  // Get Current MSHC Status
  while (MmioRead32 (gMshcBaseAddr + MSHCI_STATUS) & (DATA_BUSY)) {
    if (Timeout == 0) {
      DEBUG ((EFI_D_ERROR, "%a: The Controller didn't Release in Time!\n", __FUNCTION__));
      return EFI_DEVICE_ERROR;
    }
    Timeout--;
    MicroSecondDelay(5);
  }  

  // Clear the Raw Interrupt 
  MmioWrite32 (gMshcBaseAddr + MSHCI_RINTSTS, INTMSK_ALL);
  
  // Set Command Argument Register
  MmioWrite32 (gMshcBaseAddr + MSHCI_CMDARG, CmdArgument);

  // Send the Command
  CmdFlags = (Cmd & 0x3F);
  if (Cmd & (RSPTYP48 | RSPTYP48B | RSPTYP136)) {
    CmdFlags |= CMD_RESP_EXP_BIT;
    if (Cmd & RSPTYP136) {
      CmdFlags |= CMD_RESP_LENGTH_BIT;
    }
  }

  if ((Cmd == CMD17) | (Cmd == CMD18) | (Cmd == CMD8)) {
    CmdFlags |= CMD_DATA_EXP_BIT;
  }

  if ((Cmd == CMD24) | (Cmd == CMD25)) {
    CmdFlags |= CMD_DATA_EXP_BIT | CMD_RW_BIT;
  }

  if (Cmd & ENCMDCRC) {   
    CmdFlags |= CMD_CHECK_CRC_BIT;
  }
  CmdFlags |= (CMD_STRT_BIT | CMD_USE_HOLD_REG | CMD_WAIT_PRV_DAT_BIT | CMD_SENT_AUTO_STOP_BIT);
  
  MmioWrite32 ((gMshcBaseAddr + MSHCI_CMD), CmdFlags);

  // Check for the Raw Interrupt
  while (RetryCount != MAX_RETRY_COUNT) {
    MmcStatus = MmioRead32 (gMshcBaseAddr + MSHCI_RINTSTS);
    if (MmcStatus & INTMSK_CDONE) {
      break;
    }

    RetryCount++;
    MicroSecondDelay (10);
  }

  if (RetryCount == MAX_RETRY_COUNT) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d Timeout! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_TIMEOUT;
  }

  if (MmcStatus & INTMSK_RTO) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d Response Timeout! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_TIMEOUT;

  } else if (MmcStatus & INTMSK_RE) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d Response Error! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_DEVICE_ERROR;

  } else if (MmcStatus & INTMSK_RCRC) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d Response CRC Error! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_DEVICE_ERROR;

  } else if (MmcStatus & INTMSK_DCRC) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d Data CRC Error! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_DEVICE_ERROR;

  } else if (MmcStatus & INTMSK_HLE) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d HLE Error! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_DEVICE_ERROR;

  } else if (MmcStatus & INTMSK_SBE) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d SBE Error! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_DEVICE_ERROR;

  } else if (MmcStatus & INTMSK_EBE) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%d EBE Error! RINTSTS: 0x%x\n", __FUNCTION__, (Cmd & 0x3F), MmcStatus));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

VOID 
MshcSetDmaDesc (
  IN UINT8  *DmaDesc,
  IN UINT32  Flags,
  IN UINT32  Size,
  IN UINTN   Offset)
{
  ((struct MshcIdmaDesc *)(DmaDesc))->DmaFlags         = Flags;
  ((struct MshcIdmaDesc *)(DmaDesc))->Reserved1        = 0x0;
  ((struct MshcIdmaDesc *)(DmaDesc))->TransferSize     = Size;
  ((struct MshcIdmaDesc *)(DmaDesc))->Reserved2        = 0x0;
  ((struct MshcIdmaDesc *)(DmaDesc))->DataOffset       = Offset;
  ((struct MshcIdmaDesc *)(DmaDesc))->NextDescBase     = (UINTN)DmaDesc + sizeof (struct MshcIdmaDesc);
}

EFI_STATUS
MshcPrepareTransfer (
  IN OUT VOID           *Buffer, 
  IN     UINTN           BlockCount, 
  IN     OPERATION_TYPE  OperationType)
{
  EFI_STATUS           Status;
  struct MshcIdmaDesc *IdmaDesc;
  UINT32               Value;
  UINT32               DesFlag;
  UINTN                BlockCnt        = BlockCount;
  UINTN                DataSize        = BlockCount * BLEN_512BYTES;
  UINTN                DmaBufferSize   = BlockCount * BLEN_512BYTES + PHY_BUF_OFFSET;

  // Invalidate D-Cache to make Sure the Data is Written
  InvalidateDataCacheRange (Buffer, DataSize);

  // Allocate the Buffer for the DMA Transfer
  Status = DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (DmaBufferSize), (VOID **)&pgMshcDmaBaseAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate the DMA Buffer! Status = %r\n", Status));
    return Status;
  }

  // Map the DMA Buffer
  Status = DmaMap (MapOperationBusMasterCommonBuffer, (VOID *)pgMshcDmaBaseAddr, &DmaBufferSize, &gMshcDmaBaseAddr, &gDmaMapping);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Map the DMA Buffer! Status = %r\n", Status));
    return Status;
  }

  // Set the Descriptor Base Address
  IdmaDesc = (struct MshcIdmaDesc *)gMshcDmaBaseAddr;

  // Clear the DMA Buffer
  ZeroMem ((VOID *)gMshcDmaBaseAddr + PHY_BUF_OFFSET, DataSize);

  if (OperationType == WRITE) {
    CopyMem ((VOID *)gMshcDmaBaseAddr + PHY_BUF_OFFSET, Buffer, DataSize);
  }

  // Write and Invalidate D-Cache to make Sure the Buffer is Written to the Memory
  WriteBackInvalidateDataCacheRange ((VOID *)gMshcDmaBaseAddr + PHY_BUF_OFFSET, DataSize);

  // Reset FIFO
  MshcResetFifo ();

  // Reset IDMA
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_BMOD);
  Value |= BMOD_IDMAC_RESET;
  MmioWrite32 (gMshcBaseAddr + MSHCI_BMOD, Value);
  
  // Enable IDMA in CTRL
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_CTRL);
  Value &= ~(INT_ENABLE);
  Value &= ~(SEND_AS_CCSD);
  Value |= ENABLE_IDMAC | DMA_ENABLE;
  MmioWrite32 (gMshcBaseAddr + MSHCI_CTRL, Value);

  // Enable IDMA in BMOD
  Value  = MmioRead32 (gMshcBaseAddr + MSHCI_BMOD);
  Value |= BMOD_IDMAC_ENABLE | BMOD_IDMAC_FB;
  MmioWrite32 (gMshcBaseAddr + MSHCI_BMOD, Value);

  for (UINTN i = 0; ; i++) {
    // Set DMA Descriptors
    DesFlag  = MSHCI_IDMAC_OWN | MSHCI_IDMAC_CH;
    DesFlag |= (i == 0) ? MSHCI_IDMAC_FS : 0;
    if (BlockCnt <= 8) {
      // Set the Last Descriptor
      DesFlag |= MSHCI_IDMAC_LD;
      DesFlag &= ~(MSHCI_IDMAC_CH | MSHCI_IDMAC_DIC);
      MshcSetDmaDesc ((UINT8 *)IdmaDesc, DesFlag, BlockCnt * BLEN_512BYTES, ((UINT32)((gMshcDmaBaseAddr + PHY_BUF_OFFSET) + (UINT32)(i * PHY_BUF_SIZE))));
      break;
    }

    MshcSetDmaDesc ((UINT8 *)IdmaDesc, DesFlag, 8 * BLEN_512BYTES, ((UINT32)((gMshcDmaBaseAddr + PHY_BUF_OFFSET) + (UINT32)(i * PHY_BUF_SIZE))));

    BlockCnt -= 8;
    IdmaDesc++;
  }

  // Write and Invalidate D-Cache to make Sure the Descriptor is Written to the Memory
  WriteBackInvalidateDataCacheRange ((VOID *)gMshcDmaBaseAddr, (UINTN)(++IdmaDesc) - gMshcDmaBaseAddr);

  // Set the DMA Base Address
  MmioWrite32 (gMshcBaseAddr + MSHCI_DBADDRL, (UINT32)(gMshcDmaBaseAddr & 0xFFFFFFFF));
  MmioWrite32 (gMshcBaseAddr + MSHCI_DBADDRU, (UINT32)(gMshcDmaBaseAddr >> 32));

  // Set BLKSIZ
  MmioWrite32 (gMshcBaseAddr + MSHCI_BLKSIZ, BLEN_512BYTES);

  // Set BYTCNT
  MmioWrite32 (gMshcBaseAddr + MSHCI_BYTCNT, DataSize);

  // Set the Data Timeout Counter Value to the Max Value
  MmioWrite32 (gMshcBaseAddr + MSHCI_TMOUT, 0xFFFFFFFF);

  // Clear DMA Status
  MmioWrite32 (gMshcBaseAddr + MSHCI_IDSTS, 0xFFFFFFFF);

  // Clear Interrupt Status
  MmioWrite32 (gMshcBaseAddr + MSHCI_RINTSTS, INTMSK_ALL);

  return Status;
}

EFI_STATUS 
MshcReadDma (
  OUT VOID *Buffer, 
  IN UINTN  BlockCount)
{
  UINTN Count = MAX_RETRY_COUNT;

  while (Count) {
    if ((MmioRead32 (gMshcBaseAddr + MSHCI_RINTSTS)) & INTMSK_DTO) {
      // Invalidate D-Cache and Wait for Data to be Synchronized
      InvalidateDataCacheRange ((VOID *)gMshcDmaBaseAddr + PHY_BUF_OFFSET, BlockCount * BLEN_512BYTES);
      ArmDataSynchronizationBarrier ();

      // Copy Data
      CopyMem (Buffer, (VOID *)gMshcDmaBaseAddr + PHY_BUF_OFFSET, BlockCount * BLEN_512BYTES);

      // Disable the DMA Interrupt
      MmioWrite32 (gMshcBaseAddr + MSHCI_IDINTEN, 0x0);
      break;

    } else if ((MmioRead32 (gMshcBaseAddr + MSHCI_RINTSTS)) & (DATA_ERR | DATA_TOUT)) {
      DEBUG ((EFI_D_ERROR, "%a: Error when Reading DMA! RINTSTS: 0x%X, IDSTS: 0x%X\n", __FUNCTION__, MmioRead32 (gMshcBaseAddr + MSHCI_RINTSTS), MmioRead32 (gMshcBaseAddr + MSHCI_IDSTS)));
      
      // Disable the DMA Interrupt
      MmioWrite32 (gMshcBaseAddr + MSHCI_IDINTEN, 0x0);
      return EFI_DEVICE_ERROR;

    } else {
      Count--;
      MicroSecondDelay (10);
    }
  }

  if (Count == 0) {
    DEBUG ((EFI_D_ERROR, "%a: Timeout when Reading DMA! RINTSTS: 0x%X, IDSTS: 0x%X\n", __FUNCTION__, MmioRead32 (gMshcBaseAddr + MSHCI_RINTSTS), MmioRead32 (gMshcBaseAddr + MSHCI_IDSTS)));
      
    return EFI_TIMEOUT;
  }

  // Unmap DMA and Free the DMA Buffer
  if (gDmaMapping != NULL) {
    DmaUnmap (gDmaMapping);
    DmaFreeBuffer (EFI_SIZE_TO_PAGES (BlockCount * BLEN_512BYTES + PHY_BUF_OFFSET), &gMshcDmaBaseAddr);
  }

  return EFI_SUCCESS;
}

EFI_STATUS 
MshcWriteDma (
  IN VOID  *Buffer, 
  IN UINTN  BlockCount)
{
  // The DMA Buffer has already been Copied in MshcPrepareTransfer, there is no Need to Write anything
  
  // Wait for the Write to Finish
  while (!(MmioRead32 (gMshcBaseAddr + MSHCI_RINTSTS) & INTMSK_DTO));
  
  // Invalidate D-Cache and Wait for Data to be Synchronized
  InvalidateDataCacheRange ((VOID *)gMshcDmaBaseAddr + PHY_BUF_OFFSET, BlockCount * BLEN_512BYTES);
  ArmDataSynchronizationBarrier ();

  // Disable the DMA Interrupt
  MmioWrite32 (gMshcBaseAddr + MSHCI_IDINTEN, 0x0);

  // Unmap DMA and Free the DMA Buffer
  if (gDmaMapping != NULL) {
    DmaUnmap (gDmaMapping);
    DmaFreeBuffer (EFI_SIZE_TO_PAGES (BlockCount * BLEN_512BYTES + PHY_BUF_OFFSET), &gMshcDmaBaseAddr);
  }

  return EFI_SUCCESS;
}    
