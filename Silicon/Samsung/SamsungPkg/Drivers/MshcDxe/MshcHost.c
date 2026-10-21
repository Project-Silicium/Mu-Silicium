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

#include <Library/BaseLib.h>
#include <Library/DmaLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MshcHostBridge.h>

#include "MshcHost.h"

//
// Global Variables
//

// Host
MSHC_HOST gMshcHost;

//
// DMA Parameters
//
STATIC BOOLEAN          mDmaUsable      = FALSE;
STATIC MSHC_IDMAC_DESC *mDescriptors    = NULL;
STATIC PHYSICAL_ADDRESS mDescriptorAddr = 0;
STATIC VOID            *mDescriptorMap  = NULL;
STATIC VOID            *mBufferMap      = NULL;
STATIC UINTN            mDmaBytes       = 0;
STATIC OPERATION_TYPE   mDmaOperation   = READ;

//
// Voltage Switching State
//
STATIC BOOLEAN mVoltageSwitching = FALSE;

VOID
MshcDumpRegisters (VOID)
{
  DEBUG ((EFI_D_ERROR,
          "MSHC: CTRL=%08X PWREN=%08X CLKDIV=%08X CLKENA=%08X CLKSEL=%08X\n",
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CTRL),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_PWREN),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CLKDIV),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CLKENA),
          MmioRead32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset)));

  DEBUG ((EFI_D_ERROR,
          "MSHC: CMD=%08X CMDARG=%08X RINTSTS=%08X STATUS=%08X\n",
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CMD),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CMDARG),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_STATUS)));

  DEBUG ((EFI_D_ERROR,
          "MSHC: CTYPE=%08X BLKSIZ=%08X BYTCNT=%08X TMOUT=%08X FIFOTH=%08X\n",
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CTYPE),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_BLKSIZ),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_BYTCNT),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_TMOUT),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_FIFOTH)));

  DEBUG ((EFI_D_ERROR,
          "MSHC: CDETECT=%08X WRTPRT=%08X VERID=%08X HCON=%08X TCBCNT=%08X TBBCNT=%08X\n",
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_CDETECT),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_WRTPRT),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_VERID),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_HCON),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_TCBCNT),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_TBBCNT)));
}

STATIC
BOOLEAN
MshcCtrlReset (IN UINT32 Reset)
{
  UINT32  SavedClkSel;
  BOOLEAN Done = FALSE;

  SavedClkSel = MmioRead32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset);
  MmioAnd32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset, ~(UINT32)CLKSEL_RESET_MASK);

  MmioOr32 (gMshcHost.BaseAddress + MSHCI_CTRL, Reset);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);

  for (UINTN Timeout = MAX_RETRY_COUNT; Timeout > 0; Timeout--) {
    if (!(MmioRead32 (gMshcHost.BaseAddress + MSHCI_CTRL) & Reset)) {
      Done = TRUE;
      break;
    }

    MicroSecondDelay (1);
  }

  MmioWrite32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset, SavedClkSel);

  if (!Done) {
    DEBUG ((EFI_D_ERROR, "%a: The Block Reset 0x%X didn't Complete in Time!\n", __FUNCTION__, Reset));
  }

  return Done;
}

STATIC
EFI_STATUS
MshcUpdateClockRegisters (VOID)
{
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CMDARG, 0);

  for (UINTN Retry = 10; Retry > 0; Retry--) {
    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CMD,
                 CMD_ONLY_CLK | (mVoltageSwitching ? CMD_VOLT_SWITCH : 0));

    for (UINTN Timeout = 1000; Timeout > 0; Timeout--) {
      if (!(MmioRead32 (gMshcHost.BaseAddress + MSHCI_CMD) & CMD_STRT_BIT)) {
        return EFI_SUCCESS;
      }

      MicroSecondDelay (1);
    }

    if (MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS) & INTMSK_HLE) {
      MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_HLE);
      return EFI_SUCCESS;
    }

    MshcCtrlReset (CTRL_RESET);
  }

  DEBUG ((EFI_D_ERROR, "%a: The Clock Update didn't Complete in Time!\n", __FUNCTION__));
  MshcDumpRegisters ();

  return EFI_TIMEOUT;
}

VOID
MshcResetFifo (VOID)
{
  MshcCtrlReset (FIFO_RESET);
}

EFI_STATUS
MshcResetAll (VOID)
{
  if (!MshcCtrlReset (CTRL_RESET_ALL)) {
    return EFI_TIMEOUT;
  }

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_ENABLE);

  return MshcUpdateClockRegisters ();
}

STATIC
VOID
MshcInitDma (VOID)
{
  EFI_STATUS Status;
  UINTN      Bytes    = IDMAC_DESC_COUNT * sizeof (MSHC_IDMAC_DESC);
  UINT32     Security = MmioRead32 (gMshcHost.BaseAddress + MSHCI_MPSECURITY);

  mDmaUsable = FALSE;

  DEBUG ((EFI_D_ERROR,
          "%a: MPSECURITY 0x%08X, MPSCTRL0 0x%08X, MPSTAT 0x%08X\n",
          __FUNCTION__,
          Security,
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_MPSCTRL0),
          MmioRead32 (gMshcHost.BaseAddress + MSHCI_MPSTAT)));

  // I can't test this so PIO it is for non 64 bit DMA.
  if (gMshcHost.ClkSelOffset != MSHCI_CLKSEL_WIDE) {
    DEBUG ((EFI_D_ERROR, "%a: The Controller uses Narrow Descriptors, Using PIO.\n", __FUNCTION__));
    return;
  }

  // Same with secure devices.
  if (MPSECURITY_DESCTYPE (Security) != 0 || (Security & MPSECURITY_FMP_ENC_ON)) {
    DEBUG ((EFI_D_ERROR,
            "%a: The Security Unit is enabled, Using PIO.\n",
            __FUNCTION__));
    return;
  }

  Status = DmaAllocateBuffer (EfiBootServicesData, EFI_SIZE_TO_PAGES (Bytes), (VOID **)&mDescriptors);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate the Descriptor Ring! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  Status = DmaMap (MapOperationBusMasterCommonBuffer, mDescriptors, &Bytes, 0, 0, &mDescriptorAddr, &mDescriptorMap);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Map the Descriptor Ring! Status = %r\n", __FUNCTION__, Status));
    DmaFreeBuffer (EFI_SIZE_TO_PAGES (Bytes), mDescriptors);
    mDescriptors = NULL;
    return;
  }

  mDmaUsable = TRUE;
}

EFI_STATUS
MshcInitialize (VOID)
{
  EFI_STATUS Status;

  UINT32  Version = GET_VERID (MmioRead32 (gMshcHost.BaseAddress + MSHCI_VERID));
  UINT32  Hcon    = MmioRead32 (gMshcHost.BaseAddress + MSHCI_HCON);
  BOOLEAN Wide    = (HCON_ADDR_CONFIG (Hcon) == HCON_ADDR_64BIT);

  gMshcHost.FifoOffset   = (Version < MSHC_VERID_240A) ? MSHCI_FIFO_LEGACY : MSHCI_FIFO_240A;
  gMshcHost.ClkSelOffset = Wide ? MSHCI_CLKSEL_WIDE : MSHCI_CLKSEL_NARROW;

  switch (HCON_DATA_WIDTH (Hcon)) {
    case HCON_WIDTH_64:
      gMshcHost.FifoItemBytes = 8;
      break;

    case HCON_WIDTH_32:
      gMshcHost.FifoItemBytes = 4;
      break;

    default:
      DEBUG ((EFI_D_ERROR,
              "%a: The Controller reports an Unsupported FIFO Width! HCON = 0x%08X\n",
              __FUNCTION__, Hcon));
      return EFI_UNSUPPORTED;
  }

  // Enable MSHC Power
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_PWREN, POWER_ENABLE);

  // Reset MSHC
  Status = MshcResetAll ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reset MSHC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Call SoC specific Post Reset
  MshcBoardPostReset (&gMshcHost);

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_FIFOTH,
               MSIZE_8 | TX_WMARK (MSHC_FIFO_DEPTH / 2) | RX_WMARK ((MSHC_FIFO_DEPTH / 2) - 1));

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_INTMSK, 0);
  MmioWrite32 (gMshcHost.BaseAddress + (Wide ? MSHCI_IDINTEN_WIDE : MSHCI_IDINTEN_NARROW), 0);

  // We are PIO.
  MmioAnd32 (gMshcHost.BaseAddress + MSHCI_CTRL, ~(UINT32)(ENABLE_IDMAC | DMA_ENABLE | INT_ENABLE));
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_BMOD, BMOD_IDMAC_RESET);

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_DEBNCE, 0xFFFFF);

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CTYPE, CARD_WIDTH1);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_TMOUT, 0xFFFFFFFF);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_BLKSIZ, 0x200);

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_UHS_REG, UHS_NON_DDR);
  MmioWrite32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset, gMshcHost.ClkSelTiming);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKSRC, 0);

  if (mDescriptors == NULL) {
    MshcInitDma ();
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MshcSetClockFrequency (IN UINTN Frequency)
{
  EFI_STATUS Status;
  UINTN      Divider;
  UINTN      InputClock;

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_DISABLE);

  Status = MshcUpdateClockRegisters ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable the Card Clock! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  MshcBoardSetCiuClock (&gMshcHost, Frequency);

  InputClock = gMshcHost.CiuClockHz / (gMshcHost.ClkSelDivider + 1);

  if (Frequency >= InputClock) {
    Divider = 0;
  } else {
    Divider = (InputClock + (2 * Frequency) - 1) / (2 * Frequency);

    if (Divider > 0xFF) {
      Divider = 0xFF;
    }
  }

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKDIV, Divider);

  Status = MshcUpdateClockRegisters ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set the Clock Divider! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_ENABLE);

  Status = MshcUpdateClockRegisters ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable the Card Clock! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((EFI_D_ERROR,
          "%a: Requested %u Hz, Running at %u Hz (CIU %u Hz, CLKDIV %u)\n",
          __FUNCTION__,
          Frequency,
          Divider ? (InputClock / (2 * Divider)) : InputClock,
          gMshcHost.CiuClockHz,
          Divider));

  return EFI_SUCCESS;
}

VOID
MshcSetBusWidth (IN UINT32 Width)
{
  UINT32 Type;

  switch (Width) {
    case 8:
      Type = CARD_WIDTH8;
      break;

    case 4:
      Type = CARD_WIDTH4;
      break;

    default:
      Type = CARD_WIDTH1;
      break;
  }

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CTYPE, Type);
}

VOID
MshcRecover (VOID)
{
  MshcCtrlReset (FIFO_RESET);
  MshcCtrlReset (CTRL_RESET);

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_ENABLE);

  MshcUpdateClockRegisters ();
}

STATIC
VOID
MshcWaitDataBusy (VOID)
{
  for (UINTN Retry = 2; Retry > 0; Retry--) {
    for (UINTN Timeout = 10000; Timeout > 0; Timeout--) {
      if (!(MmioRead32 (gMshcHost.BaseAddress + MSHCI_STATUS) & DATA_BUSY)) {
        return;
      }

      MicroSecondDelay (10);
    }

    MshcCtrlReset (FIFO_RESET);
    MshcCtrlReset (CTRL_RESET);

    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_ENABLE);
    MshcUpdateClockRegisters ();
  }

  DEBUG ((EFI_D_ERROR, "%a: DAT0 is still Busy, Issuing the Command anyway.\n", __FUNCTION__));
  MshcDumpRegisters ();
}

EFI_STATUS
MshcSendCmd (
  IN  UINTN   Cmd,
  IN  UINTN   CmdArgument,
  OUT UINT32 *Response OPTIONAL)
{
  UINT32 CmdFlags;
  UINT32 RawStatus = 0;
  UINTN  Timeout;

  MshcWaitDataBusy ();

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CMDARG, CmdArgument);

  CmdFlags = INDX (Cmd) | CMD_STRT_BIT | CMD_USE_HOLD_REG | CMD_WAIT_PRV_DAT_BIT;

  if (Cmd & RSP_PRESENT) {
    CmdFlags |= CMD_RESP_EXP_BIT;
  }
  if (Cmd & RSP_136) {
    CmdFlags |= CMD_RESP_LENGTH_BIT;
  }
  if (Cmd & RSP_CRC) {
    CmdFlags |= CMD_CHECK_CRC_BIT;
  }
  if (Cmd & (DATA_READ | DATA_WRITE)) {
    CmdFlags |= CMD_DATA_EXP_BIT;
  }
  if (Cmd & DATA_WRITE) {
    CmdFlags |= CMD_RW_BIT;
  }
  if (Cmd & AUTO_STOP) {
    CmdFlags |= CMD_SENT_AUTO_STOP_BIT;
  }
  if (Cmd & INIT_SEQUENCE) {
    CmdFlags |= CMD_SEND_INIT_BIT;
  }
  if (Cmd & VOLT_SWITCH) {
    CmdFlags |= CMD_VOLT_SWITCH;
  }

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CMD, CmdFlags);

  // Voltage switch commands report completion in a different bit than the normal.
  UINT32 Completed = INTMSK_CDONE;

  if (Cmd & VOLT_SWITCH) {
    Completed |= INTMSK_VOLT_SWITCH;
  }

  for (Timeout = MAX_RETRY_COUNT; Timeout > 0; Timeout--) {
    RawStatus = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS);

    if (RawStatus & (Completed | CMD_ERROR)) {
      break;
    }

    MicroSecondDelay (10);
  }

  if (Timeout == 0) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%u Timed Out! RINTSTS = 0x%08X\n", __FUNCTION__, INDX (Cmd), RawStatus));
    return EFI_TIMEOUT;
  }

  if (RawStatus & INTMSK_RTO) {
    DEBUG ((EFI_D_ERROR, "%a: CMD%u got no Response.\n", __FUNCTION__, INDX (Cmd)));
    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);
    return EFI_NO_RESPONSE;
  }

  if (RawStatus & (INTMSK_RCRC | INTMSK_RE)) {
    DEBUG ((EFI_D_ERROR,
            "%a: CMD%u Response Error! RINTSTS = 0x%08X\n",
            __FUNCTION__, INDX (Cmd), RawStatus));
    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);
    return EFI_DEVICE_ERROR;
  }

  if (Cmd & RSP_BUSY) {
    for (Timeout = MAX_RETRY_COUNT; Timeout > 0; Timeout--) {
      if (!(MmioRead32 (gMshcHost.BaseAddress + MSHCI_STATUS) & DATA_BUSY)) {
        break;
      }

      MicroSecondDelay (10);
    }

    if (Timeout == 0) {
      DEBUG ((EFI_D_ERROR, "%a: CMD%u stayed Busy!\n", __FUNCTION__, INDX (Cmd)));
      return EFI_TIMEOUT;
    }
  }

  if (Cmd & VOLT_SWITCH) {
    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_VOLT_SWITCH);
  }

  if (Response != NULL && (Cmd & RSP_PRESENT)) {
    if (Cmd & RSP_136) {
      Response[0] = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RESP3);
      Response[1] = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RESP2);
      Response[2] = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RESP1);
      Response[3] = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RESP0);
    } else {
      Response[0] = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RESP0);
    }
  }

  return EFI_SUCCESS;
}

/**
  Start DMA operations on MSHC.
**/
STATIC
EFI_STATUS
MshcStartDma (
  IN VOID          *Buffer,
  IN UINTN          Length,
  IN OPERATION_TYPE OperationType)
{
  EFI_STATUS       Status;
  PHYSICAL_ADDRESS DeviceAddress;
  UINTN            Mapped     = Length;
  UINTN            Remaining  = Length;
  UINTN            Index      = 0;

  Status = DmaMap ((OperationType == READ) ? MapOperationBusMasterWrite : MapOperationBusMasterRead,
                   Buffer, &Mapped, 0, 0, &DeviceAddress, &mBufferMap);

  if (EFI_ERROR (Status) || Mapped != Length) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Map the Transfer Buffer! Status = %r\n", __FUNCTION__, Status));
    return EFI_OUT_OF_RESOURCES;
  }

  mDmaBytes     = Length;
  mDmaOperation = OperationType;

  if (!gMshcHost.Coherent) {
    if (OperationType == WRITE) {
      WriteBackDataCacheRange (Buffer, Length);
    } else {
      InvalidateDataCacheRange (Buffer, Length);
    }
  }

  while (Remaining > 0) {
    UINTN Chunk = MIN (Remaining, IDMAC_BYTES_PER_DESC);

    mDescriptors[Index].Des0       = IDMAC_DES0_OWN | IDMAC_DES0_CH | IDMAC_DES0_DIC;
    mDescriptors[Index].Des1       = 0;
    mDescriptors[Index].Des2       = (UINT32)(Chunk & IDMAC_DES2_SIZE_MASK);
    mDescriptors[Index].Des3       = 0;
    mDescriptors[Index].BufferLow  = (UINT32)(DeviceAddress & 0xFFFFFFFF);
    mDescriptors[Index].BufferHigh = (UINT32)RShiftU64 (DeviceAddress, 32);

    if (Index == 0) {
      mDescriptors[Index].Des0 |= IDMAC_DES0_FD;
    }

    PHYSICAL_ADDRESS Next = mDescriptorAddr + ((Index + 1) * sizeof (MSHC_IDMAC_DESC));

    mDescriptors[Index].NextLow  = (UINT32)(Next & 0xFFFFFFFF);
    mDescriptors[Index].NextHigh = (UINT32)RShiftU64 (Next, 32);

    DeviceAddress += Chunk;
    Remaining     -= Chunk;
    Index++;
  }

  // The last one closes the transfer and has nothing after it
  mDescriptors[Index - 1].Des0 |= IDMAC_DES0_LD | IDMAC_DES0_ER;
  mDescriptors[Index - 1].Des0 &= ~(UINT32)(IDMAC_DES0_CH | IDMAC_DES0_DIC);

  if (!gMshcHost.Coherent) {
    WriteBackDataCacheRange (mDescriptors, Index * sizeof (MSHC_IDMAC_DESC));
  }

  // Handover to controller
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_IDSTS_WIDE, INTMSK_ALL);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_DBADDRL, (UINT32)(mDescriptorAddr & 0xFFFFFFFF));
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_DBADDRU, (UINT32)RShiftU64 (mDescriptorAddr, 32));

  MmioOr32 (gMshcHost.BaseAddress + MSHCI_CTRL, ENABLE_IDMAC | DMA_ENABLE);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_BMOD, BMOD_IDMAC_ENABLE | BMOD_IDMAC_FB);

  return EFI_SUCCESS;
}

/**
  Wait for DMA to finish.
**/
STATIC
EFI_STATUS
MshcFinishDma (IN VOID *Buffer)
{
  EFI_STATUS Status = EFI_TIMEOUT;

  for (UINTN Timeout = MAX_RETRY_COUNT; Timeout > 0; Timeout--) {
    UINT32 Raw  = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS);
    UINT32 Idsts = MmioRead32 (gMshcHost.BaseAddress + MSHCI_IDSTS_WIDE);

    if ((Raw & (DATA_ERR | DATA_TOUT)) || (Idsts & IDSTS_ERROR)) {
      DEBUG ((EFI_D_ERROR,
              "%a: The DMA Transfer Failed! RINTSTS = 0x%08X, IDSTS = 0x%08X\n",
              __FUNCTION__, Raw, Idsts));
      Status = EFI_DEVICE_ERROR;
      break;
    }

    if (Raw & INTMSK_DTO) {
      MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_DTO);
      Status = EFI_SUCCESS;
      break;
    }

    MicroSecondDelay (10);
  }

  if (Status == EFI_TIMEOUT) {
    DEBUG ((EFI_D_ERROR, "%a: The DMA Transfer never Completed!\n", __FUNCTION__));
    MshcDumpRegisters ();
  }

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_BMOD, BMOD_IDMAC_RESET);
  MmioAnd32 (gMshcHost.BaseAddress + MSHCI_CTRL, ~(UINT32)(ENABLE_IDMAC | DMA_ENABLE));

  if (mBufferMap != NULL) {
    DmaUnmap (mBufferMap);
    mBufferMap = NULL;
  }

  if (!gMshcHost.Coherent && mDmaOperation == READ && !EFI_ERROR (Status)) {
    InvalidateDataCacheRange (Buffer, mDmaBytes);
  }

  if (EFI_ERROR (Status)) {
    MshcRecover ();
  }

  return Status;
}

BOOLEAN
MshcIsDmaActive (VOID)
{
  return mDmaUsable;
}

BOOLEAN
MshcIsCardBusy (VOID)
{
  return !!(MmioRead32 (gMshcHost.BaseAddress + MSHCI_STATUS) & DATA_BUSY);
}

/**
  Switches Host to 1.8V
**/
EFI_STATUS
MshcHostSwitchTo1V8 (VOID)
{
  EFI_STATUS Status;

  mVoltageSwitching = TRUE;

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_DISABLE);
  MshcUpdateClockRegisters ();

  Status = MshcBoardSetVoltage (&gMshcHost, 1800);
  if (EFI_ERROR (Status)) {
    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_ENABLE);
    MshcUpdateClockRegisters ();

    mVoltageSwitching = FALSE;

    return Status;
  }

  MmioOr32 (gMshcHost.BaseAddress + MSHCI_UHS_REG, UHS_VOLT_18);

  MicroSecondDelay (10000);

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_CLKENA, CLK_ENABLE);
  MshcUpdateClockRegisters ();

  mVoltageSwitching = FALSE;

  return EFI_SUCCESS;
}

/**
  Puts the Bus on a new Timing and Speed together.
**/
VOID
MshcSetSamplePhase (IN UINT32 Phase)
{
  gMshcHost.ClkSelTiming &= ~(UINT32)CLKSEL_SAMPLE_MASK;
  gMshcHost.ClkSelTiming |= CLKSEL_SAMPLE (Phase);

  MmioWrite32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset, gMshcHost.ClkSelTiming);
}

EFI_STATUS
MshcSetTiming (
  IN UINT32 ClkSelTiming,
  IN UINTN  Frequency)
{
  gMshcHost.ClkSelTiming = ClkSelTiming;

  MmioWrite32 (gMshcHost.BaseAddress + gMshcHost.ClkSelOffset, ClkSelTiming);

  return MshcSetClockFrequency (Frequency);
}

EFI_STATUS
MshcPrepareTransfer (
  IN VOID          *Buffer,
  IN UINTN          BlockSize,
  IN UINTN          BlockCount,
  IN OPERATION_TYPE OperationType)
{
  MshcResetFifo ();

  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_BLKSIZ, BlockSize);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_BYTCNT, BlockSize * BlockCount);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_TMOUT, 0xFFFFFFFF);
  MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ALL);

  if (mDmaUsable) {
    return MshcStartDma (Buffer, BlockSize * BlockCount, OperationType);
  }

  return EFI_SUCCESS;
}

/**
  Reports the Data Phase Error Behind a Raw Interrupt Status Word.
**/
STATIC
EFI_STATUS
MshcCheckDataError (
  IN UINT32 RawStatus,
  IN CONST CHAR8 *Function)
{
  if (RawStatus & DATA_TOUT) {
    DEBUG ((EFI_D_ERROR, "%a: The Data Transfer Timed Out! RINTSTS = 0x%08X\n", Function, RawStatus));
    return EFI_TIMEOUT;
  }

  if (RawStatus & DATA_ERR) {
    DEBUG ((EFI_D_ERROR, "%a: The Data Transfer Failed! RINTSTS = 0x%08X\n", Function, RawStatus));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Waits for the Controller to Report that the Data Phase is Over.
**/
STATIC
EFI_STATUS
MshcWaitTransferDone (IN CONST CHAR8 *Function)
{
  UINT32 RawStatus = 0;

  for (UINTN Timeout = MAX_RETRY_COUNT; Timeout > 0; Timeout--) {
    RawStatus = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS);

    if (RawStatus & (DATA_ERR | DATA_TOUT)) {
      return MshcCheckDataError (RawStatus, Function);
    }

    if (RawStatus & INTMSK_DTO) {
      MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_DTO);
      return EFI_SUCCESS;
    }

    MicroSecondDelay (10);
  }

  DEBUG ((EFI_D_ERROR, "%a: The Data Transfer never Completed! RINTSTS = 0x%08X\n", Function, RawStatus));

  return EFI_TIMEOUT;
}

EFI_STATUS
MshcWaitAutoStop (VOID)
{
  for (UINTN Timeout = MAX_RETRY_COUNT; Timeout > 0; Timeout--) {
    if (MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS) & INTMSK_ACD) {
      MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_ACD);
      return EFI_SUCCESS;
    }

    MicroSecondDelay (10);
  }

  DEBUG ((EFI_D_ERROR, "%a: The Automatic Stop Command never Completed!\n", __FUNCTION__));

  return EFI_TIMEOUT;
}

EFI_STATUS
MshcReadData (
  OUT VOID  *Buffer,
  IN  UINTN  Length)
{
  UINT32 *Data      = (UINT32 *)Buffer;
  UINTN   WordsEach  = gMshcHost.FifoItemBytes / sizeof (UINT32);
  UINTN   Remaining = Length / gMshcHost.FifoItemBytes;
  UINTN   Timeout   = MAX_RETRY_COUNT;

  if (mDmaUsable) {
    return MshcFinishDma (Buffer);
  }

  ASSERT ((Length % gMshcHost.FifoItemBytes) == 0);

  while (Remaining > 0) {
    UINT32 RawStatus = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS);
    UINTN  Available;

    if (RawStatus & (DATA_ERR | DATA_TOUT)) {
      MshcCheckDataError (RawStatus, __FUNCTION__);
      MshcRecover ();
      return EFI_DEVICE_ERROR;
    }

    Available = GET_FIFO_COUNT (MmioRead32 (gMshcHost.BaseAddress + MSHCI_STATUS));

    if (Available == 0) {
      if (RawStatus & INTMSK_DTO) {
        DEBUG ((EFI_D_ERROR, "%a: The Card sent %u Bytes too Few!\n", __FUNCTION__, Remaining * gMshcHost.FifoItemBytes));
        MshcRecover ();
        return EFI_DEVICE_ERROR;
      }

      if (Timeout-- == 0) {
        DEBUG ((EFI_D_ERROR, "%a: The FIFO never Filled Up!\n", __FUNCTION__));
        MshcDumpRegisters ();
        MshcRecover ();
        return EFI_TIMEOUT;
      }

      MicroSecondDelay (10);
      continue;
    }

    if (Available > Remaining) {
      Available = Remaining;
    }

    for (UINTN i = 0; i < Available; i++) {
      for (UINTN Word = 0; Word < WordsEach; Word++) {
        *Data++ = MmioRead32 (gMshcHost.BaseAddress + gMshcHost.FifoOffset + (Word * sizeof (UINT32)));
      }
    }

    Remaining -= Available;
    Timeout    = MAX_RETRY_COUNT;

    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_RXDR);
  }

  return MshcWaitTransferDone (__FUNCTION__);
}

EFI_STATUS
MshcWriteData (
  IN VOID  *Buffer,
  IN UINTN  Length)
{
  UINT32 *Data      = (UINT32 *)Buffer;
  UINTN   WordsEach  = gMshcHost.FifoItemBytes / sizeof (UINT32);
  UINTN   Remaining = Length / gMshcHost.FifoItemBytes;
  UINTN   Timeout   = MAX_RETRY_COUNT;

  if (mDmaUsable) {
    return MshcFinishDma (Buffer);
  }

  ASSERT ((Length % gMshcHost.FifoItemBytes) == 0);

  while (Remaining > 0) {
    UINT32 RawStatus = MmioRead32 (gMshcHost.BaseAddress + MSHCI_RINTSTS);
    UINTN  Free;

    if (RawStatus & (DATA_ERR | DATA_TOUT)) {
      MshcCheckDataError (RawStatus, __FUNCTION__);
      MshcRecover ();
      return EFI_DEVICE_ERROR;
    }

    Free = MSHC_FIFO_DEPTH - GET_FIFO_COUNT (MmioRead32 (gMshcHost.BaseAddress + MSHCI_STATUS));

    if (Free == 0) {
      if (Timeout-- == 0) {
        DEBUG ((EFI_D_ERROR, "%a: The FIFO never Drained!\n", __FUNCTION__));
        MshcDumpRegisters ();
        MshcRecover ();
        return EFI_TIMEOUT;
      }

      MicroSecondDelay (10);
      continue;
    }

    if (Free > Remaining) {
      Free = Remaining;
    }

    for (UINTN i = 0; i < Free; i++) {
      for (UINTN Word = 0; Word < WordsEach; Word++) {
        MmioWrite32 (gMshcHost.BaseAddress + gMshcHost.FifoOffset + (Word * sizeof (UINT32)), *Data++);
      }
    }

    Remaining -= Free;
    Timeout    = MAX_RETRY_COUNT;

    MmioWrite32 (gMshcHost.BaseAddress + MSHCI_RINTSTS, INTMSK_TXDR);
  }

  return MshcWaitTransferDone (__FUNCTION__);
}
