/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _MSHCDXE_CMD_H_
#define _MSHCDXE_CMD_H_

#define INDX(CMD_INDX)          ((CMD_INDX) & 0x3F)

#define RSP_PRESENT             (0x1 << 8)
#define RSP_136                 (0x1 << 9)
#define RSP_CRC                 (0x1 << 10)
#define RSP_BUSY                (0x1 << 11)
#define DATA_READ               (0x1 << 12)
#define DATA_WRITE              (0x1 << 13)
#define AUTO_STOP               (0x1 << 14)
#define INIT_SEQUENCE           (0x1 << 15)
#define VOLT_SWITCH             (0x1 << 16)

//
// Response Types
//
#define RSP_NONE                (0)
#define RSP_R1                  (RSP_PRESENT | RSP_CRC)
#define RSP_R1B                 (RSP_PRESENT | RSP_CRC | RSP_BUSY)
#define RSP_R2                  (RSP_PRESENT | RSP_136 | RSP_CRC)
#define RSP_R3                  (RSP_PRESENT)
#define RSP_R6                  (RSP_PRESENT | RSP_CRC)
#define RSP_R7                  (RSP_PRESENT | RSP_CRC)

//
// Command Definitions
//
#define CMD0                    (INDX (0)  | RSP_NONE | INIT_SEQUENCE)

#define CMD2                    (INDX (2)  | RSP_R2)

#define CMD3                    (INDX (3)  | RSP_R6)

#define CMD6                    (INDX (6)  | RSP_R1 | DATA_READ)

#define CMD7                    (INDX (7)  | RSP_R1B)

#define CMD8                    (INDX (8)  | RSP_R7)
#define CMD8_ARG                (0x1AA)
#define CMD8_RESPONSE_MASK      (0xFFF)

#define CMD9                    (INDX (9)  | RSP_R2)

#define CMD11                   (INDX (11) | RSP_R1 | VOLT_SWITCH)       // VOLTAGE_SWITCH
#define CMD12                   (INDX (12) | RSP_R1B)

#define CMD13                   (INDX (13) | RSP_R1)

#define CMD16                   (INDX (16) | RSP_R1)
#define CMD19                   (INDX (19) | RSP_R1 | DATA_READ)         // SEND_TUNING_BLOCK

#define CMD17                   (INDX (17) | RSP_R1 | DATA_READ)

#define CMD18                   (INDX (18) | RSP_R1 | DATA_READ  | AUTO_STOP)

#define CMD24                   (INDX (24) | RSP_R1 | DATA_WRITE)

#define CMD25                   (INDX (25) | RSP_R1 | DATA_WRITE | AUTO_STOP)

//
// eMMC Commands
//
#define MMC_CMD1                (INDX (1)  | RSP_R3)                     // SEND_OP_COND
#define MMC_CMD6                (INDX (6)  | RSP_R1B)                    // SWITCH
#define MMC_CMD8                (INDX (8)  | RSP_R1 | DATA_READ)         // SEND_EXT_CSD
#define MMC_CMD3                (INDX (3)  | RSP_R1)                     // SET_RELATIVE_ADDR
#define MMC_RCA                 (1)

#define CMD55                   (INDX (55) | RSP_R1)

#define ACMD6                   (INDX (6)  | RSP_R1)

#define ACMD41                  (INDX (41) | RSP_R3)

#define ACMD51                  (INDX (51) | RSP_R1 | DATA_READ)

#define OCR_BUSY                (0x80000000)
#define OCR_HCS                 (0x40000000)
#define OCR_CCS                 (0x40000000)
#define OCR_XPC                 (0x10000000)
#define OCR_S18R                (0x01000000)
#define OCR_S18A                (0x01000000)
#define OCR_VOLTAGE_WINDOW      (0x00FF8000)

#define SD_BUS_WIDTH_1          (0x0)
#define SD_BUS_WIDTH_4          (0x2)

#define SD_SWITCH_CHECK(Mode)   (0x00FFFFF0 | ((Mode) & 0xF))
#define SD_SWITCH_SET(Mode)     (0x80FFFFF0 | ((Mode) & 0xF))

//
// Access Modes in Function Group 1
//
#define SD_ACCESS_MODE_HS       (1)
#define SD_ACCESS_MODE_SDR50    (2)
#define SD_ACCESS_MODE_SDR104   (3)
#define SD_SWITCH_STATUS_SIZE   (64)

//
// 64 block wide tuning response
//
#define SD_TUNING_BLOCK_SIZE    (64)

//
// SD Configuration Register
//
#define SD_SCR_SIZE             (8)

#define SCR_STRUCTURE(x)        (((x) >> 28) & 0xF)
#define SCR_SPEC_VERSION(x)     (((x) >> 24) & 0xF)
#define SCR_BUS_WIDTHS(x)       (((x) >> 16) & 0xF)
#define SCR_SPEC3(x)            (((x) >> 15) & 0x1)
#define SCR_SPEC4(x)            (((x) >> 10) & 0x1)

#define SCR_BUS_WIDTH_1         (0x1)
#define SCR_BUS_WIDTH_4         (0x4)

#define SCR_SPEC_VER_100        (0)
#define SCR_SPEC_VER_110        (1)
#define SCR_SPEC_VER_200        (2)

// Tuning pattern for 4 bit bus.
#define SD_TUNING_PATTERN_4BIT { \
  0xFF, 0x0F, 0xFF, 0x00, 0xFF, 0xCC, 0xC3, 0xCC, \
  0xC3, 0x3C, 0xCC, 0xFF, 0xFE, 0xFF, 0xFE, 0xEF, \
  0xFF, 0xDF, 0xFF, 0xDD, 0xFF, 0xFB, 0xFF, 0xFB, \
  0xBF, 0xFF, 0x7F, 0xFF, 0x77, 0xF7, 0xBD, 0xEF, \
  0xFF, 0xF0, 0xFF, 0xF0, 0x0F, 0xFC, 0xCC, 0x3C, \
  0xCC, 0x33, 0xCC, 0xCF, 0xFF, 0xEF, 0xFF, 0xEE, \
  0xFF, 0xFD, 0xFF, 0xFD, 0xDF, 0xFF, 0xBF, 0xFF, \
  0xBB, 0xFF, 0xF7, 0xFF, 0xF7, 0x7F, 0x7B, 0xDE  \
}

#define CCC_SWITCH              (0x1 << 10) // Stupid quirk, overflows.

#define R1_APP_CMD              (0x1 << 5)
#define R1_READY_FOR_DATA       (0x1 << 8)
#define R1_CURRENT_STATE(x)     (((x) >> 9) & 0xF)
#define R1_ERROR_MASK           (0xFDF90000)
#define R1_SWITCH_ERROR         (0x1 << 7)

#endif /* _MSHCDXE_CMD_H_ */
