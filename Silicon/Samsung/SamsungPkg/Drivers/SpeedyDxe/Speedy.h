/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _SPEEDY_H_
#define _SPEEDY_H_

//
// SPEEDY MMIO
//
#define SPEEDY_MMIO_LENGTH                           0x2000

//
// SPPEDY Controller Register Bits
//
#define SPEEDY_DATA_WIDTH_8BIT                       (0 << 8)
#define SPEEDY_ENABLE                                BIT0
#define SPEEDY_TIMEOUT_CMD_DISABLE                   BIT1
#define SPEEDY_TIMEOUT_STANDBY_DISABLE               BIT2
#define SPEEDY_TIMEOUT_DATA_DISABLE                  BIT3
#define SPEEDY_ALWAYS_PULLUP_EN                      BIT7
#define SPEEDY_REMOTE_RESET_REQ_EN                   BIT30
#define SPEEDY_SW_RST                                BIT31

//
// SPEEDY FIFO Controller Register Bits
//
#define SPEEDY_RX_TRIGGER_LEVEL(x)                   ((x) << 0)
#define SPEEDY_TX_TRIGGER_LEVEL(x)                   ((x) << 8)
#define SPEEDY_FIFO_DEBUG_INDEX                      (0 << 24)
#define SPEEDY_FIFO_RESET                            BIT31

//
// SPEEDY Command Register Bits
//
#define SPEEDY_ADDRESS(x)                            (((x) & 0xFFF) << 7)
#define SPEEDY_SLAVE_ADDRESS(x, y)                   (y + ((x & 0xF) << 8))
#define SPEEDY_BURST_LENGTH(x)                       ((x) << 0)
#define SPEEDY_BURST_FIXED                           (0 << 5)
#define SPEEDY_ACCESS_BURST                          (0 << 19)
#define SPEEDY_DIRECTION_READ                        (0 << 20)
#define SPEEDY_BURST_INCR                            BIT5
#define SPEEDY_BURST_EXTENSION                       (BIT5 | BIT6)
#define SPEEDY_ACCESS_RANDOM                         BIT19
#define SPEEDY_DIRECTION_WRITE                       BIT20

//
// SPEEDY Interrupt Enable Register Bits
//
#define SPEEDY_TRANSFER_DONE_EN                      BIT0
#define SPEEDY_TIMEOUT_CMD_EN                        BIT1
#define SPEEDY_TIMEOUT_STANDBY_EN                    BIT2
#define SPEEDY_TIMEOUT_DATA_EN                       BIT3
#define SPEEDY_FIFO_TX_ALMOST_EMPTY_EN               BIT4
#define SPEEDY_FIFO_RX_ALMOST_FULL_EN                BIT8
#define SPEEDY_RX_FIFO_INT_TRAILER_EN                BIT9
#define SPEEDY_RX_MODEBIT_ERR_EN                     BIT16
#define SPEEDY_RX_GLITCH_ERR_EN                      BIT17
#define SPEEDY_RX_ENDBIT_ERR_EN                      BIT18
#define SPEEDY_TX_LINE_BUSY_ERR_EN                   BIT20
#define SPEEDY_TX_STOPBIT_ERR_EN                     BIT21
#define SPEEDY_REMOTE_RESET_REQ                      BIT31

//
// SPEEDY Interupt Status Register Bits
//
#define SPEEDY_TRANSFER_DONE                         BIT0
#define SPEEDY_TIMEOUT_CMD                           BIT1
#define SPEEDY_TIMEOUT_STANDBY                       BIT2
#define SPEEDY_TIMEOUT_DATA                          BIT3
#define SPEEDY_FIFO_TX_ALMOST_EMPTY                  BIT4
#define SPEEDY_FIFO_RX_ALMOST_FULL                   BIT8
#define SPEEDY_RX_FIFO_INT_TRAILER                   BIT9
#define SPEEDY_RX_MODEBIT_ERR                        BIT16
#define SPEEDY_RX_GLITCH_ERR                         BIT17
#define SPEEDY_RX_ENDBIT_ERR                         BIT18
#define SPEEDY_TX_LINE_BUSY_ERR                      BIT20
#define SPEEDY_TX_STOPBIT_ERR                        BIT21
#define SPEEDY_REMOTE_RESET_REQ_STAT                 BIT31

//
// SPEEDY FIFO Status Register Bits
//
#define SPEEDY_VALID_DATA_CNT                        (0 << 0)
#define SPEEDY_FIFO_FULL                             BIT5
#define SPEEDY_FIFO_EMPTY                            BIT6

//
// SPEEDY Packet Gap Time Register Bits
//
#define SPEEDY_PACKET_GAP_TIME_CNT                   (0 << 16)
#define SPEEDY_PULL_EN_CNT                           (BIT0 | BIT1 | BIT2 | BIT3)

//
// SPEEDY Controller Status Register Bits
//
#define SPEEDY_FSM_IDLE                              BIT0
#define SPEEDY_FSM_INIT                              BIT1
#define SPEEDY_FSM_TX_CMD                            BIT2
#define SPEEDY_FSM_STANDBY                           BIT3
#define SPEEDY_FSM_DATA                              BIT4
#define SPEEDY_FSM_TIMEOUT                           BIT5
#define SPEEDY_FSM_TRANS_DONE                        BIT6
#define SPEEDY_FSM_IO_RX_STAT_MASK                   (BIT7 | BIT8)
#define SPEEDY_FSM_IO_TX_IDLE                        BIT9
#define SPEEDY_FSM_IO_TX_GET_PACKET                  BIT10

//
// SPEEDY Bus
//
typedef struct {
  UINT32 ctrl;
  UINT32 fifo_ctrl;
  UINT32 cmd;
  UINT32 int_enable;
  UINT32 int_status;
  UINT16 Reserved1[13];
  UINT32 fifo_status;
  UINT32 tx_data;
  UINT32 rx_data;
  UINT16 Reserved2[4];
  UINT32 packet_gap_time;
  UINT32 timeout_count;
  UINT16 Reserved3[90];
  UINT32 fifo_debug;
  UINT32 ctrl_status;
} EFI_SPEEDY_BUS;

#endif /* _SPEEDY_H_ */
