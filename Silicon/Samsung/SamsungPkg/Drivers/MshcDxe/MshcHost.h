/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _MSHC_HOST_H_
#define _MSHC_HOST_H_

#include <Library/MshcHostBridge.h>

#include "MshcDxe_CMD.h"

//
// Max Transfer Size
//
#define MAX_MSHC_TRANSFER_SIZE   (0x40000)

//
// MSHC Internal Registers
//
#define MSHCI_CTRL               (0x00)
#define MSHCI_PWREN              (0x04)
#define MSHCI_CLKDIV             (0x08)
#define MSHCI_CLKSRC             (0x0C)
#define MSHCI_CLKENA             (0x10)
#define MSHCI_TMOUT              (0x14)
#define MSHCI_CTYPE              (0x18)
#define MSHCI_BLKSIZ             (0x1C)
#define MSHCI_BYTCNT             (0x20)
#define MSHCI_INTMSK             (0x24)
#define MSHCI_CMDARG             (0x28)
#define MSHCI_CMD                (0x2C)
#define MSHCI_RESP0              (0x30)
#define MSHCI_RESP1              (0x34)
#define MSHCI_RESP2              (0x38)
#define MSHCI_RESP3              (0x3C)
#define MSHCI_MINTSTS            (0x40)
#define MSHCI_RINTSTS            (0x44)
#define MSHCI_STATUS             (0x48)
#define MSHCI_FIFOTH             (0x4C)
#define MSHCI_CDETECT            (0x50)
#define MSHCI_WRTPRT             (0x54)
#define MSHCI_GPIO               (0x58)
#define MSHCI_TCBCNT             (0x5C)
#define MSHCI_TBBCNT             (0x60)
#define MSHCI_DEBNCE             (0x64)
#define MSHCI_USRID              (0x68)
#define MSHCI_VERID              (0x6C)
#define MSHCI_HCON               (0x70)
#define MSHCI_UHS_REG            (0x74)
#define MSHCI_RST_N              (0x78)
#define MSHCI_BMOD               (0x80)
#define MSHCI_PLDMND             (0x84)

#define MSHCI_DBADDRL            (0x88)     // Wide Layout
#define MSHCI_DBADDRU            (0x8C)     // Wide Layout
#define MSHCI_IDSTS              (0x90)     // Wide Layout
#define MSHCI_DSCADDRL           (0x98)     // Wide Layout
#define MSHCI_DSCADDRU           (0x9C)     // Wide Layout
#define MSHCI_BUFADDRL           (0xA0)     // Wide Layout
#define MSHCI_BUFADDRU           (0xA4)     // Wide Layout

#define MSHCI_IDSTS_WIDE         (0x90)
#define MSHCI_IDINTEN_WIDE       (0x94)
#define MSHCI_IDINTEN_NARROW     (0x90)
#define MSHCI_CLKSEL_WIDE        (0xA8)
#define MSHCI_CLKSEL_NARROW      (0x9C)
#define MSHCI_CARDTHRCTL         (0x100)
#define MSHCI_BACK_END_POWER     (0x104)

#define MSHCI_FIFO_LEGACY        (0x100)
#define MSHCI_FIFO_240A          (0x200)
#define MSHC_VERID_240A          (0x240A)
#define GET_VERID(x)             ((x) & 0xFFFF)

//
//  Control Register Bits
//
#define CTRL_RESET               (0x1)
#define FIFO_RESET               (0x1 << 1)
#define DMA_RESET                (0x1 << 2)
#define INT_ENABLE               (0x1 << 4)
#define DMA_ENABLE               (0x1 << 5)
#define READ_WAIT                (0x1 << 6)
#define SEND_IRQ_RESP            (0x1 << 7)
#define ABRT_READ_DATA           (0x1 << 8)
#define SEND_CCSD                (0x1 << 9)
#define SEND_AS_CCSD             (0x1 << 10)
#define CEATA_INTSTAT            (0x1 << 11)
#define ENABLE_OD_PULLUP         (0x1 << 24)
#define ENABLE_IDMAC             (0x1 << 25)
#define CTRL_RESET_ALL           (CTRL_RESET | FIFO_RESET | DMA_RESET)

//
//  Power Enable Register Bits
//
#define POWER_ENABLE             (0x1)

//
//  Clock Enable Register Bits
//
#define CLK_ENABLE               (0x1)
#define CLK_DISABLE              (0x0)
#define CLK_LOW_POWER            (0x1 << 16)

//
//  Card Type Register Bits
//
#define CARD_WIDTH1              (0x0)
#define CARD_WIDTH4              (0x1)
#define CARD_WIDTH8              (0x1 << 16)

//
//  Interrupt Mask Register Bits
//
#define INTMSK_ALL               (0xFFFFFFFF)
#define INTMSK_CDETECT           (0x1)
#define INTMSK_RE                (0x1 << 1)
#define INTMSK_CDONE             (0x1 << 2)
#define INTMSK_DTO               (0x1 << 3)
#define INTMSK_TXDR              (0x1 << 4)
#define INTMSK_RXDR              (0x1 << 5)
#define INTMSK_RCRC              (0x1 << 6)
#define INTMSK_DCRC              (0x1 << 7)
#define INTMSK_RTO               (0x1 << 8)
#define INTMSK_DRTO              (0x1 << 9)
#define INTMSK_HTO               (0x1 << 10)
#define INTMSK_VOLT_SWITCH       (0x1 << 10)
#define INTMSK_FRUN              (0x1 << 11)
#define INTMSK_HLE               (0x1 << 12)
#define INTMSK_SBE               (0x1 << 13)
#define INTMSK_ACD               (0x1 << 14)
#define INTMSK_EBE               (0x1 << 15)

//
//  Command Register Bits
//
#define CMD_RESP_EXP_BIT         (0x1 << 6)
#define CMD_RESP_LENGTH_BIT      (0x1 << 7)
#define CMD_CHECK_CRC_BIT        (0x1 << 8)
#define CMD_DATA_EXP_BIT         (0x1 << 9)
#define CMD_RW_BIT               (0x1 << 10)
#define CMD_TRANSMODE_BIT        (0x1 << 11)
#define CMD_SENT_AUTO_STOP_BIT   (0x1 << 12)
#define CMD_WAIT_PRV_DAT_BIT     (0x1 << 13)
#define CMD_ABRT_CMD_BIT         (0x1 << 14)
#define CMD_SEND_INIT_BIT        (0x1 << 15)
#define CMD_UPD_CLK_ONLY_BIT     (0x1 << 21)
#define CMD_VOLT_SWITCH          (0x1 << 28)
#define CMD_USE_HOLD_REG         (0x1 << 29)
#define CMD_STRT_BIT             (0x1 << 31)
#define CMD_ONLY_CLK             (CMD_STRT_BIT | CMD_UPD_CLK_ONLY_BIT)

//
//  Raw Interrupt Register Bits
//
#define DATA_ERR                 (INTMSK_EBE | INTMSK_SBE | INTMSK_HLE | \
                                  INTMSK_FRUN | INTMSK_DCRC)
#define DATA_TOUT                (INTMSK_HTO | INTMSK_DRTO)
#define CMD_ERROR                (INTMSK_RCRC | INTMSK_RTO | INTMSK_RE)

//
//  Status Register Bits
//
#define FIFO_RXWTRMARK           (0x1)
#define FIFO_TXWTRMARK           (0x1 << 1)
#define FIFO_EMPTY               (0x1 << 2)
#define FIFO_FULL                (0x1 << 3)
#define DATA_3STATUS             (0x1 << 8)
#define DATA_BUSY                (0x1 << 9)
#define DATA_MCBUSY              (0x1 << 10)
#define GET_FIFO_COUNT(x)        (((x) >> 17) & 0x1FFF)

//
//  Hardware Configuration Register Bits
//
#define HCON_DATA_WIDTH(x)       (((x) >> 7) & 0x7)
#define HCON_WIDTH_16            (0)
#define HCON_WIDTH_32            (1)
#define HCON_WIDTH_64            (2)
#define HCON_ADDR_CONFIG(x)      (((x) >> 27) & 0x1)
#define HCON_ADDR_64BIT          (1)

//
//  FIFO Threshold Watermark Register Bits
//
#define MSHC_FIFO_DEPTH          (0x40)
#define TX_WMARK(x)              ((x) & 0xFFF)
#define RX_WMARK(x)              (((x) & 0xFFF) << 16)

// DW DMA Multiple Transaction Size
#define MSIZE_1                  (0 << 28)
#define MSIZE_4                  (1 << 28)
#define MSIZE_8                  (2 << 28)
#define MSIZE_16                 (3 << 28)
#define MSIZE_32                 (4 << 28)
#define MSIZE_64                 (5 << 28)
#define MSIZE_128                (6 << 28)
#define MSIZE_256                (7 << 28)

//
//  Card Detect Register Bits
//
#define CDETECT_CARD_PRESENT     (0x1)

//
//  Write Protect Register Bits
//
#define WRTPRT_ACTIVE            (0x1)

//
//  UHS & DDR Mode Register Bits
//
#define UHS_DDR                  (0x1 << 16)
#define UHS_NON_DDR              (0x0 << 16)
#define UHS_VOLT_18              (0x1)

//
//  Internal DMA Status Register Bits
//
#define IDSTS_TI                 (0x1)        // transmit interrupt
#define IDSTS_RI                 (0x1 << 1)   // receive interrupt
#define IDSTS_FBE                (0x1 << 2)   // fatal bus error
#define IDSTS_DU                 (0x1 << 4)   // descriptor unavailable
#define IDSTS_CES                (0x1 << 5)   // card error summary
#define IDSTS_NIS                (0x1 << 8)   // normal interrupt summary
#define IDSTS_AIS                (0x1 << 9)   // abnormal interrupt summary
#define IDSTS_ERROR              (IDSTS_FBE | IDSTS_DU | IDSTS_CES | IDSTS_AIS)

//
//  Internal DMA Descriptor
//
#define IDMAC_DES0_DIC           (0x1 << 1)   // no interrupt when this one is done
#define IDMAC_DES0_LD            (0x1 << 2)   // last descriptor of the transfer
#define IDMAC_DES0_FD            (0x1 << 3)   // first descriptor of the transfer
#define IDMAC_DES0_CH            (0x1 << 4)   // chained to the next descriptor
#define IDMAC_DES0_ER            (0x1 << 5)   // end of ring
#define IDMAC_DES0_OWN           (0x1u << 31) // owned by the hardware

#define IDMAC_DES2_SIZE_MASK     (0x1FFF)
#define IDMAC_BYTES_PER_DESC     (0x1000)
#define IDMAC_DESC_COUNT         (MAX_MSHC_TRANSFER_SIZE / IDMAC_BYTES_PER_DESC)

typedef struct {
  UINT32 Des0;
  UINT32 Des1;
  UINT32 Des2;
  UINT32 Des3;
  UINT32 BufferLow;
  UINT32 BufferHigh;
  UINT32 NextLow;
  UINT32 NextHigh;
} MSHC_IDMAC_DESC;

//
//  Security Management Unit
//
#define MSHCI_EMMCP_BASE         (0x1000)
#define MSHCI_MPSTAT             (MSHCI_EMMCP_BASE + 0x008)
#define MSHCI_MPSECURITY         (MSHCI_EMMCP_BASE + 0x010)
#define MSHCI_MPSCTRL0           (MSHCI_EMMCP_BASE + 0x20C)

#define MPSECURITY_DESCTYPE(x)   (((x) >> 19) & 0x3)
#define MPSECURITY_FMP_ENC_ON    (0x1 << 28)

//
//  Bus Mode Register Bits
//
#define BMOD_IDMAC_RESET         (0x1)
#define BMOD_IDMAC_FB            (0x1 << 1)
#define BMOD_IDMAC_ENABLE        (0x1 << 7)

//
//  Clock Selection Register Bits
//
#define CLKSEL_TIMING(Div, FineDrive, Drive, Sample) \
          ((((Div) & 0x7) << 24) | (((FineDrive) & 0x3) << 22) | \
           (((Drive) & 0x7) << 16) | ((Sample) & 0x7))
#define CLKSEL_RESET_MASK        ((0x3 << 6) | 0x7)
#define CLKSEL_SAMPLE_MASK       (0x7)
#define CLKSEL_SAMPLE(x)         ((x) & 0x7)
#define MSHC_TUNING_PHASES       (8)

//
// Tuning Parameters
//
#define MSHC_TUNING_MIN_RUN      (3)

//
// Retry Parameters
//
#define MAX_RETRY_COUNT          (100000)

//
// Bus Speeds
//
#define MSHC_CLK_400K            (400000)
#define MSHC_CLK_25M             (25000000)
#define MSHC_CLK_50M             (50000000)
#define MSHC_CLK_100M            (100000000)
#define MSHC_CLK_200M            (200000000)

typedef enum {
  READ,
  WRITE
} OPERATION_TYPE;

extern MSHC_HOST gMshcHost;

//
// Function Forward Declarations
//
VOID
MshcResetFifo (VOID);

EFI_STATUS
MshcResetAll (VOID);

EFI_STATUS
MshcInitialize (VOID);

EFI_STATUS
MshcSetClockFrequency (IN UINTN Frequency);

VOID
MshcSetBusWidth (IN UINT32 Width);

EFI_STATUS
MshcSendCmd (
  IN  UINTN   Cmd,
  IN  UINTN   CmdArgument,
  OUT UINT32 *Response OPTIONAL
  );

EFI_STATUS
MshcPrepareTransfer (
  IN VOID          *Buffer,
  IN UINTN          BlockSize,
  IN UINTN          BlockCount,
  IN OPERATION_TYPE OperationType
  );

EFI_STATUS
MshcReadData (
  OUT VOID  *Buffer,
  IN  UINTN  Length
  );

EFI_STATUS
MshcWriteData (
  IN VOID  *Buffer,
  IN UINTN  Length
  );

EFI_STATUS
MshcWaitAutoStop (VOID);

VOID
MshcDumpRegisters (VOID);

VOID
MshcRecover (VOID);

BOOLEAN
MshcIsDmaActive (VOID);

BOOLEAN
MshcIsCardBusy (VOID);

EFI_STATUS
MshcHostSwitchTo1V8 (VOID);

VOID
MshcSetSamplePhase (IN UINT32 Phase);

EFI_STATUS
MshcSetTiming (
  IN UINT32 ClkSelTiming,
  IN UINTN  Frequency
  );

#endif /* _MSHC_HOST_H_ */
